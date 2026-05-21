#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

vector<string> parseCSVRow(const string& row_str) 
{
    vector<string> fields;
    string current_field = "";
    bool inside_quotes = false;

    for (size_t i = 0; i < row_str.length(); ++i) 
    {
        char c = row_str[i];

        if (inside_quotes) 
        {
            if (c == '"') 
            {
                if (i + 1 < row_str.length() && row_str[i + 1] == '"') 
                {
                    current_field += '"';
                    i++; 
                } 
                else 
                {
                    inside_quotes = false; 
                }
            } 
            else 
            {
                current_field += c; 
            }
        } 
        else 
        {
            if (c == '"') 
            {
                inside_quotes = true; 
            } 
            else if (c == ',') 
            {
                fields.push_back(current_field);
                current_field = ""; 
            } 
            else if (c == '\r' || c == '\n') 
            {
                continue; 
            }
            else 
            {
                current_field += c;
            }
        }
    }
    fields.push_back(current_field);
    return fields;
}

vector<string> tokenizeCommand(const string& command) {
    vector<string> tokens;
    stringstream ss(command);
    string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// === 3. 各項指令的實作 ===
void executeHelp() {
    cout << "Available Commands:\n";
    cout << "  SHOW COLUMNS  - Display table schema\n";
    cout << "  SELECT ...    - Execute query (e.g., SELECT Name, GPA WHERE ID = 1121516)\n";
    cout << "  --help        - Show this help message\n";
}

void executeShowColumns(const vector<string>& headers) {
    cout << "Table Columns:\n";
    for(size_t i = 0; i < headers.size(); ++i) {
        cout << "  [" << i << "] " << headers[i] << "\n";
    }
}

// 大魔王：進階版 SELECT 解析與執行
void executeSelect(const vector<string>& tokens, const vector<string>& headers) {
    if (tokens.size() < 4) {
        cout << "Syntax error. Example: SELECT Name, GPA WHERE ID = 1121516\n";
        return;
    }

    // 1. 尋找 WHERE 的位置
    int where_idx = -1;
    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "WHERE") {
            where_idx = i;
            break;
        }
    }

    if (where_idx == -1 || where_idx + 3 > tokens.size()) {
        cout << "Syntax error in WHERE clause.\n";
        return;
    }

    // 2. 解析要查詢的欄位 (把逗號清掉)
    vector<string> target_cols;
    bool select_all = false;
    for (int i = 1; i < where_idx; ++i) {
        string col = tokens[i];
        // 魔法在這裡：如果字串最後一個字是逗號，就把它彈掉 (pop_back)
        if (!col.empty() && col.back() == ',') {
            col.pop_back(); 
        }
        
        if (col == "*") {
            select_all = true;
        } else {
            target_cols.push_back(col);
        }
    }

    // 3. 解析 WHERE 後面的條件
    string cond_col = tokens[where_idx + 1]; // 例如: ID
    string cond_op  = tokens[where_idx + 2]; // 例如: =
    string cond_val = tokens[where_idx + 3]; // 例如: 1121516

    // 4. 對應欄位索引 (利用 headers 找出 ID 和 Name 在第幾格)
    int cond_col_idx = -1;
    vector<int> target_col_indices;

    for (size_t i = 0; i < headers.size(); ++i) {
        // 因為我們的標籤是 "ID:int"，要把 ":int" 切掉才能比對
        string raw_header = headers[i];
        size_t colon_pos = raw_header.find(':');
        string clean_header = (colon_pos != string::npos) ? raw_header.substr(0, colon_pos) : raw_header;

        // 找 WHERE 條件欄位的索引
        if (clean_header == cond_col) cond_col_idx = i;
        
        // 找 SELECT 要印出欄位的索引
        for (const string& target : target_cols) {
            if (clean_header == target) target_col_indices.push_back(i);
        }
    }

    if (cond_col_idx == -1) {
        cout << "Error: Column '" << cond_col << "' not found in table.\n";
        return;
    }

    // 5. 開檔搜尋並過濾
    ifstream file("students.csv");
    if (!file) {
        cout << "Error: Cannot open database file.\n";
        return;
    }

    string line;
    getline(file, line); // 跳過第一行標頭

    bool found = false;
    cout << "--------------------------------------------------\n";
    while (getline(file, line)) {
        if (line.empty()) continue;
        vector<string> fields = parseCSVRow(line);

        // 執行條件比對 (目前實作 =)
        if (fields.size() > cond_col_idx && fields[cond_col_idx] == cond_val) {
            found = true;
            
            // 決定要印出哪些欄位
            if (select_all || target_col_indices.empty()) {
                // 如果是 SELECT *，全印
                for (const auto& f : fields) cout << "[" << f << "] ";
                cout << "\n";
            } else {
                // 否則只印指定的欄位
                for (int idx : target_col_indices) {
                    cout << "[" << fields[idx] << "] ";
                }
                cout << "\n";
            }
        }
    }

    if (!found) cout << "No matching records found.\n";
    cout << "--------------------------------------------------\n";
    file.close();
}

// === 4. 核心路由大腦 (統整所有指令判斷) ===
// 注意：現在會多傳入 headers 給需要的指令
void processCommand(const vector<string>& tokens, const vector<string>& headers) {
    if (tokens.empty()) return;

    string primary_cmd = tokens[0];

    if (primary_cmd == "EXIT" || primary_cmd == "QUIT" || primary_cmd == "exit") {
        cout << "Goodbye!\n";
        exit(0);
    }
    else if (primary_cmd == "HELP" || primary_cmd == "--help" || primary_cmd == "-h") {
        executeHelp();
    }
    else if ((primary_cmd == "SHOW" || primary_cmd == "show") && tokens.size() > 1 && tokens[1] == "COLUMNS") {
        executeShowColumns(headers);
    }
    else if (primary_cmd == "SELECT" || primary_cmd == "select") {
        executeSelect(tokens, headers);
    }
    else {
        cout << "Unknown command. Type HELP for options.\n";
    }
}

// === 5. 主程式 ===
int main(int argc, char* argv[])
{
    string file_path = "students.csv";
    ifstream file(file_path);
    if (!file) {
        cerr << "Error: file could not be opened.\n";
        return 1;
    }

    // 啟動時讀取一次 Header
    string header_line;
    vector<string> headers;
    if (getline(file, header_line)) {
        headers = parseCSVRow(header_line);
    }
    file.close(); // 先關掉，等 SELECT 的時候再打開來掃描

    // 處理 CLI 參數模式
    if (argc > 1) {
        vector<string> cli_tokens;
        for (int i = 1; i < argc; ++i) {
            cli_tokens.push_back(argv[i]);
        }
        processCommand(cli_tokens, headers);
        return 0; 
    }

    // 處理 REPL 互動模式
    cout << "Database loaded. Entering interactive mode.\n";
    cout << "Type 'HELP', 'SHOW COLUMNS', 'SELECT...', or 'EXIT'.\n";
    cout << "--------------------------------------------------\n";

    string command;
    while (true) {
        cout << "> ";
        if (!getline(cin, command)) break;

        vector<string> tokens = tokenizeCommand(command);
        processCommand(tokens, headers);
    }

    return 0;
}
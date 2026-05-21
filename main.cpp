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

// === 3. 各項指令的實作 (骨架) ===
void executeHelp() {
    cout << "Available Commands:\n";
    cout << "  show columns  - Display table schema\n";
    cout << "  select ...    - (Under construction)\n";
    cout << "  help        - Show this help message\n";
}

void executeShowColumns() {
    cout << "[Debug] Executing: SHOW COLUMNS\n";
}

void executeSelect(const vector<string>& tokens) {
    cout << "[Debug] Executing: SELECT\n";
    cout << "Tokens received:\n";
    for(size_t i = 0; i < tokens.size(); ++i) {
        cout << "  [" << i << "] " << tokens[i] << "\n";
    }
}

// === 4. 核心路由大腦 (統整所有指令判斷) ===
// 只要傳入切好的單字陣列，它就會決定該呼叫誰
void processCommand(const vector<string>& tokens) {
    if (tokens.empty()) return;

    string primary_cmd = tokens[0];

    if (primary_cmd == "exit" || primary_cmd == "quit") {
        cout << "Goodbye!\n";
        exit(0); // 強制結束程式
    }
    else if (primary_cmd == "help" || primary_cmd == "--help" || primary_cmd == "-h") {
        executeHelp();
    }
    else if (primary_cmd == "show" && tokens.size() > 1 && tokens[1] == "columns") {
        executeShowColumns();
    }
    else if (primary_cmd == "select" || primary_cmd == "SELECT") {
        executeSelect(tokens);
    }
    else {
        cout << "Unknown command: ";
        for(const auto& t : tokens) cout << t << " ";
        cout << "\n";
    }
}


// === 5. 主程式 ===
int main(int argc, char* argv[])
{
    // ... (一樣先開檔跟讀取 Header) ...
    string file_path = "students.csv";
    ifstream file(file_path);
    if (!file) {
        cerr << "Error: file could not be opened.\n";
        return 1;
    }

    // --- 新增：處理外部傳入的 CLI 參數 ---
    // argc 至少會是 1 (代表程式自己的名字)
    // 如果 > 1，代表使用者有加上 --help 或其他指令
    if (argc > 1) {
        vector<string> cli_tokens;
        // 把 argv[1] 到最後一個參數通通塞進 vector
        for (int i = 1; i < argc; ++i) {
            cli_tokens.push_back(argv[i]);
        }
        
        // 丟給大腦處理，處理完就直接結束，不進入迴圈
        processCommand(cli_tokens);
        return 0; 
    }

    // --- 如果沒有外部參數，才進入 REPL 互動迴圈 ---
    cout << "Database loaded. Entering interactive mode.\n";
    cout << "Type 'HELP', 'SHOW COLUMNS', 'SELECT...', or 'EXIT'.\n";
    cout << "--------------------------------------------------\n";

    string command;
    while (true) 
    {
        cout << "> ";
        if (!getline(cin, command)) break;

        vector<string> tokens = tokenizeCommand(command);
        processCommand(tokens);
    }

    file.close();
    return 0;
}
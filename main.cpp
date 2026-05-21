#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
            // 如果在引號內，遇到引號要特別處理
            if (c == '"') 
            {
                // 檢查是否為連續兩個引號 (轉義)
                if (i + 1 < row_str.length() && row_str[i + 1] == '"') 
                {
                    current_field += '"';
                    i++; // 跳過下一個引號
                } 
                else 
                {
                    inside_quotes = false; // 離開引號模式
                }
            } 
            else 
            {
                current_field += c; // 引號內的逗號會被當作一般字元收錄
            }
        } 
        else 
        {
            // 不在引號內，遇到逗號就切開
            if (c == '"') 
            {
                inside_quotes = true; // 進入引號模式
            } 
            else if (c == ',') 
            {
                fields.push_back(current_field);
                current_field = ""; // 清空，準備裝下一個格子
            } 
            else if (c == '\r' || c == '\n') 
            {
                continue; // 略過換行符號
            }
            else 
            {
                current_field += c;
            }
        }
    }
    // 把最後一格也放進去
    fields.push_back(current_field);
    return fields;
}

int main()
{
    string file_path = "students.csv";
    ifstream file(file_path);

    if (!file) 
    {
        cerr << "file could not be opened: " << file_path << "\n";
        return 1;
    }

    cout << "--------------------------------------------------\n";

    string line;
    int row_count = 0;

    // 逐行讀取
    while (getline(file, line)) 
    {
        if (line.empty()) continue; // 跳過空行

        // 丟給 FSM 解析器去切
        vector<string> fields = parseCSVRow(line);

        // 印出結果，用中括號包起來，證明我們切得很乾淨
        cout << "第 " << row_count << " 行: ";
        for (size_t i = 0; i < fields.size(); ++i) 
        {
            cout << "[" << fields[i] << "] ";
        }
        cout << "\n";

        row_count++;
    }

    file.close();
    cout << "--------------------------------------------------\n";

    return 0;
}
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

bool SplitIntoWords(const string& text, const string buzzword) {
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (word == buzzword)
            {
                return true;
            }
            
            word = "";
        }
        else {
            word += c;
        }
    }

    if (word == buzzword)
    {
        return true;
    }

    return false;
}

void run() {
    const int queryCount = ReadLineWithNumber();

    vector<string> queries(queryCount);
    for (string& query : queries) {
        query = ReadLine();
    }
    const string buzzword = ReadLine();

    cout << count_if(queries.begin(), queries.end(), [buzzword](const string& query) {
            // Реализуйте эту лямбда-функцию
            // Верните true, если query содержит слово buzzword
            return SplitIntoWords(query, buzzword);
        });
    cout << endl;
}
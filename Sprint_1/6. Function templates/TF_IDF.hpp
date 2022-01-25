#include <vector>
#include <iostream>
#include <string>

using namespace std;

vector<double> ComputeTfIdfs(vector<vector<string>> documents, string word)
{
    vector<double> results;
    return results;
}

int run() {
    
    const vector<vector<string>> documents = 
    {
        {"белый"s, "кот"s, "и"s, "модный"s, "ошейник"s},
        {"пушистый"s, "кот"s, "пушистый"s, "хвост"s},
        {"ухоженный"s, "пёс"s, "выразительные"s, "глаза"s},
    };

    const auto& tf_idfs = ComputeTfIdfs(documents, "кот"s);
    
    for (const double tf_idf : tf_idfs) {
        cout << tf_idf << " "s;
    }
    
    cout << endl;

    return 0;
}
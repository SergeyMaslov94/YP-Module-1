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
        {"�����"s, "���"s, "�"s, "������"s, "�������"s},
        {"��������"s, "���"s, "��������"s, "�����"s},
        {"���������"s, "��"s, "�������������"s, "�����"s},
    };

    const auto& tf_idfs = ComputeTfIdfs(documents, "���"s);
    
    for (const double tf_idf : tf_idfs) {
        cout << tf_idf << " "s;
    }
    
    cout << endl;

    return 0;
}
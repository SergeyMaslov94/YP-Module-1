#include <iostream>
#include <string>

using namespace std;

template <typename F>
string BruteForce(F check) {
    // верните строку str, для которой check(str) будет true
    for (auto letter1 = 'A'; letter1 <= 'Z';) {
        for (auto letter2 = 'A'; letter2 <= 'Z';) {
            for (auto letter3 = 'A'; letter3 <= 'Z';) {
                for (auto letter4 = 'A'; letter4 <= 'Z';) {
                    for (auto letter5 = 'A'; letter5 <= 'Z';) {
                        string word = { letter1, letter2, letter3, letter4, letter5 };
                        if (check(word))
                            return word;
                        ++letter5;
                    }
                    ++letter4;
                }
                ++letter3;
            }
            ++letter2;
        }
        ++letter1;
    }

    return "";
}

int lesson_7_1() {

    string pass = "ARTUR"s;
    
    auto check = [pass](const string& s) {
        return s == pass;
    };

    cout << BruteForce(check) << endl;
    return 0;
}
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Lang {
    string name;
    int age;
};

int top_1_lesson_0() {
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    // Выведите первый язык, начинающийся на C, используя алгоритм find_if
    auto result = find_if(langs.begin(), langs.end(), [](const string word){if(word[0] == 'C') return true; return false;});
    cout <<*result << endl;
    return 0;
}

int top_1_lesson_1() {
    vector<Lang> langs = {{"Python"s, 29}, {"Java"s, 24}, {"C#"s, 20}, {"Ruby"s, 25}, {"C++"s, 37}};
    auto result_it = find_if(langs.begin(), langs.end(), [](const Lang lang){if(lang.name[0] == 'J') return true; return false;});
    cout << (*result_it).name << ", "s << (*result_it).age << endl;

    return 0;
}
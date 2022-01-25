#include <vector>
#include <iostream>
#include <set>
#include <map>

using namespace std;
template <typename first, typename second>
ostream& operator<<(ostream& out, const pair<first, second>& container) {
    cout << '{';
    cout << container.first << ", "s << container.second;
    cout << '}';
    return out;
}

template <typename Documents>
void Print(ostream& out, Documents container)
{
    bool is_first = true;
    for (const auto& element : container) {
        if (!is_first) {
            out << ", "s;
        }
        is_first = false;
        out << element;
    }
}

template <typename Element>
ostream& operator<<(ostream& out, const vector<Element>& container) {
    cout << '[';
    Print(out, container);
    cout << ']';
    return out;
}

template <typename Element>
ostream& operator<<(ostream& out, const set<Element>& container) {
    cout << '{';
    Print(out, container);
    cout << '}';
    return out;
}

template <typename Key, typename Value>
ostream& operator<<(ostream& out, const map<Key, Value>& container) {
    cout << '<';
    Print(out, container);
    cout << '>';
    return out;
}

int run() {
    const vector<int> ages = { 10, 5, 2, 12 };
    cout << ages << endl;

    const set<string> cats = { "Мурка"s, "Белка"s, "Георгий"s, "Рюрик"s };
    cout << cats << endl;

    const map<string, int> cat_ages = {
    {"Murka"s, 10},
    {"Belka"s, 5},
    {"Georgi"s, 2},
    {"Rurik"s, 12}
    };

    cout << cat_ages << endl;

    return 0;
}
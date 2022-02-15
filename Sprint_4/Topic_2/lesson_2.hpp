//==========================================
// Урок 2. Поиск в неотсортированном векторе
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

void PrintSpacesPositions(string& str) {
    // напишите реализацию
    auto begin = str.begin();
    auto end = str.end();
    auto size = distance(begin, end);

    for (auto iter = begin; distance(begin, iter) < size;) {
        auto find_iter = std::find(iter, end, ' ');
        if (find_iter == end)
            return;

        cout << distance(begin, find_iter) << endl;
        iter = next(find_iter, 1);
    }
}

int top_2_lesson_2() {
    string str = "He said: one and one and one is three"s;
    PrintSpacesPositions(str);
    return 0;
}
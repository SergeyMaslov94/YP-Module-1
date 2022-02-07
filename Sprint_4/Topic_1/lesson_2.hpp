#include <set>
#include <iostream>

using namespace std;

template<typename Iter>
void PrintRange(Iter start, Iter stop) {
    while(start != stop) {
        cout << *start << " "s;
        start++;
    }
    cout << endl;
}

template<typename Container, typename Element>
void FindAndPrint(Container container, const Element& element) {
    auto iter = find(container.begin(), container.end(), element);
    PrintRange(container.begin(), iter);
    PrintRange(iter, container.end());
}

int top_1_lesson_2() {
    set<int> test = {1, 1, 1, 2, 3, 4, 5, 5};
    FindAndPrint(test, 3);
    FindAndPrint(test, 0);
    return 0;
}
#include <stack>
#include <iostream>
#include <vector>
using namespace std;

template <typename Type>
class SortedStack {
public:
    void Push(const Type& element) {
        if (elements_.empty()) {
            elements_.push(element);
            counter++;
            return;
        }

        Type last_elem = elements_.top();
        counter_2++;
        if (last_elem < element) {
            elements_.pop();
            Push(element);

            elements_.push(last_elem);
            counter++;
        }
        else {
            elements_.push(element);
            counter++;
        }
    }

    int counter = 0;
    int counter_2 = 0;

    // остальные функции не понадобятся
private:
    stack<Type> elements_;
};

int theory_1() {
    std::string s = "10 10 10 10 101"s;
    std::vector<string> sv(10, s);

    SortedStack<int> stack;
    for (auto i = 1; i < 101; i++) {
        stack.Push(i);
    }

    cout << stack.counter << " " << stack.counter_2 << endl;

    return 0;
}
#include <iostream>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Type>
class Stack {
public:
    void Push(const Type& element) {
        // напишите реализацию
        elements_.push_back(element);
    }
    void Pop() {
        // напишите реализацию
        elements_.pop_back();
    }

    const Type& Peek() const {
        // напишите реализацию
        return elements_.back();
    }
    Type& Peek() {
        // напишите реализацию
        return elements_.back();
    }

    void Print() const {
        PrintRange(elements_.begin(), elements_.end());
    }
    uint64_t Size() const {
        return elements_.size();
    }
    bool IsEmpty() const {
        return elements_.empty();
    }

private:
    vector<Type> elements_;
};
//=================================
template <typename Type>
class StackMin {
public:
    void Push(const Type& element) {
        // напишите реализацию метода
        elements_.Push(element);

        if(minimum_.IsEmpty()) {
            minimum_.Push(element);
            return;
        }

        if(element < minimum_.Peek())
            minimum_.Push(element);
        else
            minimum_.Push(minimum_.Peek());
    }
    void Pop() {
        // напишите реализацию метода
        elements_.Pop();
        minimum_.Pop();
    }
    const Type& Peek() const {
        return elements_.Peek();
    }
    Type& Peek() {
        return elements_.Peek();
    }
    void Print() const {
        // работу этого метода мы проверять не будем,
        // но если он вам нужен, то можете его реализовать
        elements_.Print();
    }
    uint64_t Size() const {
        return elements_.size();
    }
    bool IsEmpty() const {
        return elements_.IsEmpty();
    }
    const Type& PeekMin() const {
        // напишите реализацию метода
        return minimum_.Peek();
    }
    Type& PeekMin() {
        // напишите реализацию метода
        return minimum_.Peek();
    }
private:
    Stack<Type> elements_;
    Stack<Type> minimum_;
    // возможно, здесь вам понадобится что-то изменить
};

//=================================
// Пример использования
int top_3_lesson_1_2() {
    StackMin<int> stack;
    vector<int> values {3, 2, 1, 1, 2};

    // заполняем стек
    for (int i = 0; i < 5; ++i) {
        stack.Push(values[i]);
    }

    // печатаем стек и его минимум, постепенно убирая из стека элементы
    while (!stack.IsEmpty()) {
        stack.Print();
        cout << "Минимум = "s << stack.PeekMin() << endl;
        stack.Pop();
    }
    return 0;
}
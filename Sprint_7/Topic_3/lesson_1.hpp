#include <iostream>
#include <string>

using namespace std;

class Number {
private:
    int a_;

public:
    Number() {
        cout << "Default Ctor"s << endl;
    }
    Number(int num)
        : a_(num)
    {
        cout << "Ctor(int)"s << endl;
    }
    Number(const Number& other)
        : a_(other.a_)
    {
        cout << "Copy Ctor"s << endl;
    }
    ~Number() {
        cout << "Dtor"s << endl;
    }
    Number& operator=(const Number& other) {
        a_ = other.a_;
        cout << "Op="s << endl;
        return *this;
    }
    Number operator+(const Number& second) {
        cout << "Op+" << endl;
        return a_ + second.a_;
    }
};

int lesson_1() {
    Number a(1), b(1), c;
    c = a + b;
    cout << "End of calculations"s << endl;
    return 0;
}
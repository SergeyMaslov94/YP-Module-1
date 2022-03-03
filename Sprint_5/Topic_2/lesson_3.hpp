#include "../Topic_1/log_duration.h"
#include <iostream>
#include <vector>

using namespace std;

class StreamUntier {
public:
    // добавьте конструктор, деструктор
    // и дополнительные поля класса при необходимости
    StreamUntier(std::istream& stream) :
        m_cin(stream),
        tied_before_(stream.tie(nullptr)) // отвязываем поток ввода от потока выводв
    {}

    ~StreamUntier() {
        m_cin.tie(tied_before_); // привязываем снова
    }

    std::istream& m_cin;
private:
    std::ostream* tied_before_ = nullptr;
};

int lesson_3() {
    {
        LOG_DURATION("\\n with tie"s);

        StreamUntier guard(cin);
        int i;
        while (cin >> i) {
            cout << i * i << "\n"s;
        }
    }

    return 0;
}
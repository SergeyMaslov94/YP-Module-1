// Перегрузка операторов ввода-вывода
#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

class Rational {
public:
    Rational() = default;

    Rational(int numerator)
        : numerator_(numerator)
        , denominator_(1) {
    }

    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator) {
        Normalize();
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

private:
    void Normalize() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        const int divisor = gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }

    int numerator_ = 0;
    int denominator_ = 1;
};

ostream& operator<<(ostream& output, const Rational& rational)
{
    /* Выводим содержимое объекта value в output */
    output << rational.Numerator() << "/";
    output << rational.Denominator();

    // Оператор должен вернуть ссылку на переданный поток вывода
    return output;
}

istream& operator>>(istream& input, Rational& rational)
{
    /* Выводим содержимое объекта value в output */
    int x, y = 0;
    char comma; // переменная для считывания запятой

    input >> x >> comma >> y;

    rational = Rational(x,y);

    // Оператор должен вернуть ссылку на переданный поток вывода
    return input;
}

int top_2_lesson_2()
{
    Rational rational1;
    Rational rational2;

    std::cin >> rational1 >> rational2;

    std::cout << rational1 << " "s << rational2;

    return 0;
}
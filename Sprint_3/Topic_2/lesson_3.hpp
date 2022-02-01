// Арифметические операции с одинаковыми типами аргументов
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
//----------------------------------------------
// Перегрузка операции сложения для типа Rational
Rational operator+(Rational left, Rational right)
{
    const int numerator = left.Numerator() * right.Denominator() +
            right.Numerator() * left.Denominator();

    const int denominator = left.Denominator() * right.Denominator();

    return {numerator, denominator};
}

//----------------------------------------------
// Перегрузка оператора вывода для типа Rational
ostream& operator<<(ostream& output, const Rational& rational)
{
    /* Выводим содержимое объекта value в output */
    output << rational.Numerator() << "/";
    output << rational.Denominator();

    // Оператор должен вернуть ссылку на переданный поток вывода
    return output;
}

//----------------------------------------------
// Перегрузка оператора ввода для типа Rational
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

//----------------------------------------------
// Перегрузка операции вычитания для типа Rational
Rational operator-(Rational left, Rational right)
{
    const int numerator = left.Numerator() * right.Denominator() -
            right.Numerator() * left.Denominator();

    const int denominator = left.Denominator() * right.Denominator();

    return {numerator, denominator};
}

//----------------------------------------------
// Перегрузка операции унарного плюса для типа Rational
Rational operator+(Rational r)
{
    return r;
}

//----------------------------------------------
// Перегрузка операции унарного минуса для типа Rational
Rational operator-(Rational v) {
    return{-v.Numerator(), v.Denominator()};
}


int top_2_lesson_3()
{
    Rational rational1;
    Rational rational2;

    std::cin >> rational1 >> rational2;

    auto sum  = rational1 + rational2;
    auto def  = rational2 - rational1;

    Rational r_plus = + rational1;
    Rational r_minus = - rational1;

    std::cout << sum;

    return 0;
}
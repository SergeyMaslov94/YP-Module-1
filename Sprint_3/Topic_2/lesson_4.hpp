// Перегрузка операций присваивания
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

    //------------------------------------
    /* Перегрузка операций присваивания */
    Rational& operator+=(Rational rational)
    {
        numerator_ = numerator_*rational.denominator_ + denominator_*rational.numerator_;
        denominator_ *= rational.denominator_;

        Normalize();
        return *this;
    }

    Rational& operator-=(Rational rational)
    {
        numerator_ = numerator_*rational.denominator_ - denominator_*rational.numerator_;
        denominator_ *= rational.denominator_;

        Normalize();
        return *this;
    }

    Rational& operator *= (Rational rational)
    {
        numerator_ *= rational.numerator_;
        denominator_ *= rational.denominator_;

        Normalize();
        return *this;
    }

    Rational& operator /= (Rational rational)
    {
        numerator_ *= rational.denominator_;
        denominator_ *= rational.numerator_;

        Normalize();
        return *this;
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
//---------------------------------------
/* Перегрузка бинарных операции для типа Rational */
// Сложение
Rational operator+(Rational left, Rational right)
{
    return left += right;
}
// Вычитание
Rational operator-(Rational left, Rational right)
{
    return left -= right;
}

// Умножение
Rational operator*(Rational left, Rational right)
{
    return left *= right;
}

// Деление
Rational operator/(Rational left, Rational right)
{
    return left /= right;
}
//-------------------------------------------------------
/* Перегрузка оператора ввода/вывода для типа Rational */
// Вывод
ostream& operator<<(ostream& output, const Rational& rational)
{
    /* Выводим содержимое объекта value в output */
    output << rational.Numerator() << "/";
    output << rational.Denominator();

    // Оператор должен вернуть ссылку на переданный поток вывода
    return output;
}
// Ввод
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
/* Перегрузка операции унарного плюса и минуса для типа Rational */
Rational operator+(Rational r)
{
    return r;
}

Rational operator-(Rational v) {
    return{-v.Numerator(), v.Denominator()};
}


int top_2_lesson_4()
{
    Rational rational1(5,4);
    Rational rational2(3,4);
    Rational rational3 = rational1 / rational2;

    cout << rational1 << endl;
    cout << rational2 << endl;
    cout << rational3 << endl;

    std::cin >> rational1 >> rational2;

    auto sum  = rational1 + rational2;
    auto def  = rational2 - rational1;

    Rational r_plus = + rational1;
    Rational r_minus = - rational1;

    std::cout << sum;

    return 0;
}


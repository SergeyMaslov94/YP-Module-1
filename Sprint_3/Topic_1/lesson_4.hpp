// Урок 4. Список инициализаций конструктора

/*
Перепишите конструкторы класса Rational, чтобы инициализация его полей выполнялась в списках инициализации.
*/

#include <vector>
#include <iostream>
#include <numeric>

using namespace std;

class Rational {
public:
    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

    //-----------------------------------------------
    // конструкторы класса Rational

    Rational():
        numerator_(0),
        denominator_(1)
    {}

    Rational(int numerator)
        : denominator_(1),
          numerator_(numerator)
    {
    }

    Rational(int numerator, int denominator)
        : denominator_(denominator),
        numerator_(numerator)
    {
        Normalize();
    }
    //-----------------------------------------------

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

    int numerator_;
    int denominator_;
};


Rational Add(Rational r1, Rational r2) {
    int numerator = r1.Numerator() * r2.Denominator() + r2.Numerator() * r1.Denominator();
    int denominator = r1.Denominator() * r2.Denominator();

    // Создаём и возвращаем дробь с заданным числителем и знаменателем
    return Rational{ numerator, denominator };
}

int top_1_lesson_4()
{
    Rational zero;     // Дробь 0/1 = 0
    const Rational seven(7); // Дробь 7/1 = 7
    const Rational one_third(1, 3); // Дробь 1/3

    vector<Rational> numbers;
    numbers.push_back(Rational{ 7, 8 });

    // Следующие 2 строки эквивалентны - добавляют в numbers дробь 3/1
    numbers.push_back(Rational{ 3 });
    numbers.push_back(3);

    Rational sum = Add(Rational{ 1,-6 }, one_third);
    // Выведет 1/2
    cout << sum.Numerator() << "/" << sum.Denominator();

    return 0;
}
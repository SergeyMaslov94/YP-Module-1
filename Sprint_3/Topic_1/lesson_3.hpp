// Урок 3. Параметризированный конструктор

/*
В дополнение к конструктору по умолчанию для класса Rational разработайте такие параметризованные конструкторы:
Конструктор с одним параметром, создающий дробь из целого числа. Числитель такой дроби равен переданному целому числу, а знаменатель — единице.
Конструктор, который принимает два параметра типа int, задающих значение числителя и знаменателя, а затем приводит дробь к несократимому виду 
с положительным знаменателем. Например, дробь 6/15 должна быть преобразована в 2/5, а дробь 8/(-6) — в (-4)/3. Гарантируется, что знаменатель не равен нулю.
Эти конструкторы сделают возможными такие действия над обыкновенными дробями:
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

    Rational()
    {
        numerator_ = 0;
        denominator_ = 1;
    }

    Rational(int numerator)
    {
        numerator_ = numerator;
        denominator_ = 1;
    }

    Rational(int numerator, int denominator)
    {
        if (denominator < 0)
        {
            numerator = -numerator;
            denominator = -denominator;
        }

        auto value = std::gcd(numerator, denominator);

        numerator_ = numerator/value;
        denominator_ = denominator/value;
    }

private:
    int numerator_ = 0;
    int denominator_ = 1;
};


Rational Add(Rational r1, Rational r2) {
    int numerator = r1.Numerator() * r2.Denominator() + r2.Numerator() * r1.Denominator();
    int denominator = r1.Denominator() * r2.Denominator();

    // Создаём и возвращаем дробь с заданным числителем и знаменателем
    return Rational{ numerator, denominator };
}

int top_1_lesson_3() 
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
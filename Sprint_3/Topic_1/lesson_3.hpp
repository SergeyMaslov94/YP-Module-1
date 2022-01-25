// ���� 3. ������������������� �����������

/*
� ���������� � ������������ �� ��������� ��� ������ Rational ������������ ����� ����������������� ������������:
����������� � ����� ����������, ��������� ����� �� ������ �����. ��������� ����� ����� ����� ����������� ������ �����, � ����������� � �������.
�����������, ������� ��������� ��� ��������� ���� int, �������� �������� ��������� � �����������, � ����� �������� ����� � ������������� ���� 
� ������������� ������������. ��������, ����� 6/15 ������ ���� ������������� � 2/5, � ����� 8/(-6) � � (-4)/3. �������������, ��� ����������� �� ����� ����.
��� ������������ ������� ���������� ����� �������� ��� ������������� �������:
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

    // ������ � ���������� ����� � �������� ���������� � ������������
    return Rational{ numerator, denominator };
}

int top_1_lesson_3() 
{
    Rational zero;     // ����� 0/1 = 0
    const Rational seven(7); // ����� 7/1 = 7
    const Rational one_third(1, 3); // ����� 1/3

    vector<Rational> numbers;
    numbers.push_back(Rational{ 7, 8 });

    // ��������� 2 ������ ������������ - ��������� � numbers ����� 3/1
    numbers.push_back(Rational{ 3 });
    numbers.push_back(3);

    Rational sum = Add(Rational{ 1,-6 }, one_third);
    // ������� 1/2
    cout << sum.Numerator() << "/" << sum.Denominator();

    return 0;
}
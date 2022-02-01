// ���������� ���������� �����-������
#include <iostream>
#include <vector>
#include <string>

using namespace std;
struct MyValue
{
    MyValue():
        first(""s),
        second(""s),
        third(""s)
    {}

    string first;
    string second;
    string third;
};

ostream& operator<<(ostream& output, const MyValue& values) 
{
    /* ������� ���������� ������� value � output */
   
    output << values.first << ", ";
    output << values.second << ", ";
    output << values.third;

    // �������� ������ ������� ������ �� ���������� ����� ������
    return output;
}

istream& operator>>(istream& input, MyValue& values) 
{
    /* ������� ���������� ������� value � output */
    input >> values.first;
    input >> values.second;
    input >> values.third;

    // �������� ������ ������� ������ �� ���������� ����� ������
    return input;
}

int top_2_theory_1()
{
    MyValue my_values;

    std::cin >> my_values;
    std::cout << my_values;

	return 0;
}
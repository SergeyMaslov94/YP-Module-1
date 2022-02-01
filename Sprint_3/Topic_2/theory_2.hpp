// Перегрузка операторов ввода-вывода
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
    /* Выводим содержимое объекта value в output */
   
    output << values.first << ", ";
    output << values.second << ", ";
    output << values.third;

    // Оператор должен вернуть ссылку на переданный поток вывода
    return output;
}

istream& operator>>(istream& input, MyValue& values) 
{
    /* Выводим содержимое объекта value в output */
    input >> values.first;
    input >> values.second;
    input >> values.third;

    // Оператор должен вернуть ссылку на переданный поток вывода
    return input;
}

int top_2_theory_2()
{
    MyValue my_values;

    std::cin >> my_values;
    std::cout << my_values;

	return 0;
}
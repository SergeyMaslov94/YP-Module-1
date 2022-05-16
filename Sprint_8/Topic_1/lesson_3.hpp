#include <algorithm>
#include <execution>
#include <iostream>
#include <string_view>
#include <functional>

using namespace std;

int lesson_3() {
    const string_view s = "pretty little octopus"sv;
    vector<int> marks(s.size());
    
    {
        // концепция MapReduce: функция transform
        transform(execution::par, s.begin(), s.end(), marks.begin(),
            [](char c) { return c == ' '; });

        // выведет 000000100000010000000
        for (const int mark : marks) {
            cout << mark;
        }
        cout << endl;

        // концепция MapReduce: функция reduce 
        cout << reduce(execution::par, marks.begin(), marks.end()) + 1
            << " words"s << endl;
    }

    /*
        Функция transform_reduce позволяет обойтись без вектора для хранения
        результатов между стадиями. В неё, в отличие от reduce, нужно обязательно
        передать операцию для группировки объектов. Но нет нужды писать лямбду.
        Подключите библиотеку <functional> и создайте функциональный объект plus<>{},
        который, будучи вызванным как функция, суммирует аргументы
    */
    {
        const string_view s = "pretty little octopus"sv;
        const int space_count = std::transform_reduce(
            execution::par, // для демонстрации, можно убрать 
            s.begin(), s.end(), // входной диапазон
            0, // начальное значение
            plus<>{}, // reduce-операция (группирующая функция)
            [](char c) {return c == ' '; }  // map-операция
        );

        cout << space_count + 1 << " words"s << endl;  // вывод: 3 words
    }


    return 0;
}
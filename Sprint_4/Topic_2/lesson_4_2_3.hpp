// ====================================================
// Урок 4. Задание 2.
// Поиск в отсортированном векторе, словаре и множестве
#include <iostream>
#include <set>
#include <algorithm>
#include <vector>

using namespace std;
/*
//===================================================================================================================
Задание 2
Напишите функцию FindStartsWith, которая:
-принимает отсортированный набор строк в виде итераторов range_begin, range_end и один символ prefix;
-возвращает полуинтервал [range_begin, range_end) строк в виде пары итераторов. 
Эти строки начинаются с символа prefix.

Если итоговый диапазон пуст, его границы должны указывать на место в контейнере, куда можно без нарушения порядка 
сортировки вставить любую строку, начинающуюся с символа prefix — подобно алгоритму equal_range. 
Гарантируется, что строки состоят лишь из строчных латинских букв, а символ prefix — строчная латинская буква.
*/

// Префикс - СИМВОЛ
template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, char prefix) {
    
    string str1(1, prefix);
    // Все строки, начинающиеся с '<prefix>', больше или равны строке "<prefix>"
    auto left = lower_bound(range_begin, range_end, str1);

    // Составим следующий в алфавите символ.
    // Не страшно, если prefix = 'z':
    //в этом случае мы получим следующий за 'z' символ в таблице символов
    char next_prefix = static_cast<char>(prefix + 1);

    // Строка "<next_prefix>" в рамках буквенных строк
    // является точной верхней гранью
    // множества строк, начнающихся с '<prefix>'
    string str2(1, next_prefix);
    auto right = lower_bound(range_begin, range_end, str2);

    return { left, right };
}

//===================================================================================================================
// Задание 3
// Обобщим предыдущую задачу.Теперь префикс будет не одним символом типа char, а строкой с любым количеством символов.
// Префикс - СТРОКА
template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, string prefix) {

    auto left = lower_bound(range_begin, range_end, prefix);

    string next_prefix(prefix.begin(), prefix.end());
    auto& symbol = next_prefix.back();
    symbol = static_cast<char>(symbol + 1);

    auto right = lower_bound(range_begin, range_end, next_prefix);

    return { left, right };
}

int top_2_lesson_4_2_3() {
//===================================================================================
// Задание 2
    const vector<string> sorted_strings_1 = { "moscow", "murmansk", "vologda" };

    const auto m_result = FindStartsWith(begin(sorted_strings_1), end(sorted_strings_1), 'm');
    for (auto it = m_result.first; it != m_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    const auto p_result = FindStartsWith(begin(sorted_strings_1), end(sorted_strings_1), 'p');
    cout << (p_result.first - begin(sorted_strings_1)) << " " << (p_result.second - begin(sorted_strings_1)) << endl;

    const auto z_result = FindStartsWith(begin(sorted_strings_1), end(sorted_strings_1), 'z');
    cout << (z_result.first - begin(sorted_strings_1)) << " " << (z_result.second - begin(sorted_strings_1)) << endl;

//===================================================================================
// Задание 3
    const vector<string> sorted_strings_2 = { "moscow", "motovilikha", "murmansk" };

    const auto mo_result = FindStartsWith(begin(sorted_strings_2), end(sorted_strings_2), "mo");
    for (auto it = mo_result.first; it != mo_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    const auto mt_result = FindStartsWith(begin(sorted_strings_2), end(sorted_strings_2), "mt");
    cout << (mt_result.first - begin(sorted_strings_2)) << " " << (mt_result.second - begin(sorted_strings_2)) << endl;

    const auto na_result = FindStartsWith(begin(sorted_strings_2), end(sorted_strings_2), "na");
    cout << (na_result.first - begin(sorted_strings_2)) << " " << (na_result.second - begin(sorted_strings_2)) << endl;

    return 0;
}
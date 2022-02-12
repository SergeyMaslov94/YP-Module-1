// Урок 8. Стандартные алгоритмы из algorithm
#include <algorithm>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

using namespace std;
//======================================================================
// ЗАДАНИЕ 1
/*
// функция, записывающая элементы диапазона в строку
template <typename It>
string PrintRangeToString(It range_begin, It range_end) {
    // удобный тип ostringstream -> https://ru.cppreference.com/w/cpp/io/basic_ostringstream
    ostringstream out;
    for (auto it = range_begin; it != range_end; ++it) {
        out << *it << " "s;
    }
    out << endl;
    // получаем доступ к строке с помощью метода str для ostringstream
    return out.str();
}

// Функция, выполняющая перестановку
template<typename It>
std::vector<string> GetPermutations(It first, It last) {
    std::vector<string> result;
    std::sort(first, last, [](int left, int right){return left > right;});
    result.push_back(PrintRangeToString(first,last));

    while(std::prev_permutation(first, last)) {
        result.push_back(PrintRangeToString(first,last));
    }

    return result;
}

int top_1_lesson_8() {
    vector<int> permutation(3);
    // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
    // Заполняет диапазон последовательно возрастающими значениями
    iota(permutation.begin(), permutation.end(), 1);
    auto result = GetPermutations(permutation.begin(), permutation.end());
    for (const auto& s : result) {
        std::cout << s << " ";
    }
    std::cout << endl;

    return 0;
}
 */
//======================================================================
// ЗАДАНИЕ 2
template <typename RandomIt>
int Merge(RandomIt range_begin, RandomIt range_end)
{
    auto size_range = range_end - range_begin;

    std::vector<int> dst;
    std::merge(range_begin, range_begin + size_range, range_begin + size_range, range_end, std::back_inserter(dst));

    return 0;
}


template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {

    auto size_range = range_end - range_begin;

    if(size_range == 1)
        return;

    MergeSort(range_begin, range_begin + size_range/2);
    MergeSort(range_begin + size_range/2, range_end);

    std::vector<int> dst;
    std::inplace_merge(range_begin, range_begin + size_range/2, range_end);
}


int top_1_lesson_8() {
    vector<int> test_vector{9, 2, 10, 3, 1, 6, 8, 4, 5, 7};

    // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
    // Заполняет диапазон последовательно возрастающими значениями
    //iota(test_vector.begin(), test_vector.end(), 1);

    // random_shuffle   -> https://ru.cppreference.com/w/cpp/algorithm/random_shuffle
    // Перемешивает элементы в случайном порядке
    //random_shuffle(test_vector.begin(), test_vector.end());

    // Выводим вектор до сортировки
    PrintRange(test_vector.begin(), test_vector.end());

    // Сортируем вектор с помощью сортировки слиянием
    MergeSort(test_vector.begin(), test_vector.end());

    // Выводим результат
    PrintRange(test_vector.begin(), test_vector.end());

    return 0;
}

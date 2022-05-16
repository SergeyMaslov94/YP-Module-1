#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <execution>

#include "log_duration.hpp"

using namespace std;

string GenerateQuery(mt19937& generator, int max_length, int space_rate) {
    const int length = uniform_int_distribution(1, max_length)(generator);
    string query(length, ' ');
    for (char& c : query) {
        const int rnd = uniform_int_distribution(0, space_rate - 1)(generator);
        if (rnd > 0) {
            c = 'a' + (rnd - 1);
        }
    }
    return query;
}

template <typename Solver>
void Test(string_view mark, string_view s, Solver solver) {
    int result;
    {
        LOG_DURATION(mark);
        result = solver(s);
    }
    cout << result << endl;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define TEST(solver) Test(#solver, s, solver)

// использование алгоритма стандартной библиотеки
int CountWordsSTL(string_view str) {
    // подсчитайте количество слов
    return count(str.begin(), str.end(), ' ') + 1;
}

// использование паралельного алгоритма стандартной библиотеки
int CountWordsParallSTL(string_view str) {
    // подсчитайте количество слов
    return count(execution::par, str.begin(), str.end(), ' ') + 1;
}

// использование концепции MapReduce
int CountWordsMapReduce(string_view str) {
    // подсчитайте количество слов,
    // игнорируя начальные, конечные
    // и подряд идущие пробелы
    auto pos = str.find_first_not_of(" ");
    auto pos_end = str.npos;
    if (pos == pos_end) {
        return 0;
    }

    const int word_count = std::transform_reduce(
        str.begin() + pos, str.end() - 1, // входной диапазон
        str.begin() + pos + 1,
        0, // начальное значение
        plus<>{}, // reduce-операция (группирующая функция)
        [](char prev_c, char c) { 
            return (prev_c == ' ' && c != ' ');
        }
    );   

    return word_count + 1;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
int lesson_2() {	
    // должно вывести 3
    cout << CountWordsMapReduce("pretty little octopus"sv) << endl;

    mt19937 generator;

    const string s = GenerateQuery(generator, 100'000'000, 4);

    TEST(CountWordsSTL);
    TEST(CountWordsMapReduce);
    TEST(CountWordsParallSTL);

    return 0;
}
#include "../log_duration.hpp"

#include <algorithm>
#include <execution>
#include <iostream>
#include <numeric>
#include <random>
#include <string_view>
#include <vector>
#include <mutex>

using namespace std;

// максимальная величина числа
// constexpr означает, что эта переменная вычисляется на этапе компиляции
constexpr int MAX_VALUE = 1000;

// первая версия — с гонкой (БЫСТРО НО НЕ ВЕРНО)
vector<int> CountFrequenciesForEachRace(const vector<int>& numbers) {
    vector<int> freqs(MAX_VALUE + 1);
    for_each(
        execution::par,
        numbers.begin(), numbers.end(),
        [&freqs](int number) { ++freqs[number]; }
    );
    return freqs;
}
// вторая версия — с mutex (ВЕРНО НО ДОЛГО)
vector<int> CountFrequenciesForEachOneMutex(const vector<int>& numbers) {
    vector<int> freqs(MAX_VALUE + 1);
    mutex freqs_mutex;
    for_each(
        execution::par,
        numbers.begin(), numbers.end(),
        [&freqs, &freqs_mutex](int number) {
            lock_guard guard(freqs_mutex);
            ++freqs[number];
        }
    );
    return freqs;
}

// третья версия, с mutex в отношении конкретного элемента
vector<int> CountFrequenciesForEachSeparateMutexes(const vector<int>& numbers) {
    vector<int> freqs(MAX_VALUE + 1);
    vector<mutex> freqs_mutexes(MAX_VALUE + 1);
    for_each(
        execution::par,
        numbers.begin(), numbers.end(),
        [&freqs, &freqs_mutexes](int number) {
            lock_guard guard(freqs_mutexes[number]);
            ++freqs[number];
        }
    );
    return freqs;
}

// случайные count чисел из диапазона [0, MAX_VALUE]
vector<int> GenerateNumbers(mt19937& generator, int count, int max_value = MAX_VALUE) {
    vector<int> numbers(count);
    for (int& number : numbers) {
        number = uniform_int_distribution(0, max_value)(generator);
    }
    return numbers;
}

// тестирует время работы и выводит сумму всех частот
template <typename Function>
void Test(string_view mark, const vector<int>& numbers, Function function) {
    vector<int> freqs;
    {
        LOG_DURATION(mark);
        freqs = function(numbers);
    }
    cout << "sum = " << reduce(freqs.begin(), freqs.end()) << endl;
}

#define TEST(function) Test(#function, numbers, function)

int lesson_2() {
    mt19937 generator;
    const auto numbers = GenerateNumbers(generator, 100'000'000);

    TEST(CountFrequenciesForEachRace);
    TEST(CountFrequenciesForEachOneMutex);
    TEST(CountFrequenciesForEachSeparateMutexes);
    return 0;
}
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <string_view>
#include <vector>
#include <mutex>
#include <execution>

#include "../log_duration.hpp"

using namespace std;

string GenerateWord(mt19937& generator, int max_length) {
    const int length = uniform_int_distribution(1, max_length)(generator);
    string word;
    uniform_int_distribution<> distrib('a', 'z');
    word.reserve(length);
    for (int i = 0; i < length; ++i) {
        word.push_back(distrib(generator));
    }
    return word;
}

template <template <typename> typename Container>
Container<string> GenerateDictionary(mt19937& generator, int word_count, int max_length) {
    vector<string> words;
    words.reserve(word_count);
    for (int i = 0; i < word_count; ++i) {
        words.push_back(GenerateWord(generator, max_length));
    }
    return Container(words.begin(), words.end());
}

template <typename Strings, typename Predicate, typename Function>
void Test(string_view mark, const Strings& strings, Predicate predicate, Function function) {
    LOG_DURATION(mark);
    const auto result = function(strings, predicate);
    cout << result.size() << " " << result[5].substr(0, 5) << endl;
}

#define TEST(function) \
    Test(#function, strings, predicate, function<vector<string>, decltype(predicate)>)

template <typename Container, typename Predicate>
vector<typename Container::value_type> CopyIfUnordered(const Container& container,
    Predicate predicate) {
    vector<typename Container::value_type> result;
    for (const auto& value : container) {
        if (predicate(value)) {
            result.push_back(value);
        }
    }
    return result;
}

template <typename Container, typename Predicate>
vector<typename Container::value_type> CopyIfOrdered(const Container& container,
    Predicate predicate) {
    std::vector<std::mutex> result_mutexes(container.size());
    std::vector<size_t> idxs_elements(container.size());
    std::iota(idxs_elements.begin(), idxs_elements.end(), 0);
    vector<typename Container::value_type> result;
    result.reserve(container.size());
    std::mutex mut;

    std::for_each(
        std::execution::par,
        idxs_elements.begin(), idxs_elements.end(),
        [&container, &mut, &result, &predicate](size_t idx) {
            if (predicate(container[idx])) {
                std::lock_guard guard(mut);
                result.push_back(container[idx]);
            }
        }
    );

    return result;
}

int lesson_3() {
    vector<int> numbers(1000);
    iota(numbers.begin(), numbers.end(), 0);

    {
        const vector<int> even_numbers = CopyIfUnordered(numbers, [](int number) {
            return number % 2 == 0;
            });
        for (const int number : even_numbers) {
            cout << number << " "s;
        }
        cout << endl;
    }
    
    cout << "=================================" << endl;
    {
        const vector<int> even_numbers = CopyIfOrdered(numbers, [](int number) {
            return number % 2 == 0;
            });
        for (const int number : even_numbers) {
            cout << number << " "s;
        }
        cout << endl;
    }
    // выведет все чётные числа от 0 до 999

    mt19937 generator;

    const auto strings = GenerateDictionary<vector>(generator, 50'000, 3000);
    auto predicate = [](const string& s) {
        return count(s.begin(), s.end(), 'a') < 100;
    };

    TEST(CopyIfUnordered);
    TEST(CopyIfOrdered);
    return 0;
}
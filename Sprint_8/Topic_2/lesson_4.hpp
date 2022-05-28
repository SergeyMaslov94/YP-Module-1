#include <algorithm>
#include <execution>
#include <iostream>
#include <list>
#include <random>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <iterator>
#include <future>

#include "../Topic_1/log_duration.hpp"

using namespace std;

string GenerateWord(mt19937& generator, int max_length) {
    const int length = uniform_int_distribution(1, max_length)(generator);
    string word;
    word.reserve(length);
    uniform_int_distribution <> distr('a', 'z');
    for (int i = 0; i < length; ++i) {
        word.push_back(distr(generator));
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

struct Reverser {
    void operator()(string& value) const {
        reverse(value.begin(), value.end());
    }
};

template <typename Container, typename Function>
void Test(string_view mark, Container keys, Function function) {
    LOG_DURATION(mark);
    function(keys, Reverser{});
}

#define TEST(function) Test(#function, keys, function<remove_const_t<decltype(keys)>, Reverser>)

template <typename ForwardRange, typename Function>
void ForEachOld(ForwardRange& range, Function function) {
    // ускорьте эту реализацию
    for_each(execution::par, range.begin(), range.end(), function);
}

template <typename ForwardRange, typename Function>
void ForEach(ForwardRange& range, Function function) {
    // ускорьте эту реализацию

    /* вычислить количество элементов */
    size_t num_elements_ = range.size();
    size_t elements_processed = 0;

    /* вычислить размер каждой части, разделив
       количество элементов на количество задач */
    auto num_tasks = 2;
    int length_part = std::ceil(static_cast<float>(num_elements_) / num_tasks);
    auto num_part = std::floor(static_cast<float>(num_elements_) / length_part);
    std::vector<int> points;

    for (auto i = 1; i <= range.size(); i++) {
        if (!(i % length_part)) {
            points.push_back(i);
        }
    }

    auto tesk1 = std::async([left = range.begin(), right = next(range.begin(), points[0]), &function](){ for_each(execution::par, left, right, function); });
    auto tesk2 = std::async([left = next(range.begin(), points[0]), right = range.end(), &function](){ for_each(execution::par, left, right, function); });
}

int lesson_4() {
    // для итераторов с произвольным доступом тоже должно работать
    vector<string> strings = { "cat", "dog", "code", "tor", "bob", "lol", "fuck", "pain", "train", "rop", "hot"};

    ForEach(strings, [](string& s) {
        reverse(s.begin(), s.end());
        });

    for (string_view s : strings) {
        cout << s << " ";
    }
    cout << endl;
    // вывод: tac god edoc

    mt19937 generator;
    const auto keys = GenerateDictionary<list>(generator, 1000, 500);

    TEST(ForEachOld);
    TEST(ForEach);

    return 0;
}
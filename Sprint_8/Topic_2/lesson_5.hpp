#include <algorithm>
#include <execution>
#include <iostream>
#include <list>
#include <random>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <future>

#include <type_traits>
#include <iterator>

#include "../Topic_1/log_duration.hpp"

using namespace std;

string GenerateWord(mt19937& generator, int max_length) {
    const int length = uniform_int_distribution(1, max_length)(generator);
    string word;
    word.reserve(length);
    uniform_int_distribution<> distr('a', 'z');
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

template <typename ExecutionPolicy, typename ForwardRange, typename Function>
void ForEach(ExecutionPolicy policy, ForwardRange& range, Function function) {
    if constexpr (!std::is_same_v<typename iterator_traits<typename ForwardRange::iterator>::iterator_category, std::random_access_iterator_tag> &
        std::is_same_v<std::decay_t<ExecutionPolicy>, std::execution::parallel_policy>) {

        static constexpr int PART_COUNT = 4;
        const auto part_length = range.size() / PART_COUNT;
        auto part_begin = range.begin();
        auto part_end = next(part_begin, part_length);

        vector<future<void>> futures;
        for (int i = 0;
            i < PART_COUNT;
            ++i,
            part_begin = part_end,
            part_end = (i == PART_COUNT - 1
                ? range.end()
                : next(part_begin, part_length))
            ) {
            futures.push_back(async([function, part_begin, part_end] {
                for_each(part_begin, part_end, function);
                }));
        }
    }
    else {
        std::for_each(policy, range.begin(), range.end(), function);
    }
}

template <typename ForwardRange, typename Function>
void ForEach(ForwardRange& range, Function function) {
    ForEach(std::execution::seq, range, function);
}

template <typename Strings>
void PrintStrings(const Strings& strings) {
    for (string_view s : strings) {
        cout << s << " ";
    }
    cout << endl;
}

int lesson_5() {
    auto reverser = [](string& s) { reverse(s.begin(), s.end()); };

    list<string> strings_list = { "cat", "dog", "code" };

    ForEach(strings_list, reverser);
    PrintStrings(strings_list);
    // tac god edoc

    ForEach(execution::seq, strings_list, reverser);
    PrintStrings(strings_list);
    // cat dog code

    // единственный из вызовов, где должна работать ваша версия
    // из предыдущего задания
    ForEach(execution::par, strings_list, reverser);
    PrintStrings(strings_list);
    // tac god edoc

    vector<string> strings_vector = { "cat", "dog", "code" };

    ForEach(strings_vector, reverser);
    PrintStrings(strings_vector);
    // tac god edoc

    ForEach(execution::seq, strings_vector, reverser);
    PrintStrings(strings_vector);
    // cat dog code

    ForEach(execution::par, strings_vector, reverser);
    PrintStrings(strings_vector);
    // tac god edoc

    return 0;
}

/*
template <typename ForwardRange, typename Function>
void ForEach(ForwardRange& range, Function function) {
    static constexpr int PART_COUNT = 4;
    const auto part_length = range.size() / PART_COUNT;
    auto part_begin = range.begin();
    auto part_end = next(part_begin, part_length);

    vector<future<void>> futures;
    for (int i = 0;
         i < PART_COUNT;
         ++i,
             part_begin = part_end,
             part_end = (i == PART_COUNT - 1
                             ? range.end()
                             : next(part_begin, part_length))
         ) {
        futures.push_back(async([function, part_begin, part_end] {
            for_each(part_begin, part_end, function);
        }));
    }
}
*/


//template <typename ExecutionPolicy, typename ForwardRange, typename Function>
//void ForEach(ExecutionPolicy, ForwardRange& range, Function function) {
//    if constexpr (!std::is_same_v<typename iterator_traits<typename ForwardRange::iterator>::iterator_category, std::random_access_iterator_tag> &
//        std::is_same_v<std::decay_t<ExecutionPolicy>, std::execution::parallel_policy>) {
//
//        if (range.empty()) {
//            return;
//        }
//        /* вычислить количество элементов */
//        size_t num_elements_ = range.size();
//
//        /* вычислить размер каждой части, разделив
//           количество элементов на количество задач */
//        auto num_tasks = 2;
//        int length_part = std::ceil(static_cast<float>(num_elements_) / num_tasks);
//
//        std::vector<int> points;
//        for (size_t i = 1; i <= range.size(); i++) {
//            if (!(i % length_part)) {
//                points.push_back(i);
//            }
//        }
//
//        auto tesk1 = std::async([left = range.begin(), right = next(range.begin(), points[0]), &function](){ for_each(execution::par, left, right, function); });
//        auto tesk2 = std::async([left = next(range.begin(), points[0]), right = range.end(), &function](){ for_each(execution::par, left, right, function); });
//    }
//    else {
//        ForEachSeq(range, function);
//    }
//}
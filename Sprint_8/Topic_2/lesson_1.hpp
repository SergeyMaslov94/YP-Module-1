#include <algorithm>
#include <execution>
#include <iostream>

using namespace std;

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(const execution::parallel_policy&, RandomAccessIterator range_begin,
    RandomAccessIterator range_end, const Value& value) {
    auto left_bound = range_begin;
    auto right_bound = range_end;
    while (left_bound + 1 < right_bound) {
        const int distance = right_bound - left_bound;
        const int part_length = max(1, distance / 3);
        const auto middle_left = left_bound + part_length;
        const auto middle_right = right_bound - part_length;

        auto left_less_future = async([middle_left, &value] { return *middle_left < value; });

        if (*middle_right < value) {
            left_bound = middle_right;
        }
        else if (left_less_future.get()) {
            left_bound = middle_left;
            right_bound = middle_right;
        }
        else {
            right_bound = middle_left;
        }
    }
    if (left_bound == range_begin && !(*left_bound < value)) {
        return left_bound;
    }
    else {
        return right_bound;
    }
}

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(const execution::sequenced_policy&,
    RandomAccessIterator range_begin, RandomAccessIterator range_end,
    const Value& value) {
    return std::lower_bound(range_begin, range_end, value);
}

template <typename RandomAccessIterator, typename Value>
RandomAccessIterator LowerBound(RandomAccessIterator range_begin, RandomAccessIterator range_end,
    const Value& value) {
    return LowerBound(execution::seq, range_begin, range_end, value);
}

void tests() {
    vector<int> strings = { 2, 3, 3, 5, 8, 8};
    vector<int> requests = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto i = 0;
    while (i < requests.size()) {
        const auto value = requests[i];
        cout << "Request [" << requests[i] << "] position "
            << std::lower_bound(strings.begin(), strings.end(), requests[i]) - strings.begin() << " / "
            << LowerBound(std::execution::par, strings.cbegin(), strings.cend(), value) - strings.begin() << endl;
        i++;
    }
}

int lesson_1() {
    
    tests();


    const vector<string> strings = { "cat", "dog", "dog", "horse" };

    const vector<string> requests = { "bear", "cat", "deer", "dog", "dogs", "horses" };

    // последовательные версии
    cout << "Request [" << requests[0] << "] → position "
        << LowerBound(strings.begin(), strings.end(), requests[0]) - strings.begin() << endl;
    
    cout << "Request [" << requests[1] << "] → position "
        << LowerBound(execution::seq, strings.begin(), strings.end(), requests[1])
        - strings.begin()
        << endl;
    
    cout << "Request [" << requests[2] << "] → position "
        << LowerBound(execution::seq, strings.begin(), strings.end(), requests[2])
        - strings.begin()
        << endl;

    // параллельные
    cout << "Request [" << requests[3] << "] → position "
        << LowerBound(execution::par, strings.begin(), strings.end(), requests[3])
        - strings.begin()
        << endl;

    cout << "Request [" << requests[4] << "] → position "
        << LowerBound(execution::par, strings.begin(), strings.end(), requests[4])
        - strings.begin()
        << endl;

    cout << "Request [" << requests[5] << "] → position "
        << LowerBound(execution::par, strings.begin(), strings.end(), requests[5])
        - strings.begin()
        << endl;

    return 0;
}
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include <future>
#include <chrono>
#include <iostream>
#include <chrono>
#include <iostream>

using namespace std;

#define PROFILE_CONCAT_INTERNAL(X, Y) X##Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)
#define LOG_DURATION_STREAM(x, y) LogDuration UNIQUE_VAR_NAME_PROFILE(x, y)

class LogDuration {
public:
    // заменим имя типа std::chrono::steady_clock
    // с помощью using для удобства
    using Clock = std::chrono::steady_clock;

    LogDuration(const std::string_view id, std::ostream& dst_stream = std::cerr)
        : id_(id)
        , dst_stream_(dst_stream) {
    }

    ~LogDuration() {
        using namespace std::chrono;
        using namespace std::literals;

        const auto end_time = Clock::now();
        const auto dur = end_time - start_time_;
        dst_stream_ << id_ << ": "s << duration_cast<milliseconds>(dur).count() << " ms"s << std::endl;
    }

private:
    const std::string id_;
    const Clock::time_point start_time_ = Clock::now();
    std::ostream& dst_stream_;
};

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

// Ускорьте с помощью параллельности
template <typename RandomIt>
void MergeSortSlow(RandomIt range_begin, RandomIt range_end) {
    // 1. Если диапазон содержит меньше 2 элементов, выходим из функции
    int range_length = range_end - range_begin;
    if (range_length < 2) {
        return;
    }

    // 2. Создаём вектор, содержащий все элементы текущего диапазона
    vector elements(range_begin, range_end);
    // Тип элементов — typename iterator_traits<RandomIt>::value_type

    // 3. Разбиваем вектор на две равные части
    auto mid = elements.begin() + range_length / 2;

    // 4. Вызываем функцию MergeSort от каждой половины вектора
    MergeSortSlow(elements.begin(), mid);
    MergeSortSlow(mid, elements.end());

    // 5. С помощью алгоритма merge сливаем отсортированные половины
    // в исходный диапазон
    // merge -> http://ru.cppreference.com/w/cpp/algorithm/merge
    merge(elements.begin(), mid, mid, elements.end(), range_begin);
}

constexpr int MAX_ASYNC_DEPTH = 2;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end, int depth = 0) {
    const int range_length = range_end - range_begin;
    if (range_length < 2) {
        return;
    }

    vector elements(range_begin, range_end);

    const auto mid = elements.begin() + range_length / 2;

    auto left_task = [start = elements.begin(), mid, depth]{
        MergeSort(start, mid, depth + 1);
    };
    auto right_task = [mid, finish = elements.end(), depth]{
        MergeSort(mid, finish, depth + 1);
    };

    if (depth <= MAX_ASYNC_DEPTH) {
        auto left_future = async(left_task);
        right_task();
        left_future.get();
    }
    else {
        left_task();
        right_task();
    }

    merge(execution::par, elements.begin(), mid, mid, elements.end(), range_begin);
}

int main() {
    mt19937 generator;

    vector<int> test_vector(4000000);

    // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
    // Заполняет диапазон последовательно возрастающими значениями
    iota(test_vector.begin(), test_vector.end(), 1);

    // shuffle   -> https://ru.cppreference.com/w/cpp/algorithm/random_shuffle
    // Перемешивает элементы в случайном порядке
    shuffle(test_vector.begin(), test_vector.end(), generator);
    auto test_vector_2 = test_vector;

    // Сортируем вектор с помощью сортировки слиянием
    {
        LOG_DURATION("old algorithm");
        MergeSortSlow(test_vector.begin(), test_vector.end());
    }

    {
        LOG_DURATION("new algorithm");
        MergeSort(test_vector_2.begin(), test_vector_2.end());
    }

    return 0;
}
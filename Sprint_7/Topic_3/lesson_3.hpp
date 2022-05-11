#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>
#include <deque>

using namespace std;

template <typename RandomIt>
void MakeJosephusPermutationOld(RandomIt first, RandomIt last, uint32_t step_size) {
    vector<typename RandomIt::value_type> pool(first, last);
    size_t cur_pos = 0;
    while (!pool.empty()) {
        *(first++) = pool[cur_pos];
        pool.erase(pool.begin() + cur_pos);
        if (pool.empty()) {
            break;
        }
        cur_pos = (cur_pos + step_size - 1) % pool.size();
    }
}

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
    deque<typename RandomIt::value_type> pool;
    for (auto i = first; i < last; ++i) {
        pool.push_back(move(*i));
    }
    size_t cur_pos = 0;
    while (!pool.empty()) {
        *(first++) = move(pool[cur_pos]);
        pool.erase(pool.begin() + cur_pos);
        if (move(pool.empty())) {
            break;
        }
        cur_pos = move((cur_pos + step_size - 1) % move(pool.size()));
    }
}


vector<int> MakeTestVector() {
    vector<int> numbers(10);
    iota(begin(numbers), end(numbers), 0);
    return numbers;
}

void TestIntVectorOld() {
    const vector<int> numbers = MakeTestVector();
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutationOld(begin(numbers_copy), end(numbers_copy), 1);
        assert(numbers_copy == numbers);
    }
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutationOld(begin(numbers_copy), end(numbers_copy), 3);
        assert(numbers_copy == vector<int>({ 0, 3, 6, 9, 4, 8, 5, 2, 7, 1 }));
    }
}

void TestIntVector() {
    const vector<int> numbers = MakeTestVector();
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
        assert(numbers_copy == numbers);
    }
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
        assert(numbers_copy == vector<int>({ 0, 3, 6, 9, 4, 8, 5, 2, 7, 1 }));
    }
}

// ��� ����������� ���, ������� ������� ��� ���������, ��� ���� ����������
// ������� MakeJosephusPermutation �� ��������� ����������� ��������.
// ������ ��, ��������, �� ��������� ��� �� �������, ������ �� ���������
// �� ���� ����� � ����� �����

struct NoncopyableInt {
    int value;

    NoncopyableInt(const NoncopyableInt&) = delete;
    NoncopyableInt& operator=(const NoncopyableInt&) = delete;

    NoncopyableInt(NoncopyableInt&&) = default;
    NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator==(const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
    return lhs.value == rhs.value;
}

ostream& operator<<(ostream& os, const NoncopyableInt& v) {
    return os << v.value;
}

//void TestAvoidsCopying() {
//    vector<NoncopyableInt> numbers;
//    numbers.push_back({ 1 });
//    numbers.push_back({ 2 });
//    numbers.push_back({ 3 });
//    numbers.push_back({ 4 });
//    numbers.push_back({ 5 });
//
//    MakeJosephusPermutation(begin(numbers), end(numbers), 2);
//
//    vector<NoncopyableInt> expected;
//    expected.push_back({ 1 });
//    expected.push_back({ 3 });
//    expected.push_back({ 5 });
//    expected.push_back({ 4 });
//    expected.push_back({ 2 });
//
//    assert(numbers == expected);
//}

int lesson_3() {
    TestIntVectorOld();
    TestIntVector();
    //TestAvoidsCopying();
    return 0;
}
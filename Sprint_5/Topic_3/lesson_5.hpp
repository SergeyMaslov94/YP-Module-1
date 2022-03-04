#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include "../Topic_1/log_duration.h"

using namespace std;

// функция возвращает true, если векторы из одинаковых элементов
// перепишите эту функцию, улучшив её асимптотическую сложность
bool TestPermut_1(const vector<int>& v1, const vector<int>& v2) {
    // если они разной длины, элементы заведомо разные
    if (v1.size() != v2.size()) {
        return false;
    }

    for (const int& i : v1) {
        // проверяем, что каждый элемент первого вектора
        // содержится одинаковое количество раз в обоих векторах
        if (count(v1.begin(), v1.end(), i) != count(v2.begin(), v2.end(), i)) {
            return false;
        }
    }

    return true;
}

// функция возвращает true, если векторы из одинаковых элементов
// перепишите эту функцию, улучшив её асимптотическую сложность
bool TestPermut_2(const vector<int>& v1, const vector<int>& v2) {
    // если они разной длины, элементы заведомо разные
    if (v1.size() != v2.size()) {
        return false;
    }

    auto u1 = v1;
    auto u2 = v2;

    std::sort(u1.begin(), u1.end());
    std::sort(u2.begin(), u2.end());

    if (u1 != u2)
        return false;
    /*
    for (const int& i : v1) {
        // проверяем, что каждый элемент первого вектора
        // содержится одинаковое количество раз в обоих векторах
        if (count(v1.begin(), v1.end(), i) != count(v2.begin(), v2.end(), i)) {
            return false;
        }
    }
    */

    return true;
}


int lesson_5() {
    std::mt19937 g;

    int n = 100000;
    //cin >> n;
    vector<int> v1, v2;
    v1.reserve(n);
    v2.reserve(n);

    for (int i = 0; i < n; ++i) {
        v1.push_back(rand());
        v2.push_back(rand());
    }

    // оба вектора случайны, вряд ли они совпадут
    //cout << "Random vectors match? "s << flush;
    //cout << (TestPermut(v1, v2) ? "Yes"s : "No"s) << endl;

    // делаем один перестановкой другого явным образом
    v2 = v1;
    shuffle(v2.begin(), v2.end(), g);
    //{
    //    LOG_DURATION("TestPermut_1");
    //    cout << (TestPermut_1(v1, v2) ? "Yes"s : "No"s) << endl;
    //}

    {
        LOG_DURATION("TestPermut_2");
        cout << (TestPermut_2(v1, v2) ? "Yes"s : "No"s) << endl;
    }
    //cout << "Permuted vectors match? "s << flush;
    //cout << (TestPermut(v1, v2) ? "Yes"s : "No"s) << endl;

    return 0;
}
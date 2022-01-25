#pragma once
#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <ratio>
#include <vector>
#include <execution>

using namespace std;

const size_t testSize = 1'000'000;
const int iterationCount = 5;

int task()
{
    random_device rd;

    cout << "Testing with "s << testSize << " integers..."s << endl;
    vector<int> integers(testSize);
    for (auto& i : integers) {
        i = static_cast<int>(rd());
    }

    for (int i = 0; i < iterationCount; ++i)
    {
        vector<int> sorted(integers);
        const auto startTime = chrono::high_resolution_clock::now();
        sort(sorted.begin(), sorted.end());
        const auto endTime = chrono::high_resolution_clock::now();
        cout << "Sequential execution time: "s << chrono::duration_cast<chrono::duration<int, milli>>(endTime - startTime).count() << " ms."s << endl;
    }

    for (int i = 0; i < iterationCount; ++i)
    {
        vector<int> sorted(integers);
        const auto startTime = chrono::high_resolution_clock::now();
        sort(execution::par, sorted.begin(), sorted.end());
        const auto endTime = chrono::high_resolution_clock::now();
        cout << "Parallel execution time: "s << chrono::duration_cast<chrono::duration<int, milli>>(endTime - startTime).count() << " ms."s << endl;
    }

    return 0;
}
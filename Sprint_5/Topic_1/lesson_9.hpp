#include "log_duration.h"

#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using namespace std;

vector<float> ComputeAvgTemp1(const vector<vector<float>>& measures) {
    if (measures.empty())
        return {};

    std::vector<float> means;
    auto m = measures.size();
    auto n = measures[0].size();
    auto k = 0;
    float mean = 0;
    // место для вашего решения
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            auto val = measures[j][i];
            if (val > 0) {
                mean += val;
                k++;
            }
        }
        if (k > 0)
            means.push_back(mean / k);
        else
            means.push_back(0);

        mean = 0;
        k = 0;
    }
    return means;
}

vector<float> ComputeAvgTemp2(const vector<vector<float>>& measures) {
    if (measures.empty())
        return {};

    auto hours = measures[0].size();
    auto days = measures.size();

    std::vector<float> means(hours);
    std::vector<float> count(hours);
    
    auto k = 0;
    float mean = 0;
    // место для вашего решения
    for (auto i = 0, j = 0; i < days; i++) {
        for(const auto& val : measures[i]) {
            count[j] += (val > 0 ? 1 : 0);
            means[j] += (val > 0 ? val : 0);
            j++;
        }
        j = 0;
    }

    for (auto i = 0; i < hours; i++) {
        if (count[i] > 0)
            means[i] /= count[i];
        else
            means[i] = 0;
    }

    return means;
}

vector<float> GetRandomVector(int size) {
    static mt19937 engine;
    uniform_real_distribution<float> d(-100, 100);

    vector<float> res(size);
    for (int i = 0; i < size; ++i) {
        res[i] = d(engine);
    }

    return res;
}

int lesson_9() {
    vector<vector<float>> data;
    data.reserve(5000);

    for (int i = 0; i < 5000; ++i) {
        data.push_back(GetRandomVector(5000));
    }

    //vector<vector<float>> data = {
    //{0, -1, -1},
    //{1, -2, -2},
    //{2, 3, -3},
    //{3, 4, -4}
    //};


    {
        vector<float> avg;
        LOG_DURATION("ComputeAvgTemp1"s);
        avg = ComputeAvgTemp1(data);
    }

    {
        vector<float> avg;
        LOG_DURATION("ComputeAvgTemp2"s);
        avg = ComputeAvgTemp2(data);
    }

    //cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;

    return 0;
}
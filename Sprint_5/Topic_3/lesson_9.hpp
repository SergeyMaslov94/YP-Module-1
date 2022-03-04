#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int EffectiveCount(const vector<int>& v, int n, int i) {
    auto a = static_cast<double>(v.size()) / (n + 1) * (i + 1);

    if (a < std::log2(v.size())) {
        std::cout << "Using find_if" << std::endl;     
        return std::distance( v.begin(), find_if(v.begin(), v.end(), [i](int value) {return value > i; }));
    }
    
    std::cout << "Using upper_bound" << std::endl;
    return std::distance(v.begin(), std::upper_bound(v.begin(), v.end(), i));
}

int lesson_9() {

    static const int NUMBERS = 1'000'000;
    static const int MAX = 1'000'000'000;

    mt19937 r;
    uniform_int_distribution<int> uniform_dist(0, MAX);

    vector<int> nums;
    for (int i = 0; i < NUMBERS; ++i) {
        int random_number = uniform_dist(r);
        nums.push_back(random_number);
    }
    sort(nums.begin(), nums.end());

    int i;
    cin >> i;
    int result = EffectiveCount(nums, MAX, i);
    cout << "Total numbers before "s << i << ": "s << result << endl;
    
    return 0;
}
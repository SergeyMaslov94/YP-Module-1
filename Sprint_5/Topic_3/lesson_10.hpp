#include <algorithm>
#include <random>
#include <string>
#include <vector>

using namespace std;

string RandString(int len) {
    static mt19937 engine;

    // строка длины len из символов 'A'
    string s(len, 'A');
    for (int i = 0; i < len / 2; ++i) {
        uniform_int_distribution<int> dist(0, i - 1);
        s[dist(engine)] = 'B';
    }

    return s;
}

void F(int n, int m) {
    vector<int> v1;
    vector<string> v2;

    for (int i = 0; i < m; ++i) {
        v1.push_back(rand() % 100);
    }

    for (int i = 0; i < n; ++i) {
        v2.push_back(RandString(i));
    }

    sort(v2.begin(), v2.end());

    for (int r : v1) {
        for (int j = 0; j < r * 2; ++j) {
            string s = RandString(n);
            upper_bound(v2.begin(), v2.end(), s);
        }
    }
}

int lesson_10()
{
    F(10, 30);
    return 0;
}
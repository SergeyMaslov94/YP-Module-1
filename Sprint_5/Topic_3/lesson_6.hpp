#include <iostream>

using namespace std;

template <typename F>
int FindFloor_1(int n, F drop) {
    // Переделайте этот алгоритм, имеющий линейную сложность.
    // В итоге должен получится логарифмический алгоритм.
    for (int i = 1; i < n; ++i) {
        if (drop(i)) {
            return i;
        }
    }

    return n;
}

template <typename F>
int FindFloor_2(int n, F drop) {
   
    int a = 1;
    int b = n;
    int i = (a + b) / 2;

    while(a != b)
    {
        if (drop(i))
        {
            b = i;
        }
        else
        {
            a = i + 1;
        }

        i = (a + b) / 2;
    }
    
    return a;
}

int lesson_6() {
    int n, t;
    cout << "Enter n and target floor number: "s << endl;
    cin >> n >> t;

    int count = 0;
    int found = FindFloor_2(n, [t, &count](int f) {
        ++count;
        return f >= t;
        });

    cout << "Found floor "s << found << " after "s << count << " drops"s;

    return 0;
}
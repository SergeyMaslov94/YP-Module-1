#include <cstdint>
#include <iostream>

using namespace std;

// упростите эту экспоненциальную функцию,
// реализовав линейный алгоритм
int64_t T(int i) 
{
    if (i <= 1) {
        return 0;
    }
    if (i == 2) {
        return 1;
    }

    int64_t prev3 = 0;
    int64_t prev2 = 0;
    int64_t prev1 = 1;    

    for (int count = 3; count <= i; count++)
    {
        int64_t current = prev1 + prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return prev1;
}

int lesson_7() {
    int i;

    while (true) {
        cout << "Enter index: "s;
        if (!(cin >> i)) {
            break;
        }

        cout << "Ti = "s << T(i) << endl;
    }

    return 0;
}
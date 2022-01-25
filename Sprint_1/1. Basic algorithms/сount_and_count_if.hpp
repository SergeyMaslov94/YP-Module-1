#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void PrintWorryCount(const vector<bool>& is_nervous) {
    // подсчитываем количество элементов в векторе is_nervous, равных true
    auto worry_count = count(is_nervous.begin(), is_nervous.end(), true);
    cout << worry_count << endl;
}

bool Search(const string s)
{
    if (!s.empty())
    {
        if (s[0] == 'A' || s[0] == 'a')
            return true;
    }

    return false;
}

int CountStartsWithA(const vector<string>& xs) {

    // посчитайте число строк, начинающихс¤ на букву A
    int cnt = count_if(xs.begin(), xs.end(), Search);
    return cnt;
}

//==============================================================

int task_1() {
    // не мен¤йте содержимое функции main
    PrintWorryCount({true, true, false, true});
    return 0;
}

int task_2() {
    // не мен¤йте тело main
    cout << CountStartsWithA({ "And"s, "another"s, "one"s, "gone"s, "another"s, "one"s "bites"s, "the"s, "dust"s });
    return 0;
}
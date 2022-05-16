#include <functional>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

struct DailyStat {
    int day_id;
    int cat_count;
};

int lesson_4() {
    const vector<DailyStat> daily_stats = {
        {15, 1},  // 1 кот 15-го числа
        {16, 7},  // 7 котов 16-го числа
        {17, 2},
        {18, 6},
        {19, 9}
    };

    vector<int> cat_count_cumulative(daily_stats.size());

    /*
        Как inclusive_scan обобщает reduce, так transform_inclusive_scan 
        обобщает transform_reduce и позволяет применить преобразование 
        ко всем элементам перед вычислением сумм. Например, 
        transform-операция может помочь достать нужное 
        поле из структуры:
    */

    //===================================================
    transform_inclusive_scan(
        daily_stats.begin(), daily_stats.end(),
        cat_count_cumulative.begin(),
        plus<>{},
        [](const DailyStat& stat) { return stat.cat_count; }
    );
    cout << "Количество котов на конец каждого дня: ";
    for (const int count : cat_count_cumulative) {
        cout << count << " ";
    }
    cout << endl;

    //===================================================
    transform_exclusive_scan(
        daily_stats.begin(), daily_stats.end(),
        cat_count_cumulative.begin(),
        0,
        plus<>{},
        [](const DailyStat& stat) { return stat.cat_count; }
    );
    cout << "Количество котов на начало каждого дня: ";
    for (const int count : cat_count_cumulative) {
        cout << count << " ";
    }
    cout << endl;

    return 0;
}
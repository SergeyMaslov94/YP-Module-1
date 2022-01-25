#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

int task_1() {
    int count;
    cin >> count;

    vector<pair<int, string>> base;

    for (int i = 0; i < count; ++i) {
        string name;
        int age;
        cin >> name >> age;
        // сохраните в вектор пар
        base.push_back(pair<int, string>(age, name));
    }

    sort(base.begin(), base.end());
    reverse(base.begin(), base.end());

    for (auto& n : base)
    {
        cout << n.second << endl;
    }

    // выведите только имена в порядке убывания возраста
    // Jack
    // John
    // Oliver
    // Harry

    return 0;
}

pair<bool, double> CalcMedian(vector<double> samples) {
    // верните {true, медиана}, если она существует,
    // то есть вектор непустой,
    // иначе - {false, 0}
    // обратите внимание - вектор принимаем по значению,
    // так как его придётся немного подпортить, чтобы вернуть ответ

    if (samples.empty())
        return { false,0 };

    auto m = samples.begin() + samples.size() / 2;
    nth_element(samples.begin(), m, samples.end());

    auto size = samples.size();

    if (size % 2 != 0)
    {
        auto value = samples[size / 2];
        return{ true, value };
    }
    else
    {
        auto value = (samples[size / 2 - 1] + samples[size / 2]) / 2;
        return{ true, value };
    }
}

int task_2() {

    int size;
    cin >> size;

    vector<double> samples;
   
    for (int i = 0; i < size; ++i) {
        double sample;
        cin >> sample;
        samples.push_back(sample);
    }
    
    pair<bool, double> result = CalcMedian(samples);

    if (result.first) {
        cout << result.second << endl;
    }
    else {
        cout << "Empty vector"s << endl;
    }

    return 0;
}
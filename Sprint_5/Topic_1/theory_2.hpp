#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;
// хотите немного магии? тогда используйте namespace literals
using namespace literals;

int theory_2() {
    cout << "Ожидание 5s..."s << endl;
    const auto start_time = steady_clock::now();

    // операция - ожидание 5 секунд
    this_thread::sleep_for(5s);
    const auto end_time = steady_clock::now();

    const auto dur = end_time - start_time;
    cerr << "Продолжительность сна: "s << chrono::duration_cast<chrono::milliseconds>(dur).count() << " ms"s << endl;

    cout << "Ожидание завершено"s << endl;

    return 0;
}
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using namespace chrono;
// ������ ������� �����? ����� ����������� namespace literals
using namespace literals;

int theory_2() {
    cout << "�������� 5s..."s << endl;
    const auto start_time = steady_clock::now();

    // �������� - �������� 5 ������
    this_thread::sleep_for(5s);
    const auto end_time = steady_clock::now();

    const auto dur = end_time - start_time;
    cerr << "����������������� ���: "s << chrono::duration_cast<chrono::milliseconds>(dur).count() << " ms"s << endl;

    cout << "�������� ���������"s << endl;

    return 0;
}
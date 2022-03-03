#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;
using namespace chrono;

vector<int> ReverseVector(const vector<int>& source_vector) {
    vector<int> res;
    for (int i : source_vector) {
        res.insert(res.begin(), i);
    }

    return res;
}

int CountPops(const vector<int>& source_vector, int begin, int end) {
    int res = 0;

    for (int i = begin; i < end; ++i) {
        if (source_vector[i]) {
            ++res;
        }
    }

    return res;
}

void AppendRandom(vector<int>& v, int n) {
    for (int i = 0; i < n; ++i) {
        // �������� ��������� ����� � ������� ������� rand.
        // � ������� (rand() % 2) ������� ����� ����� � ��������� 0..1.
        // � C++ ������� ����� ����������� ���������� ��������� �����,
        // �� � ������ ����� �� ����� �� ��������
        v.push_back(rand() % 2);
    }
}

void Operate() {
    vector<int> random_bits;

    // �������� << ��� ����� ����� ��� ����� ���� ��� � ��������
    // ������ �����. ������� � � ������� ����� 2 � ������� 17 (131072)
    static const int N = 1 << 17;

    // �������� ������ ���������� ������� 0 � 1
    
    auto start_time = chrono::steady_clock::now();
    AppendRandom(random_bits, N);
    auto end_time = chrono::steady_clock::now();
    auto dur = end_time - start_time;
    cerr << "Append random: " << chrono::duration_cast<chrono::milliseconds>(dur).count() << " ms" << endl;
    
    // ��������� ������ ����� ������
   
    start_time = chrono::steady_clock::now();
    vector<int> reversed_bits = ReverseVector(random_bits);
    end_time = chrono::steady_clock::now();
    dur = end_time - start_time;
    cerr << "Reverse: " << chrono::duration_cast<chrono::milliseconds>(dur).count() << " ms" << endl;
    
    // ��������� ������� ������ �� ��������� �������� �������
    start_time = chrono::steady_clock::now();
    for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
        // ����� ��������� ��������, �� �������� �� ������� 100. ���� double;
        // ������������� �������� ������� CountPops ��� ���� �������������
        // ������������� � double, ��� � i
        double rate = CountPops(reversed_bits, 0, i) * 100. / i;
        cout << "After "s << i << " bits we found "s << rate << "% pops"s
            << endl;
    }
    end_time = chrono::steady_clock::now();
    dur = end_time - start_time;
    cerr << "Counting: " << chrono::duration_cast<chrono::milliseconds>(dur).count() << " ms" << endl;
}

int lesson_3() {
    Operate();
    return 0;
}
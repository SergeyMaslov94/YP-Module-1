// ��������� cstdlib ����������� ��� ������� rand,
// �������� ��������� �����.
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

vector<int> ReverseVector(const vector<int>& source_vector) {
    vector<int> res;
    for (int i : source_vector) {
        res.insert(res.begin(), i);
    }

    return res;
}

// ������� ������� ���������� ��������� ����� � �������
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
        // ������������ (rand() % 2) ������� ����� ����� � ��������� 0..1.
        // � C++ ������� ����� ����������� ���������� ��������� �����,
        // �� � ������ ����� �� ����� �� ��������.
        v.push_back(rand() % 2);
    }
}

int theory_1() {
    vector<int> random_bits;

    // �������� << ��� ����� ����� ��� ����� ���� ��� � ��������
    // ������ �����. ������� � � ������� ����� 2 � ������� 17 (131072)
    static const int N = 1 << 17;

    // �������� ������ ���������� ������� 0 � 1.
    AppendRandom(random_bits, N);

    // ��������� ������ ����� ������.
    vector<int> reversed_bits = ReverseVector(random_bits);

    // ��������� ������� ������ �� ��������� �������� �������.
    for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
        // ����� ��������� �������� �� �������� �� ������� 100. ���� double.
        // ������������� �������� ������� CountPops ��� ���� �������������
        // ������������� � double, ��� � i.
        double rate = CountPops(reversed_bits, 0, i) * 100. / i;
        cout << "After "s << i << " bits we found "s << rate << "% pops"s
            << endl;
    }

    return 0;
}
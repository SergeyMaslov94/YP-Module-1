#include "../Topic_1/log_duration.h"
#include <iostream>
#include <vector>

using namespace std;

class StreamUntier {
public:
    // �������� �����������, ����������
    // � �������������� ���� ������ ��� �������������
    StreamUntier(std::istream& stream) :
        m_cin(stream),
        tied_before_(stream.tie(nullptr)) // ���������� ����� ����� �� ������ ������
    {}

    ~StreamUntier() {
        m_cin.tie(tied_before_); // ����������� �����
    }

    std::istream& m_cin;
private:
    std::ostream* tied_before_ = nullptr;
};

int lesson_3() {
    {
        LOG_DURATION("\\n with tie"s);

        StreamUntier guard(cin);
        int i;
        while (cin >> i) {
            cout << i * i << "\n"s;
        }
    }

    return 0;
}
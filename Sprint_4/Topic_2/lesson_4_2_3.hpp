// ====================================================
// ���� 4. ������� 2.
// ����� � ��������������� �������, ������� � ���������
#include <iostream>
#include <set>
#include <algorithm>
#include <vector>

using namespace std;
/*
//===================================================================================================================
������� 2
�������� ������� FindStartsWith, �������:
-��������� ��������������� ����� ����� � ���� ���������� range_begin, range_end � ���� ������ prefix;
-���������� ������������ [range_begin, range_end) ����� � ���� ���� ����������. 
��� ������ ���������� � ������� prefix.

���� �������� �������� ����, ��� ������� ������ ��������� �� ����� � ����������, ���� ����� ��� ��������� ������� 
���������� �������� ����� ������, ������������ � ������� prefix � ������� ��������� equal_range. 
�������������, ��� ������ ������� ���� �� �������� ��������� ����, � ������ prefix � �������� ��������� �����.
*/

// ������� - ������
template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, char prefix) {
    
    string str1(1, prefix);
    // ��� ������, ������������ � '<prefix>', ������ ��� ����� ������ "<prefix>"
    auto left = lower_bound(range_begin, range_end, str1);

    // �������� ��������� � �������� ������.
    // �� �������, ���� prefix = 'z':
    //� ���� ������ �� ������� ��������� �� 'z' ������ � ������� ��������
    char next_prefix = static_cast<char>(prefix + 1);

    // ������ "<next_prefix>" � ������ ��������� �����
    // �������� ������ ������� ������
    // ��������� �����, ����������� � '<prefix>'
    string str2(1, next_prefix);
    auto right = lower_bound(range_begin, range_end, str2);

    return { left, right };
}

//===================================================================================================================
// ������� 3
// ������� ���������� ������.������ ������� ����� �� ����� �������� ���� char, � ������� � ����� ����������� ��������.
// ������� - ������
template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, string prefix) {

    auto left = lower_bound(range_begin, range_end, prefix);

    string next_prefix(prefix.begin(), prefix.end());
    auto& symbol = next_prefix.back();
    symbol = static_cast<char>(symbol + 1);

    auto right = lower_bound(range_begin, range_end, next_prefix);

    return { left, right };
}

int top_2_lesson_4_2_3() {
//===================================================================================
// ������� 2
    const vector<string> sorted_strings_1 = { "moscow", "murmansk", "vologda" };

    const auto m_result = FindStartsWith(begin(sorted_strings_1), end(sorted_strings_1), 'm');
    for (auto it = m_result.first; it != m_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    const auto p_result = FindStartsWith(begin(sorted_strings_1), end(sorted_strings_1), 'p');
    cout << (p_result.first - begin(sorted_strings_1)) << " " << (p_result.second - begin(sorted_strings_1)) << endl;

    const auto z_result = FindStartsWith(begin(sorted_strings_1), end(sorted_strings_1), 'z');
    cout << (z_result.first - begin(sorted_strings_1)) << " " << (z_result.second - begin(sorted_strings_1)) << endl;

//===================================================================================
// ������� 3
    const vector<string> sorted_strings_2 = { "moscow", "motovilikha", "murmansk" };

    const auto mo_result = FindStartsWith(begin(sorted_strings_2), end(sorted_strings_2), "mo");
    for (auto it = mo_result.first; it != mo_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    const auto mt_result = FindStartsWith(begin(sorted_strings_2), end(sorted_strings_2), "mt");
    cout << (mt_result.first - begin(sorted_strings_2)) << " " << (mt_result.second - begin(sorted_strings_2)) << endl;

    const auto na_result = FindStartsWith(begin(sorted_strings_2), end(sorted_strings_2), "na");
    cout << (na_result.first - begin(sorted_strings_2)) << " " << (na_result.second - begin(sorted_strings_2)) << endl;

    return 0;
}
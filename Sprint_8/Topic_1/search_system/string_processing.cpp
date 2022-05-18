#include "string_processing.h"
#include <iostream>
#include <algorithm>

bool NegativeWordErr(const std::string_view& word) {
    // ���� ����� ������� '�����' ����������� �����
    if (word.size() == 1) {
        if (word[0] == '-') {
            return false;
        }
    }
    // ���� � ������� ����� ������ ������ ����� �����-�������
    if (word.size() > 1) {
        if (word[0] == '-' && word[1] == '-') {
            return false;
        }
    }
    return true;
}

bool IsValidWord(const std::string_view& word) {
    // A valid word must not contain special characters
    return std::none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
        });
}

std::vector<std::string> SplitIntoWords(const std::string& text) {
    std::vector<std::string> words;
    std::string word;

    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }

    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}

// ����� ������ ���� ������� ���������� ������ ��������� string_view
std::vector<std::string_view> SplitIntoWordsView(std::string_view str) {
    /*// 1
    ������� ������ �� str �� ������� ������������� �������, ���������������� ������� remove_prefix.
    �� ����� �� string_view ��������� ���������� ��������.*/
    int64_t pos = str.find_first_not_of(" ");

    const int64_t pos_end = str.npos;
    if (pos == pos_end)
        return {};

    str.remove_prefix(pos);
    /*// 2
    ����� ���������� �����, ����� ������ ��������� �� �����. � string � string_view ����� ����
    ��������� npos � ����������� ��������� ������. ������ ��� ������ ������� �����, �������
    ���� �� �����-������ ������ ��������� �������� �������� � ������.

    ���������� ����. ������� �� ����, ���� �� ������� ����� ������������ ������.
    */
    std::vector<std::string_view> result;
    while (!str.empty()) {
        /*// 4
        � ����� ����������� ����� find � ����� ����������, ����� ����� ����� ������� ������� �������.
        */
        int64_t space = str.find(' ');
        /*// 5
        �������� � �������������� ������ ������� string_view, ���������� ������� ������ substr,
        ��� ��������� ������� ����� 0, � �������� � ��������� ������� ������� ��� npos.
        */
        result.push_back(str.substr(0, space));

        // 6
        // ������������ ���� ����� ����
        if (!NegativeWordErr(result.back())) {
            throw std::invalid_argument("������������ ���� ����� ����!");
        }
        // �������� �� ������� ���� �������� � �������
        if (!IsValidWord(result.back())) {
            throw std::invalid_argument("�������� �������� ������������ �������!");
        }

        /*// 7
        �������� ������ str ���, ����� ��� ��������� �� ������� �� ��������. ��� ����� �������
        ������� remove_prefix, ���������� ������ str �� ��������� � ��������� ���������� �������.
        */
        str.remove_prefix(std::min(str.size(), static_cast<size_t>(space)));
        pos = str.find_first_not_of(" ");
        str.remove_prefix(std::min(str.size(), static_cast<size_t>(pos)));
    }
    return result;
}
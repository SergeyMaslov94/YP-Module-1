#pragma once
//========================================================
// Сопутствующие функции
//========================================================
#include <string>
#include <vector>
#include <set>

std::string ReadLine();

int ReadLineWithNumber();

// Проверка корректного ввода минус слов
bool NegativeWordErr(const std::string& word);

std::vector<std::string> SplitIntoWords(const std::string& text);

template <typename StringContainer>
std::set<std::string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    set<string> non_empty_strings;
    for (const string& str : strings) {
        if (!str.empty()) {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}
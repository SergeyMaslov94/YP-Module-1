#pragma once
//========================================================
// Сопутствующие функции
//========================================================
#include <string>
#include <vector>
#include <set>

// Разделяем строку на отдельные слова
std::vector<std::string> SplitIntoWords(const std::string& text);
std::vector<std::string_view> SplitIntoWordsView(std::string_view str);

// Определяем не пустые строки
template <typename StringContainer>
std::set<std::string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    std::set<std::string> non_empty_strings;
    for (const std::string& str : strings) {
        if (!str.empty()) {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}
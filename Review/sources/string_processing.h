#pragma once
//========================================================
// Сопутствующие функции
//========================================================
#include <string>
#include <vector>
#include <set>

// Читаем строку
std::string ReadLine();

// Читаем цифру
int ReadLineWithNumber();

// Проверка корректного ввода минус слов
bool NegativeWordErr(const std::string& word);

// Разделяем строку на отдельные слова
std::vector<std::string> SplitIntoWords(const std::string& text);

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
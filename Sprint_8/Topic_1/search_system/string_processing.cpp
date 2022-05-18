#include "string_processing.h"
#include <iostream>
#include <algorithm>

bool NegativeWordErr(const std::string_view& word) {
    // если после символа 'минус' отсутствует текст
    if (word.size() == 1) {
        if (word[0] == '-') {
            return false;
        }
    }
    // если в запросе более одного минуса перед минус-словами
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

// пусть теперь наша функция возвращает вектор элементов string_view
std::vector<std::string_view> SplitIntoWordsView(std::string_view str) {
    /*// 1
    Удалите начало из str до первого непробельного символа, воспользовавшись методом remove_prefix.
    Он уберёт из string_view указанное количество символов.*/
    int64_t pos = str.find_first_not_of(" ");

    const int64_t pos_end = str.npos;
    if (pos == pos_end)
        return {};

    str.remove_prefix(pos);
    /*// 2
    Чтобы остановить поиск, нужен аналог итератора на конец. У string и string_view такую роль
    выполняет npos — специальная константа класса. Внутри это просто большое число, которое
    вряд ли когда-нибудь сможет оказаться реальной позицией в строке.

    Используем цикл. Выходим из него, если не удалось найти непробельный символ.
    */
    std::vector<std::string_view> result;
    while (!str.empty()) {
        /*// 4
        В цикле используйте метод find с одним параметром, чтобы найти номер позиции первого пробела.
        */
        int64_t space = str.find(' ');
        /*// 5
        Добавьте в результирующий вектор элемент string_view, полученный вызовом метода substr,
        где начальная позиция будет 0, а конечная — найденная позиция пробела или npos.
        */
        result.push_back(str.substr(0, space));

        // 6
        // некорректный ввод минус слов
        if (!NegativeWordErr(result.back())) {
            throw std::invalid_argument("некорректный ввод минус слов!");
        }
        // Проверка на наличие спец символов в запросе
        if (!IsValidWord(result.back())) {
            throw std::invalid_argument("документ содержит недопустимые символы!");
        }

        /*// 7
        Сдвиньте начало str так, чтобы оно указывало на позицию за пробелом. Это можно сделать
        методом remove_prefix, передвигая начало str на указанное в аргументе количество позиций.
        */
        str.remove_prefix(std::min(str.size(), static_cast<size_t>(space)));
        pos = str.find_first_not_of(" ");
        str.remove_prefix(std::min(str.size(), static_cast<size_t>(pos)));
    }
    return result;
}
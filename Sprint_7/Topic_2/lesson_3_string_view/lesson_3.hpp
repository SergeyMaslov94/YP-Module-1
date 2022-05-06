#include <string>
#include <string_view>
#include <cmath>

using namespace std;
// старая реализация функции разделения строки на слова
vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                // обратите внимание на это место в коде
                // здесь мы каждый раз копируем найденное слово
                // в новый элемент в векторе
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
vector<string_view> SplitIntoWordsViewOld(string_view str) {
    vector<string_view> result;
    /*// 1
    Создаём переменную, где будем сохранять начальную позицию для поиска следующего пробела.
    Иначе говоря, начало слова. Устанавливаем её на позицию первого непробельного символа.*/
    int64_t pos = str.find_first_not_of(" ");
    /*// 2
    Чтобы остановить поиск, нужен аналог итератора на конец. У string и string_view такую роль 
    выполняет npos — специальная константа класса. Внутри это просто большое число, которое 
    вряд ли когда-нибудь сможет оказаться реальной позицией в строке.
    */
    const int64_t pos_end = str.npos;
    /*// 3
    Используем цикл. Выходим из него, если не удалось найти непробельный символ.
    */
    while (pos != pos_end) {
        /*// 4
        В цикле ищем следующий пробел, вызывая метод find, который вернёт 
        позицию ближайшего пробела, или npos, если пробел не найден.
        */
        int64_t space = str.find(' ', pos);
        /*// 5
        Метод substr для string_view возвращает string_view, то есть новая строка не создаётся.
        Просто продолжаем с указателями на уже имеющуюся строку. Если пробел не найден, добавляем
        в вектор всё, начиная с pos. Если найден, выделяем слово от pos длиной space - pos.
        */
        result.push_back(space == pos_end ? str.substr(pos) : str.substr(pos, space - pos));
        /*// 6
        Достигнув конца строки, выходим из цикла. Иначе, пропускаем одну позицию
        — пробел — и начинаем следующую итерацию цикла.
        */
        pos = str.find_first_not_of(" ", space);
    }
    return result;
}

// пусть теперь наша функция возвращает вектор элементов string_view
vector<string_view> SplitIntoWordsView(string_view str) {
    vector<string_view> result;
    /*// 1
    Удалите начало из str до первого непробельного символа, воспользовавшись методом remove_prefix. 
    Он уберёт из string_view указанное количество символов.*/
    int64_t pos = str.find_first_not_of(" ");

    const int64_t pos_end = str.npos;
    if (pos == pos_end)
        return result;

    str.remove_prefix(pos);
    /*// 2
    Чтобы остановить поиск, нужен аналог итератора на конец. У string и string_view такую роль
    выполняет npos — специальная константа класса. Внутри это просто большое число, которое
    вряд ли когда-нибудь сможет оказаться реальной позицией в строке.

    Используем цикл. Выходим из него, если не удалось найти непробельный символ.
    */
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
        /*// 6
        Сдвиньте начало str так, чтобы оно указывало на позицию за пробелом. Это можно сделать
        методом remove_prefix, передвигая начало str на указанное в аргументе количество позиций.
        */
        str.remove_prefix(std::min(str.size(), static_cast<size_t>(space)));
        pos = str.find_first_not_of(" ");
        str.remove_prefix(std::min(str.size(), static_cast<size_t>(pos)));
    }
    return result;
}

int lesson_3() {
    const string text = "";
    //{
    //    LOG_DURATION("string");
    //    const auto words = SplitIntoWords(text);
    //    cout << words[0] << "\n";
    //}
    //{
    //    LOG_DURATION("string_view_old");
    //    const auto words = SplitIntoWordsViewOld(text);
    //    cout << words[0] << "\n";
    //}
    //{
    //    LOG_DURATION("string_view");
    //    const auto words = SplitIntoWordsView(text);
    //    cout << words[0] << "\n";
    //}

    assert((SplitIntoWordsView("") == vector<string_view>{}));
    assert((SplitIntoWordsView("     ") == vector<string_view>{}));
    assert((SplitIntoWordsView("aaaaaaa") == vector{ "aaaaaaa"sv }));
    assert((SplitIntoWordsView("a") == vector{ "a"sv }));
    assert((SplitIntoWordsView("a b c") == vector{ "a"sv, "b"sv, "c"sv }));
    assert((SplitIntoWordsView("a    bbb   cc") == vector{ "a"sv, "bbb"sv, "cc"sv }));
    assert((SplitIntoWordsView("  a    bbb   cc") == vector{ "a"sv, "bbb"sv, "cc"sv }));
    assert((SplitIntoWordsView("a    bbb   cc   ") == vector{ "a"sv, "bbb"sv, "cc"sv }));
    assert((SplitIntoWordsView("  a    bbb   cc   ") == vector{ "a"sv, "bbb"sv, "cc"sv }));
    cout << "All OK" << endl;

	return 0;
}
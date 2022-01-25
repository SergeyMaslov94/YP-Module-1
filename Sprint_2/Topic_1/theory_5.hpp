/* Юнит тестирование класса */

#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <map>
#include <set>

using namespace std;

// Класс обработки синонимов
class Synonyms {
public:
    void Add(const string& first_word, const string& second_word) {
        synonyms_[first_word].insert(second_word);
        synonyms_[second_word].insert(first_word);
    }

    size_t GetSynonymCount(const string& word) const {
        if (synonyms_.count(word) == 0)
            return 0;
        
        return synonyms_.at(word).size();
    }

    bool AreSynonyms(const string& first_word, const string& second_word) const {
        
        if (synonyms_.count(first_word) == 0 || synonyms_.count(second_word) == 0)
        {
            return false;
        }

        if (synonyms_.at(first_word).count(second_word) == 0)
        {
            return false;
        }

        return true;
    }

private:
    map<string, set<string>> synonyms_;
};

// Тест на увеличение количества синонимов при их добавлении
void TestAddingSynonymsIncrreasesTheirCount()
{
    Synonyms synonyms;
    assert(synonyms.GetSynonymCount("music"s) == 0);
    assert(synonyms.GetSynonymCount("melody"s) == 0);

    synonyms.Add("music"s, "melody"s);
    // Два слова являются синонимами друг друга
    assert(synonyms.GetSynonymCount("music"s) == 1);
    assert(synonyms.GetSynonymCount("melody"s) == 1);

    synonyms.Add("music"s, "tune"s);
    assert(synonyms.GetSynonymCount(("music"s)) == 2);

    // Отношение синонимичности в нашей постановке задачи не является транзитивным.
    // Поэтому слова "tune" и "melody" синонимами друг друга не являются,
    // несмотря на то что оба они являются синонимами слова "music".
    assert(synonyms.GetSynonymCount("tune"s) == 1);
    assert(synonyms.GetSynonymCount("melody"s) == 1);
}

// Тест на определение, являются ли слова синонимами
void TestAreSynonyms()
{
    Synonyms synonyms;

    synonyms.Add("kit"s, "kot"s);
    assert(synonyms.AreSynonyms("kit"s, "kot"s) == 1);

    synonyms.Add("kit"s, "kat"s);
    assert(synonyms.AreSynonyms("kat"s, "kot"s) == 0);
}

// Юнит-тесты
void TestSynonyms()
{
    TestAddingSynonymsIncrreasesTheirCount();
    TestAreSynonyms();
}

int theory_5() {

    Synonyms synonyms;

    string line;

    /* Написание теста перед написанием кода называется «‎разработка через тестирование»‎
    (test-driven development, TDD). Этот подход заключается в следующем: */
    TestSynonyms();

    // В случае ошибок чтения из cin произойдёт выход из цикла
    while (getline(cin, line)) {
      
        /* Для этого подключим библиотеку <sstream>. Классы этой библиотеки позволяют считывать данные
           из строк и выводить данные в строки. Класс istringstream — это строковый поток ввода, 
           а класс ostringstream — строковый поток вывода. В коде ниже стандартный класс 
           istringstream упрощает разбор строки */        

        istringstream command(line);
        string action;
        command >> action;

        if (action == "ADD"s) {
            string first_word, second_word;
            command >> first_word >> second_word;
            synonyms.Add(first_word, second_word);
        }
        else if (action == "COUNT"s) {
            string word;
            command >> word;
            cout << synonyms.GetSynonymCount(word) << endl;
        }
        else if (action == "CHECK"s) {
            string first_word, second_word;
            command >> first_word >> second_word;

            if (synonyms.AreSynonyms(first_word, second_word)) {
                cout << "YES"s << endl;
            }
            else {
                cout << "NO"s << endl;
            }
        }
        else if (action == "EXIT"s) {
            break;
        }
    }

    return 0;
}
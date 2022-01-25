/* 
Замените встроенный макрос assert на макросы ASSERT_EQUAL, ASSERT_EQUAL_HINT, ASSERT 
и ASSERT_HINT из вашего фреймворка. Вам пригодится решение задания из урока 
«Юнит-тестирование класса». Используйте его в качестве заготовки кода.
*/

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

using namespace std;

//=======================================================================
// Фреймворк на "трушность"
void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line,
    const string& hint) 
{
    if (!value) {
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))
//=======================================================================

//---------------------------------------------------------------
// Фреймворк на равенство значений
template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
    const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cout << boolalpha;
        cout << file << "("s << line << "): "s << func << ": "s;
        cout << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cout << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

//=======================================================================
class Synonyms {
public:
    void Add(const string& first_word, const string& second_word) {
        synonyms_[first_word].insert(second_word);
        synonyms_[second_word].insert(first_word);
    }

    size_t GetSynonymCount(const string& word) const {
        if (synonyms_.count(word) != 0) {
            return synonyms_.at(word).size();
        }
        return 0;
    }

    bool AreSynonyms(const string& first_word, const string& second_word) const {
        if (synonyms_.count(first_word) != 0) {
            return synonyms_.at(first_word).count(second_word) != 0;
        }
        return false;
    }

private:
    map<string, set<string>> synonyms_;
};

void TestAddingSynonymsIncreasesTheirCount() 
{
    Synonyms synonyms;
    
    ASSERT_EQUAL(synonyms.GetSynonymCount("music"s), 0);
    ASSERT_EQUAL(synonyms.GetSynonymCount("melody"s), 0);

    synonyms.Add("music"s, "melody"s);
    ASSERT_EQUAL(synonyms.GetSynonymCount("music"s), 1);
    ASSERT_EQUAL(synonyms.GetSynonymCount("melody"s), 1);

    synonyms.Add("music"s, "tune"s);
    ASSERT_EQUAL(synonyms.GetSynonymCount("music"s), 2);
    ASSERT_EQUAL(synonyms.GetSynonymCount("tune"s), 1);
    ASSERT_EQUAL(synonyms.GetSynonymCount("melody"s), 1);
}

void TestAreSynonyms() 
{
    Synonyms synonyms;

    synonyms.Add("winner"s, "champion"s);
    synonyms.Add("good"s, "nice"s);

    ASSERT(synonyms.AreSynonyms("winner"s, "champion"s));
    ASSERT(synonyms.AreSynonyms("champion"s, "winner"s));

    ASSERT(!synonyms.AreSynonyms("good"s, "champion"s));
    ASSERT(synonyms.AreSynonyms("good"s, "nice"s));
}

void TestSynonyms() 
{
    TestAddingSynonymsIncreasesTheirCount();
    TestAreSynonyms();
}

int top_2_lesson_5() 
{
    TestSynonyms();

    Synonyms synonyms;

    string line;
    while (getline(cin, line)) {
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
        else if (action == "EXIT") {
            break;
        }
    }

    return(0);
}
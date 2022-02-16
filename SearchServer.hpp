// СПРИНТ 3. ИТОГОВЫЙ ПРОЕКТ. ОБРАБОТКА ОШИБОК(ИСКЛЮЧЕНИЙ) В ПОИСКОВОЙ СИСТЕМЕ
//============================================================================
#include <algorithm>
#include <cassert>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <deque>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;
const double MEASURE = 1e-6;

//========================================================
// Сопутствующие функции
//========================================================

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result;
    cin >> result;
    ReadLine();
    return result;
}

// Проверка корректного ввода минус слов
bool NegativeWordErr(const string& word) {
    // если после символа 'минус' отсутствует текст
    if(word.size() == 1) {
        if(word[0] == '-') {
            return false;
        }
    }
    // если в запросе более одного минуса перед минус-словами
    if(word.size() > 1) {
        if (word[0] == '-' && word[1] == '-') {
            return false;
        }
    }
    return true;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;

    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }

    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}

struct Document {
    Document() :
            id(0),
            relevance(0.0),
            rating(0)
    {}

    Document(int id_m, double relevance_m, int rating_m) {
        id = id_m;
        relevance = relevance_m;
        rating = rating_m;
    }

    int id;
    double relevance;
    int rating;
};

template <typename StringContainer>
set<string> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
    set<string> non_empty_strings;
    for (const string& str : strings) {
        if (!str.empty()) {
            non_empty_strings.insert(str);
        }
    }
    return non_empty_strings;
}

enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

//========================================================
// Класс поисковой системы
//========================================================
class SearchServer {
public:
    // Конструктор
    explicit SearchServer()
    = default;

    explicit SearchServer(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            if(IsValidWord(word)) {
                stop_words_.insert(word);
            } else {
                throw invalid_argument("В стоп-словах содержатся недопустимые символы!");
            }
        }
    }

    template <typename  StringCollection>
    explicit SearchServer(const StringCollection& collection) {
        for (const string& text : collection) {
            for (const string& word : SplitIntoWords(text)) {
                if(IsValidWord(word)) {
                    stop_words_.insert(word);
                } else {
                    throw invalid_argument("Stop-word contains invalid characters");
                }
            }
        }
    }

    // Добавляет в словарь новый документ
    void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
        // Проверка на отрицательный ID
        if(document_id < 0) {
            throw invalid_argument("документ содержит отрицательный id!");
        }
        // Проверка на наличие документов с одинаковым ID
        if(documents_.count(document_id)) {
            throw invalid_argument("документ с таким id уже существует!");
        }

        const vector<string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();

        for (const string& word : words) {
            // Проверка на наличие спец символов в документе
            if(!IsValidWord(word)) {
                throw invalid_argument("документ содержит недопустимые символы!");
            } else {
                word_to_document_freqs_[word][document_id] += inv_word_count;
            }
        }

        documents_.emplace(document_id,DocumentData{ComputeAverageRating(ratings), status});
        document_ids_.push_back(document_id);
    }

    // Вариант FindTopDocuments в котором не указывается статус, а документы фильтруются по умолчанию (DocumentStatus::ACTIVE)
    vector<Document> FindTopDocuments(const string& raw_query) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
    }

    // Вариант FindTopDocuments в котором документы фильтруются по статусу
    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        return FindTopDocuments(raw_query,[status]([[maybe_unused]] int document_id, DocumentStatus document_status, [[maybe_unused]] int rating){return ((document_status == status));});
    }

    // Вариант FindTopDocuments, в котором задается функция-предикат key_filter по условиям которой будут фильтроваться документы
    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const {
        // Минус и плюс слова
        const Query query = ParseQuery(raw_query);
        //  ID - РЕЛЕВАНТНОСТЬ - РЕЙТИНГ
        auto matched_documents = FindAllDocuments(query, document_predicate);
        // Сортируем по релевантности. Если релевантность одинаковая, сортируем по рейтингу
        sort(matched_documents.begin(), matched_documents.end(),[](const Document& lhs, const Document& rhs) {
            if (abs(lhs.relevance - rhs.relevance) < MEASURE) {
                return lhs.rating > rhs.rating;
            } else {
                return lhs.relevance > rhs.relevance;
            }
        });
        // Отсатвляем только первые MAX_RESULT_DOCUMENT_COUNT документов
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

    // метод позволяющий получить идентификатор документа по его порядковому номеру
    int GetDocumentId(int index) const {
        return document_ids_.at(index);
    }

    tuple<vector<string>, DocumentStatus> MatchDocument(const string& raw_query, int document_id) const {
        const Query query = ParseQuery(raw_query);
        vector<string> matched_words;

        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }

            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }

        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }

            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }
        return { matched_words, documents_.at(document_id).status };
    }

private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    set<string> stop_words_;
    vector<int> document_ids_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;

    static bool IsValidWord(const string& word) {
        // A valid word must not contain special characters
        return none_of(word.begin(), word.end(), [](char c) {
            return c >= '\0' && c < ' ';
        });
    }

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    static int ComputeAverageRating(const vector<int>& ratings) {
        if (ratings.empty()) {
            return 0;
        }

        int rating_sum = 0;

        for (const int rating : ratings) {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    }

    struct QueryWord {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(string text) const {
        bool is_minus = false;
        if (text[0] == '-') {
            is_minus = true;
            text = text.substr(1);
        }
        return {text, is_minus,IsStopWord(text)};
    }

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            // некорректный ввод минус слов
            if(!NegativeWordErr(word)) {
                throw invalid_argument("некорректный ввод минус слов!");
            }
            // Проверка на наличие спец символов в запросе
            if(!IsValidWord(word)) {
                throw invalid_argument("документ содержит недопустимые символы!");
            }
            const QueryWord query_word = ParseQueryWord(word);

            if (!query_word.is_stop) {
                if (query_word.is_minus) {
                    query.minus_words.insert(query_word.data);
                } else {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }

    // Считает долю документов в которых встречается слово из запроса
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const {
        map<int, double> document_to_relevance;
        // расчитываем релевантность для каждого документа, где встречается слово из запроса
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);

            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                // Отфильтровываем документы по заданным условиям в функции key_filter
                if (document_predicate(document_id, documents_.at(document_id).status, documents_.at(document_id).rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }
        // убираем релевантности документов, которые содержат минус слова
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        // Формируем результат: ID - РЕЛЕВАНТНОСТЬ - РЕЙТИНГ
        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back({ document_id, relevance, documents_.at(document_id).rating });
        }
        return matched_documents;
    }
};
//========================================================
// Фреймворк и модульные тесты поисковой системы
//========================================================
// Функции макросов
void AssertImpl(bool value, const string &hint,
                const string &expr_str,
                const string &file,
                const string &func,
                unsigned line) {
    if (!value) {
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT("s << expr_str << ") failed."s;

        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }

        cerr << endl;
        abort();
    }
}

template<typename A, typename B>
void AssertEqual(const A &a, const B &b, const string &hint,
                 const string &a_str,
                 const string &b_str,
                 const string &file,
                 const string &func,
                 unsigned line) {
    if (a != b) {
        cerr << boolalpha;
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT_EQUAL("s << a_str << ", "s << b_str << ") failed: "s;
        cerr << a << " != "s << b << "."s;

        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }

        cerr << endl;
        abort();
    }
}

// Макросы
#define ASSERT(expr) AssertImpl((expr), ""s, #expr, __FILE__, __FUNCTION__, __LINE__)
#define ASSERT_HINT(expr, hint) AssertImpl((expr), hint, #expr, __FILE__, __FUNCTION__, __LINE__)

#define ASSERT_EQUAL(a, b) AssertEqual((a), (b), ""s, #a, #b, __FILE__, __FUNCTION__, __LINE__)
#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqual((a), (b), (hint), #a, #b, __FILE__, __FUNCTION__, __LINE__)

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1U);
        const Document &doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }

    // Затем убеждаемся, что поиск этого же слова, входящего в список стоп-слов,
    // возвращает пустой результат
    {
        SearchServer server("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT(server.FindTopDocuments("in"s).empty());
    }
}

// Тест на добавление документов
void TestSearchAddDocuments() {
    {
        const int doc_id_1 = 0;
        const int doc_id_2 = 1;
        const int doc_id_3 = 2;

        const vector<int> ratings = {1, 2, 3};

        const string document_1 = "belyj kot i modnyj oshejnik"s;
        const string document_2 = "uhozhennyj skvorec evgenij"s;
        const string document_3 = ""s;

        SearchServer server;

        server.AddDocument(doc_id_1, document_1, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id_3, document_3, DocumentStatus::ACTUAL, ratings);

        const string content = "skvorec"s;

        // убеждаемся, что поиск ничего не даст
        ASSERT(server.FindTopDocuments(content).empty());

        // убеждаемся, что поиск выдаст добавленный документ, в котором содержатся слова из запроса
        server.AddDocument(doc_id_2, document_2, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments(content);

        ASSERT_EQUAL(found_docs.size(), 1U);
        const Document &doc0 = found_docs[0];

        ASSERT_EQUAL(doc0.id, doc_id_2);
    }
}

// Стоп-слова исключаются из текста документов
void TestNoStopWords() {
    const int doc_id = 0;
    const vector<int> ratings = {1, 2, 3};
    const string stop_words = "i kot"s;
    const string document = "belyj kot i modnyj oshejnik"s;
    const string content = "kot"s;

    {
        SearchServer server;
        server.AddDocument(doc_id, document, DocumentStatus::ACTUAL, ratings);

        // результатом будет документ с id = doc_id
        const auto found_docs = server.FindTopDocuments(content);

        ASSERT_EQUAL(found_docs.size(), 1U);
        ASSERT_EQUAL(found_docs[0].id, doc_id);
    }

    {
        SearchServer server(stop_words);
        server.AddDocument(doc_id, document, DocumentStatus::ACTUAL, ratings);

        // убеждаемся, что поиск ничего не даст
        const auto found_docs = server.FindTopDocuments(content);
        ASSERT(found_docs.empty());
    }
}

// Поддержка минус - слов. Документы, содержащие минус - слова поискового запроса, не должны включаться в результаты поиска.
void TestSupportMinusWords() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;

    const string document_1 = "uhozhennyj kot i modnyj oshejnik"s;
    const string document_2 = "belyj kot i modnyj oshejnik"s;
    const string content = "-uhozhennyj kot i modnyj oshejnik"s;

    server.AddDocument(doc_id_1, document_1, DocumentStatus::ACTUAL, ratings);
    ASSERT(server.FindTopDocuments(content).empty());

    server.AddDocument(doc_id_2, document_2, DocumentStatus::ACTUAL, ratings);

    const auto found_docs = server.FindTopDocuments(content);
    ASSERT_EQUAL(found_docs.size(), 1U);

    const Document &doc = found_docs[0];
    ASSERT_EQUAL(doc.id, doc_id_2);
}

// При матчинге документа по поисковому запросу должны быть возвращены все слова из поискового запроса, присутствующие в документе
void TestDocumentMatching() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;
    const int doc_id_3 = 2;
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;

    const string document_1 = ""s;
    const string document_2 = "belyj kot i modnyj oshejnik"s;
    const string document_3 = "uhozhennyj kot i modnyj oshejnik"s;
    const string content = "-belyj modnyj kot"s;

    // добавлен пустой документ
    server.AddDocument(doc_id_1, document_1, DocumentStatus::ACTUAL, ratings);
    {
        auto[words, status] = server.MatchDocument(content, doc_id_1);
        ASSERT(words.empty());
    }

    // добавлен документ, содержащий минус слово
    server.AddDocument(doc_id_2, document_2, DocumentStatus::ACTUAL, ratings);
    {
        auto[words, status] = server.MatchDocument(content, doc_id_2);
        ASSERT(words.empty());
    }

    // добавлен документ, содержащий  только плюс слова
    server.AddDocument(doc_id_3, document_3, DocumentStatus::ACTUAL, ratings);
    {
        auto[words, status] = server.MatchDocument(content, doc_id_3);
        ASSERT_EQUAL(words.size(), 2U);
        ASSERT_EQUAL(words[0], "kot"s);
        ASSERT_EQUAL(words[1], "modnyj"s);
    }
}

// Сортировка найденных документов по релевантности
void TestSortByRelevance() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;
    const int doc_id_3 = 2;
    const int doc_id_4 = 3;

    SearchServer server;

    const string document_1 = ""s;
    const string document_2 = "belyj kot i modnyj oshejnik"s;
    const string document_3 = "uhozhennyj kot i modnyj oshejnik"s;
    const string document_4 = "kot modnyj hvost"s;

    const string content = "belyj modnyj kot"s;

    server.AddDocument(doc_id_1, document_1, DocumentStatus::ACTUAL, {8, -3});
    server.AddDocument(doc_id_2, document_2, DocumentStatus::ACTUAL, {7, 2, 7});
    server.AddDocument(doc_id_3, document_3, DocumentStatus::ACTUAL, {5, -12, 2, 1});
    server.AddDocument(doc_id_4, document_4, DocumentStatus::ACTUAL, {5, -12, 10, 1});

    auto results = server.FindTopDocuments(content);

    ASSERT(is_sorted(results.begin(), results.end(),
                     [](const Document &lhs, const Document &rhs) { return lhs.relevance > rhs.relevance; }));

    ASSERT_EQUAL(results.size(), 3U);
    ASSERT_EQUAL(results[0].id, doc_id_2);
    ASSERT_EQUAL(results[1].id, doc_id_4);
    ASSERT_EQUAL(results[2].id, doc_id_3);
}

// Рейтинг добавленного документа равен среднему арифметическому оценок документа
void TestCalculateRatings() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;

    SearchServer server;

    const string document_1 = "pushistyj kot modnyj hvost"s;
    const string document_2 = "uhozhennyj kot i modnyj oshejnik"s;

    const string content = "belyj modnyj kot"s;

    // подаем пустой рейтинг для документа doc_id_1

    std::vector<int> ratings_1 = {};
    server.AddDocument(doc_id_1, document_1, DocumentStatus::ACTUAL, ratings_1);

    {
        auto result = server.FindTopDocuments(content);

        ASSERT_EQUAL(result.size(), 1U);
        ASSERT_EQUAL(result[0].rating, 0);
    }

    // задаем рейтинг

    std::vector<int> ratings_2 = {5, -12, 2, 1};
    int value = -1;
    server.AddDocument(doc_id_2, document_2, DocumentStatus::ACTUAL, ratings_2);

    {
        auto result = server.FindTopDocuments(content);

        ASSERT_EQUAL(result.size(), 2U);
        ASSERT_EQUAL(result[0].rating, 0);
        ASSERT_EQUAL(result[1].rating, value);
    }
}

// Фильтрация результатов поиска с использованием предиката, задаваемого пользователем
void TestFilteringUsingPredictFunction() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;
    const int doc_id_3 = 2;
    const int doc_id_4 = 3;
    const int doc_id_5 = 4;

    SearchServer server;

    const string document_1 = "pushistyj kot belyj hvost"s;
    const string document_2 = "belyj pjos vyrazitelnye glaza"s;
    const string document_3 = "belyj kot i modnyj oshejnik"s;
    const string document_4 = "modnyj kot belyj glaza"s;
    const string document_5 = "chernyi kot i modnyj hvost"s;

    const string content = "belyj hvost glaza"s;

    server.AddDocument(doc_id_1, document_1, DocumentStatus::ACTUAL, {-8});
    server.AddDocument(doc_id_2, document_2, DocumentStatus::BANNED, {10});
    server.AddDocument(doc_id_3, document_3, DocumentStatus::ACTUAL, {5});
    server.AddDocument(doc_id_4, document_4, DocumentStatus::IRRELEVANT, {7});
    server.AddDocument(doc_id_5, document_5, DocumentStatus::REMOVED, {-2});

    // вернет результаты с положительным рейтингом
    {
        auto results = server.FindTopDocuments(content,
                                               []([[maybe_unused]] int document_id,
                                                  [[maybe_unused]] DocumentStatus status, [[maybe_unused]] int rating) {
                                                   return (rating > 0);
                                               });

        ASSERT_EQUAL(results.size(), 3);
        ASSERT_EQUAL(results[0].id, doc_id_2);
        ASSERT_EQUAL(results[1].id, doc_id_4);
        ASSERT_EQUAL(results[2].id, doc_id_3);
    }

    // вернет только BANNED результат
    {
        auto results = server.FindTopDocuments(content,
                                               []([[maybe_unused]] int document_id,
                                                  [[maybe_unused]] DocumentStatus status, [[maybe_unused]] int rating) {
                                                   return (status == DocumentStatus::BANNED);
                                               });

        ASSERT_EQUAL(results.size(), 1U);
        ASSERT_EQUAL(results[0].id, doc_id_2);
    }

    // вернет только результат с нечетным id
    {
        auto results = server.FindTopDocuments(content,
                                               []([[maybe_unused]] int document_id, DocumentStatus status,
                                                  [[maybe_unused]] int rating) {
                                                   return (document_id % 2);
                                               });

        ASSERT_EQUAL(results.size(), 2U);
        ASSERT_EQUAL(results[0].id, doc_id_2);
        ASSERT_EQUAL(results[1].id, doc_id_4);
    }
}

// Поиск документов, имеющих заданный статус
void TestSearchDocumentsByStatus() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;
    const int doc_id_3 = 2;
    const int doc_id_4 = 3;
    const int doc_id_5 = 4;

    SearchServer server;

    const string document_1 = "pushistyj kot belyj hvost"s;
    const string document_2 = "belyj pjos vyrazitelnye glaza"s;
    const string document_3 = "belyj kot i modnyj oshejnik"s;
    const string document_4 = "modnyj kot belyj glaza"s;
    const string document_5 = "chernyi kot i modnyj hvost"s;

    const string content = "belyj hvost glaza"s;

    server.AddDocument(doc_id_1, document_1, DocumentStatus::ACTUAL, {-8});
    server.AddDocument(doc_id_2, document_2, DocumentStatus::BANNED, {10});
    server.AddDocument(doc_id_3, document_3, DocumentStatus::ACTUAL, {5});
    server.AddDocument(doc_id_4, document_4, DocumentStatus::IRRELEVANT, {7});
    server.AddDocument(doc_id_5, document_5, DocumentStatus::REMOVED, {-2});

    // вернет только ACTUAL результат
    {
        auto results = server.FindTopDocuments(content,
                                               []([[maybe_unused]] int document_id, DocumentStatus status,
                                                  [[maybe_unused]] int rating) {
                                                   return (status == DocumentStatus::ACTUAL);
                                               });

        ASSERT_EQUAL(results.size(), 2U);
        ASSERT_EQUAL(results[0].id, doc_id_1);
        ASSERT_EQUAL(results[1].id, doc_id_3);
    }

    // вернет только BANNED результат
    {
        auto results = server.FindTopDocuments(content,
                                               []([[maybe_unused]] int document_id, DocumentStatus status,
                                                  [[maybe_unused]] int rating) {
                                                   return (status == DocumentStatus::BANNED);
                                               });

        ASSERT_EQUAL(results.size(), 1U);
        ASSERT_EQUAL(results[0].id, doc_id_2);
    }

    // вернет только IRRELEVANT результат
    {
        auto results = server.FindTopDocuments(content,
                                               []([[maybe_unused]] int document_id, DocumentStatus status,
                                                  [[maybe_unused]] int rating) {
                                                   return (status == DocumentStatus::IRRELEVANT);
                                               });

        ASSERT_EQUAL(results.size(), 1U);
        ASSERT_EQUAL(results[0].id, doc_id_4);
    }

    // вернет только REMOVED результат
    {
        auto results = server.FindTopDocuments(content,
                                               []([[maybe_unused]] int document_id, DocumentStatus status,
                                                  [[maybe_unused]] int rating) {
                                                   return (status == DocumentStatus::REMOVED);
                                               });

        ASSERT_EQUAL(results.size(), 1U);
        ASSERT_EQUAL(results[0].id, doc_id_5);
    }

    // Вернет все результаты
    {
        auto results = server.FindTopDocuments(content,
                                               []([[maybe_unused]] int document_id, DocumentStatus status,
                                                  [[maybe_unused]] int rating) {
                                                   return ((status == DocumentStatus::ACTUAL) |
                                                           (status == DocumentStatus::BANNED) |
                                                           (status == DocumentStatus::IRRELEVANT) |
                                                           (status == DocumentStatus::REMOVED)
                                                   );
                                               });

        ASSERT_EQUAL(results.size(), 5U);
    }
}

// Корректное вычисление релевантности найденных документов
void TestCorrectCalcRelevance() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;
    const int doc_id_3 = 2;

    SearchServer server;

    const string document_1 = "pushistyj kot belyj hvost"s;
    const string document_2 = "belyj pjos vyrazitelnye glaza"s;
    const string document_3 = "belyj kot i modnyj oshejnik"s;

    const string content = "kot belyj hvost"s;

    server.AddDocument(doc_id_1, document_1, DocumentStatus::ACTUAL, {8, -3});
    server.AddDocument(doc_id_2, document_2, DocumentStatus::ACTUAL, {7, 2, 7});
    server.AddDocument(doc_id_3, document_3, DocumentStatus::ACTUAL, {5, -12, 2, 1});

    double value_1 = 0.376019;
    double value_2 = 0.081093;
    double value_3 = 0;

    auto results = server.FindTopDocuments(content, DocumentStatus::ACTUAL);

    ASSERT_EQUAL(results.size(), 3U);
    ASSERT((results[0].relevance - value_1) < MEASURE);
    ASSERT((results[1].relevance - value_2) < MEASURE);
    ASSERT((results[2].relevance - value_3) < MEASURE);
}

// Посик по пустому запросу
void TestEmptyContent() {
    const int doc_id = 0;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments(""s);
        ASSERT(found_docs.empty());
    }
}

// Введенные плюс и минус слова одинаковы
void TestSamePlusMinusWord() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;

    const string document_1 = "uhozhennyj kot i modnyj oshejnik"s;
    const string document_2 = "belyj kot i modnyj oshejnik"s;
    const string content = "-kot kot"s;

    server.AddDocument(doc_id_1, document_1, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_2, document_2, DocumentStatus::ACTUAL, ratings);

    const auto found_docs = server.FindTopDocuments(content);
    ASSERT(found_docs.empty());
}

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    // Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
    TestExcludeStopWordsFromAddedDocumentContent();

    // Добавленный документ должен находиться по поисковому запросу, который содержит слова из документа
    TestSearchAddDocuments();

    // Стоп-слова исключаются из текста документов
    TestNoStopWords();

    // Поддержка минус - слов. Документы, содержащие минус - слова поискового запроса, не должны включаться в результаты поиска
    TestSupportMinusWords();

    // Матчинг документа
    TestDocumentMatching();

    // Сортировка найденных документов по релевантности
    TestSortByRelevance();

    // Вычисление рейтинга документов
    TestCalculateRatings();

    // Фильтрация результатов поиска с использованием предиката, задаваемого пользователем
    TestFilteringUsingPredictFunction();

    // Поиск документов, имеющих заданный статус
    TestSearchDocumentsByStatus();

    // Корректное вычисление релевантности найденных документов
    TestCorrectCalcRelevance();

    // Посик по пустому запросу
    TestEmptyContent();

    // Введенные плюс и минус слова одинаковы
    TestSamePlusMinusWord();
}

//========================================================
// Обертки для отладки
//========================================================
void PrintDocument(const Document& document) {
    cout << "{ "s
         << "document_id = "s << document.id << ", "s
         << "relevance = "s << document.relevance << ", "s
         << "rating = "s << document.rating << " }"s << endl;
}

void PrintMatchDocumentResult(int document_id, const vector<string>& words, DocumentStatus status) {
    cout << "{ "s
         << "document_id = "s << document_id << ", "s
         << "status = "s << static_cast<int>(status) << ", "s
         << "words ="s;
    for (const string& word : words) {
        cout << ' ' << word;
    }
    cout << "}"s << endl;
}

void AddDocument(SearchServer& search_server, int document_id, const string& document, DocumentStatus status,
                 const vector<int>& ratings) {
    try {
        search_server.AddDocument(document_id, document, status, ratings);
    } catch (const exception& e) {
        cout << "Ошибка добавления документа "s << document_id << ": "s << e.what() << endl;
    }
}

void FindTopDocuments(const SearchServer& search_server, const string& raw_query) {
    cout << "Результаты поиска по запросу: "s << raw_query << endl;
    try {
        for (const Document& document : search_server.FindTopDocuments(raw_query)) {
            PrintDocument(document);
        }
    } catch (const exception& e) {
        cout << "Ошибка поиска: "s << e.what() << endl;
    }
}

void MatchDocuments(const SearchServer& search_server, const string& query) {
    try {
        cout << "Матчинг документов по запросу: "s << query << endl;
        const int document_count = search_server.GetDocumentCount();
        for (int index = 0; index < document_count; ++index) {
            const int document_id = search_server.GetDocumentId(index);
            const auto [words, status] = search_server.MatchDocument(query, document_id);
            PrintMatchDocumentResult(document_id, words, status);
        }
    } catch (const exception& e) {
        cout << "Ошибка матчинга документов на запрос "s << query << ": "s << e.what() << endl;
    }
}

//==============================================================
/* Класс для хранения пары итераторов.Используется для
   разделения документов из результата на отдельные страницы
 */
 //==============================================================
template <typename Iterator>
class IteratorRange
{
public:
    IteratorRange(Iterator begin, Iterator end) :
        start_(begin),
        stop_(end),
        size_range_(distance(start_, stop_)) {
    }

    Iterator begin() const {
        return start_;
    }

    Iterator end() const {
        return stop_;
    }

    size_t size() const {
        return size_range_;
    }

private:
    Iterator start_;
    Iterator stop_;
    size_t size_range_;
};

template <typename Iterator>
ostream& operator<<(ostream& out, const IteratorRange<Iterator>& range) {
    for (Iterator it = range.begin(); it != range.end(); ++it) {
        out << *it;
    }
    return out;
}

// Перегрузка оператора вывода для Document
ostream& operator<<(ostream& out, const Document& document) {
    out << "{ "s
        << "document_id = "s << document.id << ", "s
        << "relevance = "s << document.relevance << ", "s
        << "rating = "s << document.rating << " }"s;
    return out;
}
//==============================================================
/* Класс разделяет документы из результата на отдельные страницы
*/
//==============================================================

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        for (size_t left = distance(begin, end); left > 0;) {
            const size_t current_page_size = min(page_size, left);
            const Iterator current_page_end = next(begin, current_page_size);
            pages_.push_back({ begin, current_page_end });

            left -= current_page_size;
            begin = current_page_end;
        }
    }

    auto begin() const {
        return pages_.begin();
    }

    auto end() const {
        return pages_.end();
    }

    size_t size() const {
        return pages_.size();
    }

private:
    std::vector<IteratorRange<Iterator>> pages_;

};

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

//==============================================================
/* Класс определяет очередь запросов и  является "оберткой" для методов поиска

Метод GetNoResultRequests возвращает сколько за
последние сутки (min_in_day_) было запросов, на которые
ничего не нашлось.
 */
//==============================================================
class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server):
            search_server_(search_server),
            min_pass_(0),
            count_no_result_(0)
    {}
    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    template <typename DocumentPredicate>
    vector<Document> AddFindRequest(const string& raw_query, DocumentPredicate document_predicate) {
        return AddRequest(search_server_.FindTopDocuments(raw_query, document_predicate));
    }

    vector<Document> AddFindRequest(const string& raw_query, DocumentStatus status) {
        return AddRequest(search_server_.FindTopDocuments(raw_query, status));
    }

    vector<Document> AddFindRequest(const string& raw_query) {
        return AddRequest(search_server_.FindTopDocuments(raw_query));
    }

    int GetNoResultRequests() const {
        return count_no_result_;
    }
private:
    const SearchServer& search_server_;
    const static int min_in_day_ = 1440;

    struct QueryResult {
        vector<Document> search_results;
        bool is_empty_result = false;
    };

    deque<QueryResult> requests_;
    int min_pass_;
    int count_no_result_;

    // метод добавляет результаты поиска в очередь хранения
    vector<Document> AddRequest(vector<Document> search_results) {
        QueryResult query_results;
        query_results.search_results = search_results;
        min_pass_++;

        if(query_results.search_results.empty()) {
            query_results.is_empty_result = true;
            count_no_result_++;
        }

        if(min_pass_ <= min_in_day_)
            requests_.push_back(query_results);
        else {
            if(requests_.front().is_empty_result) {
                count_no_result_--;
            }
            requests_.pop_front();
            requests_.push_back(query_results);
        }

        return search_results;
    }
};
//==============================================================
int run() 
{
    // ТЕСТИРОВАНИЕ
    //TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;

    // ОТЛАДКА
    try {
        SearchServer search_server("and with"s);
        RequestQueue request_queue(search_server);

        search_server.AddDocument(1, "funny pet and nasty rat"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "funny pet with curly hair"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
        search_server.AddDocument(3, "big cat nasty hair"s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog cat Vladislav"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
        search_server.AddDocument(5, "big dog hamster Borya"s, DocumentStatus::ACTUAL, { 1, 1, 1 });

        // Определяем, сколько запросов остались без ответа
        const auto search_results = request_queue.AddFindRequest("curly dog"s);
        cout << "Всего запросов, оставшихся без ответа: "s << request_queue.GetNoResultRequests() << endl;

        // Разделяем вывод результатов поиска на отдельные страницы
        int page_size = 2;
        const auto pages = Paginate(search_results, page_size);

        for (auto page = pages.begin(); page != pages.end(); ++page) {
            cout << *page << endl;
            cout << "Page break"s << endl;
        }

    } catch (const exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}
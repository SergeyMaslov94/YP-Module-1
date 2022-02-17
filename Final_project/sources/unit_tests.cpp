#include "unit_tests.h"
#include "document.h"
#include "search_server.h"
#include <algorithm>

// Функции макросов
void AssertImpl(bool value, const std::string &hint,
                const std::string &expr_str,
                const std::string &file,
                const std::string &func,
                unsigned line) {
    if (!value) {
        std::cerr << file << "("s << line << "): "s << func << ": "s;
        std::cerr << "ASSERT("s << expr_str << ") failed."s;

        if (!hint.empty()) {
            std::cerr << " Hint: "s << hint;
        }

        std::cerr << std::endl;
        abort();
    }
}

template<typename A, typename B>
void AssertEqual(const A &a, const B &b, const std::string &hint,
                 const std::string &a_str,
                 const std::string &b_str,
                 const std::string &file,
                 const std::string &func,
                 unsigned line) {
    if (a != b) {
        std::cerr << boolalpha;
        std::cerr << file << "("s << line << "): "s << func << ": "s;
        std::cerr << "ASSERT_EQUAL("s << a_str << ", "s << b_str << ") failed: "s;
        std::cerr << a << " != "s << b << "."s;

        if (!hint.empty()) {
            std::cerr << " Hint: "s << hint;
        }

        std::cerr << std::endl;
        abort();
    }
}

// Макросы
#define ASSERT(expr) AssertImpl((expr), ""s, #expr, __FILE__, __FUNCTION__, __LINE__)
#define ASSERT_HINT(expr, hint) AssertImpl((expr), hint, #expr, __FILE__, __FUNCTION__, __LINE__)

#define ASSERT_EQUAL(a, b) AssertEqual((a), (b), ""s, #a, #b, __FILE__, __FUNCTION__, __LINE__)
#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqual((a), (b), (hint), #a, #b, __FILE__, __FUNCTION__, __LINE__)

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void UnitTestsSearchServer::TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const std::string content = "cat in the city"s;
    const std::vector<int> ratings = {1, 2, 3};
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
void UnitTestsSearchServer::TestSearchAddDocuments() {
    {
        const int doc_id_1 = 0;
        const int doc_id_2 = 1;
        const int doc_id_3 = 2;

        const std::vector<int> ratings = {1, 2, 3};

        const std::string document_1 = "belyj kot i modnyj oshejnik"s;
        const std::string document_2 = "uhozhennyj skvorec evgenij"s;
        const std::string document_3 = ""s;

        SearchServer server;

        server.AddDocument(doc_id_1, document_1, DocumentStatus::ACTUAL, ratings);
        server.AddDocument(doc_id_3, document_3, DocumentStatus::ACTUAL, ratings);

        const std::string content = "skvorec"s;

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
void UnitTestsSearchServer::TestNoStopWords() {
    const int doc_id = 0;
    const std::vector<int> ratings = {1, 2, 3};
    const std::string stop_words = "i kot"s;
    const std::string document = "belyj kot i modnyj oshejnik"s;
    const std::string content = "kot"s;

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
void UnitTestsSearchServer::TestSupportMinusWords() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;
    const std::vector<int> ratings = {1, 2, 3};

    SearchServer server;

    const std::string document_1 = "uhozhennyj kot i modnyj oshejnik"s;
    const std::string document_2 = "belyj kot i modnyj oshejnik"s;
    const std::string content = "-uhozhennyj kot i modnyj oshejnik"s;

    server.AddDocument(doc_id_1, document_1, DocumentStatus::ACTUAL, ratings);
    ASSERT(server.FindTopDocuments(content).empty());

    server.AddDocument(doc_id_2, document_2, DocumentStatus::ACTUAL, ratings);

    const auto found_docs = server.FindTopDocuments(content);
    ASSERT_EQUAL(found_docs.size(), 1U);

    const Document &doc = found_docs[0];
    ASSERT_EQUAL(doc.id, doc_id_2);
}

// При матчинге документа по поисковому запросу должны быть возвращены все слова из поискового запроса, присутствующие в документе
void UnitTestsSearchServer::TestDocumentMatching() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;
    const int doc_id_3 = 2;
    const std::vector<int> ratings = {1, 2, 3};

    SearchServer server;

    const std::string document_1 = ""s;
    const std::string document_2 = "belyj kot i modnyj oshejnik"s;
    const std::string document_3 = "uhozhennyj kot i modnyj oshejnik"s;
    const std::string content = "-belyj modnyj kot"s;

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
void UnitTestsSearchServer::TestSortByRelevance() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;
    const int doc_id_3 = 2;
    const int doc_id_4 = 3;

    SearchServer server;

    const std::string document_1 = ""s;
    const std::string document_2 = "belyj kot i modnyj oshejnik"s;
    const std::string document_3 = "uhozhennyj kot i modnyj oshejnik"s;
    const std::string document_4 = "kot modnyj hvost"s;

    const std::string content = "belyj modnyj kot"s;

    server.AddDocument(doc_id_1, document_1, DocumentStatus::ACTUAL, {8, -3});
    server.AddDocument(doc_id_2, document_2, DocumentStatus::ACTUAL, {7, 2, 7});
    server.AddDocument(doc_id_3, document_3, DocumentStatus::ACTUAL, {5, -12, 2, 1});
    server.AddDocument(doc_id_4, document_4, DocumentStatus::ACTUAL, {5, -12, 10, 1});

    auto results = server.FindTopDocuments(content);

    ASSERT(std::is_sorted(results.begin(), results.end(),
                          [](const Document &lhs, const Document &rhs) { return lhs.relevance > rhs.relevance; }));

    ASSERT_EQUAL(results.size(), 3U);
    ASSERT_EQUAL(results[0].id, doc_id_2);
    ASSERT_EQUAL(results[1].id, doc_id_4);
    ASSERT_EQUAL(results[2].id, doc_id_3);
}

// Рейтинг добавленного документа равен среднему арифметическому оценок документа
void UnitTestsSearchServer::TestCalculateRatings() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;

    SearchServer server;

    const std::string document_1 = "pushistyj kot modnyj hvost"s;
    const std::string document_2 = "uhozhennyj kot i modnyj oshejnik"s;

    const std::string content = "belyj modnyj kot"s;

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
void UnitTestsSearchServer::TestFilteringUsingPredictFunction() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;
    const int doc_id_3 = 2;
    const int doc_id_4 = 3;
    const int doc_id_5 = 4;

    SearchServer server;

    const std::string document_1 = "pushistyj kot belyj hvost"s;
    const std::string document_2 = "belyj pjos vyrazitelnye glaza"s;
    const std::string document_3 = "belyj kot i modnyj oshejnik"s;
    const std::string document_4 = "modnyj kot belyj glaza"s;
    const std::string document_5 = "chernyi kot i modnyj hvost"s;

    const std::string content = "belyj hvost glaza"s;

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
void UnitTestsSearchServer::TestSearchDocumentsByStatus() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;
    const int doc_id_3 = 2;
    const int doc_id_4 = 3;
    const int doc_id_5 = 4;

    SearchServer server;

    const std::string document_1 = "pushistyj kot belyj hvost"s;
    const std::string document_2 = "belyj pjos vyrazitelnye glaza"s;
    const std::string document_3 = "belyj kot i modnyj oshejnik"s;
    const std::string document_4 = "modnyj kot belyj glaza"s;
    const std::string document_5 = "chernyi kot i modnyj hvost"s;

    const std::string content = "belyj hvost glaza"s;

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
void UnitTestsSearchServer::TestCorrectCalcRelevance() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;
    const int doc_id_3 = 2;

    SearchServer server;

    const std::string document_1 = "pushistyj kot belyj hvost"s;
    const std::string document_2 = "belyj pjos vyrazitelnye glaza"s;
    const std::string document_3 = "belyj kot i modnyj oshejnik"s;

    const std::string content = "kot belyj hvost"s;

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
void UnitTestsSearchServer::TestEmptyContent() {
    const int doc_id = 0;
    const std::string content = "cat in the city"s;
    const std::vector<int> ratings = {1, 2, 3};
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments(""s);
        ASSERT(found_docs.empty());
    }
}

// Введенные плюс и минус слова одинаковы
void UnitTestsSearchServer::TestSamePlusMinusWord() {
    const int doc_id_1 = 0;
    const int doc_id_2 = 1;
    const std::vector<int> ratings = {1, 2, 3};

    SearchServer server;

    const std::string document_1 = "uhozhennyj kot i modnyj oshejnik"s;
    const std::string document_2 = "belyj kot i modnyj oshejnik"s;
    const std::string content = "-kot kot"s;

    server.AddDocument(doc_id_1, document_1, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_2, document_2, DocumentStatus::ACTUAL, ratings);

    const auto found_docs = server.FindTopDocuments(content);
    ASSERT(found_docs.empty());
}

// Функция TestSearchServer является точкой входа для запуска тестов
void UnitTestsSearchServer::TestSearchServer() {
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
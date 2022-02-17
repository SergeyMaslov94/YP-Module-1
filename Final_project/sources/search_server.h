#pragma once
//========================================================
// Класс поисковой системы
//========================================================
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <algorithm>
#include "document.h"

const int MAX_RESULT_DOCUMENT_COUNT = 5;
const double MEASURE = 1e-6;

class SearchServer {
public:
    // Конструкторы класса
    explicit SearchServer();

    explicit SearchServer(const std::string& text);

    template <typename  StringCollection>
    explicit SearchServer(const StringCollection& collection) {
        for (const std::string& text : collection) {
            for (const std::string& word : SplitIntoWords(text)) {
                if(IsValidWord(word)) {
                    stop_words_.insert(word);
                } else {
                    throw std::invalid_argument("Stop-word contains invalid characters");
                }
            }
        }
    }

    // Метод добавления новых документов
    void AddDocument(int document_id, const std::string& document, DocumentStatus status, const std::vector<int>& ratings);

    // Вариант FindTopDocuments в котором не указывается статус, а документы фильтруются по умолчанию (DocumentStatus::ACTIVE)
    std::vector<Document> FindTopDocuments(const std::string& raw_query) const;

    // Вариант FindTopDocuments в котором документы фильтруются по статусу
    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentStatus status) const;

    // Вариант FindTopDocuments, в котором задается функция-предикат key_filter по условиям которой будут фильтроваться документы
    template <typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentPredicate document_predicate) const {
        // Минус и плюс слова
        const Query query = ParseQuery(raw_query);
        //  ID - РЕЛЕВАНТНОСТЬ - РЕЙТИНГ
        auto matched_documents = FindAllDocuments(query, document_predicate);
        // Сортируем по релевантности. Если релевантность одинаковая, сортируем по рейтингу
        std::sort(matched_documents.begin(), matched_documents.end(),[](const Document& lhs, const Document& rhs) {
            if (std::abs(lhs.relevance - rhs.relevance) < MEASURE) {
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

    // Количество документов  в базе
    int GetDocumentCount() const;

    // метод позволяющий получить идентификатор документа по его порядковому номеру
    int GetDocumentId(int index) const;

    // метод поиска совпадений запроса и документа из базы (с учетом плюс/минус слов)
    std::tuple<std::vector<std::string>, DocumentStatus> MatchDocument(const std::string& raw_query, int document_id) const;

private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    std::set<std::string> stop_words_;
    std::vector<int> document_ids_;
    std::map<std::string, std::map<int, double>> word_to_document_freqs_;
    std::map<int, DocumentData> documents_;

    static bool IsValidWord(const std::string& word);

    bool IsStopWord(const std::string& word) const;

    std::vector<std::string> SplitIntoWordsNoStop(const std::string& text) const;

    static int ComputeAverageRating(const std::vector<int>& ratings);

    struct QueryWord {
        std::string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(std::string text) const;

    struct Query {
        std::set<std::string> plus_words;
        std::set<std::string> minus_words;
    };

    Query ParseQuery(const std::string& text) const;

    // Считает долю документов в которых встречается слово из запроса
    double ComputeWordInverseDocumentFreq(const std::string& word) const;

    // Методы поиска всех документов, удовлетворяющих запросу
    template <typename DocumentPredicate>
    std::vector<Document> FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const {
        std::map<int, double> document_to_relevance;
        // расчитываем релевантность для каждого документа, где встречается слово из запроса
        for (const std::string& word : query.plus_words) {
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
        for (const std::string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        // Формируем результат: ID - РЕЛЕВАНТНОСТЬ - РЕЙТИНГ
        std::vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back({ document_id, relevance, documents_.at(document_id).rating });
        }
        return matched_documents;
    }
};
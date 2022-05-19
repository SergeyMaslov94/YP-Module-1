#pragma once
//========================================================
// Класс поисковой системы
//========================================================
#include <string>
#include <string_view>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <execution>
#include "document.h"
#include "string_processing.h"

const int MAX_RESULT_DOCUMENT_COUNT = 5;
const double MEASURE = 1e-6;

class SearchServer {
public:
    // Конструкторы класса
    explicit SearchServer();
    explicit SearchServer(const std::string_view& text);
    explicit SearchServer(const std::string& text);

    template <typename  StringCollection>
    explicit SearchServer(const StringCollection& collection) {
        for (const std::string& text : collection) {
            for (const std::string& word : SplitIntoWords(text)) {
                if (IsValidWord(word)) {
                    stop_words_.insert(word);
                }
                else {
                    throw std::invalid_argument("Stop-word contains invalid characters");
                }
            }
        }
    }

    // Метод добавления новых документов
    void AddDocument(int document_id, const std::string_view& document, DocumentStatus status, const std::vector<int>& ratings);

    // Вариант FindTopDocuments в котором не указывается статус, а документы фильтруются по умолчанию (DocumentStatus::ACTIVE)
    std::vector<Document> FindTopDocuments(const std::string_view& raw_query) const;

    // Вариант FindTopDocuments в котором документы фильтруются по статусу
    std::vector<Document> FindTopDocuments(const std::string_view& raw_query, DocumentStatus status) const;

    // Вариант FindTopDocuments, в котором задается функция-предикат key_filter по условиям которой будут фильтроваться документы
    template <typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(const std::string_view& raw_query, DocumentPredicate document_predicate) const;

    // Количество документов  в базе
    int GetDocumentCount() const;

    auto begin() const {
        return document_ids_.begin();
    };
    auto end() const {
        return document_ids_.end();
    };

    // получение частот слов по id документа:
    const std::map<std::string_view, double>& GetWordFrequencies(int document_id) const;

    // поиск совпадений запроса и документа из базы (с учетом плюс/минус слов)
    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(const std::string_view& raw_query, int document_id) const;
    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(std::execution::sequenced_policy seq, const std::string_view& raw_query, int document_id) const;
    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(std::execution::parallel_policy type, const std::string_view& raw_query, int document_id) const;

    // удаление документов из поискового сервера
    void RemoveDocument(int document_id);
    template<typename execution_type>
    void RemoveDocument(execution_type type, int document_id);

private:
    // Проверка корректного ввода минус слов
    bool NegativeWordErr(const std::string_view& word) const;

    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    struct QueryWord {
        std::string_view data;
        bool is_minus;
        bool is_stop;
    };

    struct Query {
        std::set<std::string_view> plus_words;
        std::set<std::string_view> minus_words;
    };

    struct Query_vector {
        std::vector<std::string_view> plus_words;
        std::vector<std::string_view> minus_words;
    };

    std::set<std::string> stop_words_;
    std::set<int> document_ids_;

    std::map<std::string, std::map<int, double>> word_to_document_freqs_;

    std::map<int, DocumentData> documents_;
    std::map<int, std::map<std::string_view, double>> ids_and_words_with_freqs;

    static bool IsValidWord(const std::string_view& word);
    static bool IsEmptyWord(const std::string_view& word);
    bool IsStopWord(const std::string_view& word) const;

    std::vector<std::string_view> SplitIntoWordsNoStop(const std::string_view& text) const;
    static int ComputeAverageRating(const std::vector<int>& ratings);

    QueryWord ParseQueryWord(std::string_view texts) const;
    Query_vector ParseQueryWordSeq(std::vector<std::string_view> texts) const;
    Query_vector ParseQueryWordParall(std::vector<std::string_view> texts) const;

    Query ParseQuery(const std::string_view& text) const;
    Query_vector ParseQuerySeq(const std::string_view& text) const;
    Query_vector ParseQueryParall(const std::string_view& text) const;

    // Считает долю документов в которых встречается слово из запроса
    double ComputeWordInverseDocumentFreq(const std::string& word) const;

    // Методы поиска всех документов, удовлетворяющих запросу
    template <typename DocumentPredicate>
    std::vector<Document> FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const;
};

// Методы поиска всех документов, удовлетворяющих запросу
template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const {
    std::map<int, double> document_to_relevance;
    // расчитываем релевантность для каждого документа, где встречается слово из запроса
    for (const std::string_view& word : query.plus_words) {
        if (word_to_document_freqs_.count({ word.begin(), word.end() }) == 0) {
            continue;
        }
        const double inverse_document_freq = ComputeWordInverseDocumentFreq({ word.begin(), word.end() });

        for (const auto [document_id, term_freq] : word_to_document_freqs_.at({ word.begin(), word.end() })) {
            // Отфильтровываем документы по заданным условиям в функции key_filter
            if (document_predicate(document_id, documents_.at(document_id).status, documents_.at(document_id).rating)) {
                document_to_relevance[document_id] += term_freq * inverse_document_freq;
            }
        }
    }
    // убираем релевантности документов, которые содержат минус слова
    for (const std::string_view& word : query.minus_words) {
        if (word_to_document_freqs_.count({ word.begin(), word.end() }) == 0) {
            continue;
        }
        for (const auto [document_id, _] : word_to_document_freqs_.at({ word.begin(), word.end() })) {
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

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindTopDocuments(const std::string_view& raw_query, DocumentPredicate document_predicate) const{

    // Минус и плюс слова
    const SearchServer::Query query = ParseQuery(raw_query);
    //  ID - РЕЛЕВАНТНОСТЬ - РЕЙТИНГ
    auto matched_documents = FindAllDocuments(query, document_predicate);
    // Сортируем по релевантности. Если релевантность одинаковая, сортируем по рейтингу
    std::sort(matched_documents.begin(), matched_documents.end(), [](const Document& lhs, const Document& rhs) {
        if (std::abs(lhs.relevance - rhs.relevance) < MEASURE) {
            return lhs.rating > rhs.rating;
        }
        else {
            return lhs.relevance > rhs.relevance;
        }
        });
    // Отсатвляем только первые MAX_RESULT_DOCUMENT_COUNT документов
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }
    return matched_documents;
}

template<typename execution_type>
void SearchServer::RemoveDocument(execution_type type, int document_id) {

    if (documents_.count(document_id) == 0) {
        return;
    }

    documents_.erase(document_id);
    document_ids_.erase(document_id);

    std::vector<std::string_view> words;
    std::vector<int> number_erased_element;

    for (const auto& word : GetWordFrequencies(document_id)) {
        words.push_back(word.first);
    }

    number_erased_element.resize(words.size());
    std::transform(type, words.begin(), words.end(), number_erased_element.begin(),
        [this, document_id](const std::string_view word) {
            return word_to_document_freqs_.at({ word.begin(), word.end() }).erase(document_id);
        });

    ids_and_words_with_freqs.erase(document_id);
}
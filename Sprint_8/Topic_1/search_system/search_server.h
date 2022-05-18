#pragma once
/*
 * По правде говоря я сам не до конца понял, с чего это вдруг у меня вышло разместить шаблонный класс в .cpp...
 * ну да ладно.
 * До варианта вынести метод из тела класса, признаю, недопонял
 */

//========================================================
// Класс поисковой системы
//========================================================
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include "document.h"
#include "string_processing.h"

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
    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentPredicate document_predicate) const;

    // Количество документов  в базе
    int GetDocumentCount() const;

    auto begin() const {
        return document_ids_.begin();
    };
    auto end() const {
        return document_ids_.end();
    };

    // получение частот слов по id документа:
    const std::map<std::string, double>& GetWordFrequencies(int document_id) const;

    // поиск совпадений запроса и документа из базы (с учетом плюс/минус слов)
    std::tuple<std::vector<std::string>, DocumentStatus> MatchDocument(const std::string& raw_query, int document_id) const;
    template<typename execution_type>
    std::tuple<std::vector<std::string>, DocumentStatus> MatchDocument(const execution_type type, const std::string& raw_query, int document_id);

    // удаление документов из поискового сервера
    void RemoveDocument(int document_id);
    template<typename execution_type>
    void RemoveDocument(execution_type type, int document_id);

private:
    // Проверка корректного ввода минус слов
    bool NegativeWordErr(const std::string& word) const;

    struct DocumentData {
        int rating;
        DocumentStatus status;
    };

    struct QueryWord {
        std::string data;
        bool is_minus;
        bool is_stop;
    };

    struct Query {
        std::set<std::string> plus_words;
        std::set<std::string> minus_words;
    };

    struct Query_parall {
        std::vector<std::string> plus_words;
        std::vector<std::string> minus_words;
    };

    std::set<std::string> duplicate_words_in_parse_query_;
    std::set<std::string> stop_words_;
    std::set<int> document_ids_;

    std::map<std::string, std::map<int, double>> word_to_document_freqs_;
    std::map<int, DocumentData> documents_;
    std::map<int, std::map<std::string, double>> ids_and_words_with_freqs;

    static bool IsValidWord(const std::string& word);
    bool IsStopWord(const std::string& word) const;

    std::vector<std::string> SplitIntoWordsNoStop(const std::string& text) const;
    static int ComputeAverageRating(const std::vector<int>& ratings);

    QueryWord ParseQueryWord(std::string text) const;
    Query_parall ParseQueryWordParall(std::vector<std::string> texts);

    Query ParseQuery(const std::string& text) const;
    template<typename execution_type>
    Query_parall ParseQuery(const execution_type type, const std::string& text);

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

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindTopDocuments(const std::string& raw_query, DocumentPredicate document_predicate) const {

    // Минус и плюс слова
    const SearchServer::Query query = ParseQuery(raw_query);
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

template<typename execution_type>
void SearchServer::RemoveDocument(execution_type type, int document_id) {

    if (documents_.count(document_id) == 0) {
        return;
    }

    documents_.erase(document_id);
    document_ids_.erase(document_id);

    std::vector<const std::string*> words;
    std::vector<int> number_erased_element;

    for (const auto& word : GetWordFrequencies(document_id)) {
        words.push_back(&word.first);
    }

    number_erased_element.resize(words.size());
    std::transform(type, words.begin(), words.end(), number_erased_element.begin(),
        [this, document_id](const std::string* word) {
            return word_to_document_freqs_.at(*word).erase(document_id);
        });

    ids_and_words_with_freqs.erase(document_id);
}

template<typename execution_type>
std::tuple<std::vector<std::string>, DocumentStatus> SearchServer::MatchDocument(const execution_type type, const std::string & raw_query, int document_id) {

    const Query_parall query = ParseQuery(type, raw_query);
    std::vector<std::string> matched_words;

    for (const std::string& word : query.plus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }

        if (word_to_document_freqs_.at(word).count(document_id)) {
            matched_words.push_back(word);
        }
    }

    for (const std::string& word : query.minus_words) {
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

template<typename execution_type>
SearchServer::Query_parall SearchServer::ParseQuery(const execution_type type, const std::string& text) {

    //Query_parall query;
    //for (const std::string& word : SplitIntoWords(text)) {
        //// некорректный ввод минус слов
        //if (!NegativeWordErr(word)) {
        //    throw std::invalid_argument("некорректный ввод минус слов!");
        //}
        //// Проверка на наличие спец символов в запросе
        //if (!IsValidWord(word)) {
        //    throw std::invalid_argument("документ содержит недопустимые символы!");
        //}
        //auto query_word = ParseQueryWordParall(SplitIntoWords(text));

        //if (!query_word.is_stop) {
        //    if (query_word.is_minus) {
        //        query.minus_words.insert(query_word.data);
        //    }
        //    else {
        //        query.plus_words.insert(query_word.data);
        //    }
        //}
    //}

    return ParseQueryWordParall(SplitIntoWords(text));
}
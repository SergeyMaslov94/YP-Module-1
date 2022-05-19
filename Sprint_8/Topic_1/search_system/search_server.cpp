#include "search_server.h"
#include <stdexcept>
#include <numeric>
//========================================================================================
// public
SearchServer::SearchServer()
= default;

SearchServer::SearchServer(const std::string_view & text) {
    for (const std::string_view& word : SplitIntoWordsView(text)) {
        if (!IsValidWord(word))
            throw std::invalid_argument("В стоп-словах содержатся недопустимые символы!");

        stop_words_.insert({ word.begin(), word.end() });
    }
}

SearchServer::SearchServer(const std::string & text) {
    for (const std::string& word : SplitIntoWords(text)) {
        if (!IsValidWord(word))
            throw std::invalid_argument("В стоп-словах содержатся недопустимые символы!");

        stop_words_.insert(word);
    }
}

void SearchServer::AddDocument(
    int document_id,
    const std::string_view & document,
    DocumentStatus status,
    const std::vector<int>&ratings) {

    // Проверка на отрицательный ID
    if (document_id < 0) {
        throw std::invalid_argument("документ содержит отрицательный id!");
    }
    // Проверка на наличие документов с одинаковым ID
    if (documents_.count(document_id)) {
        throw std::invalid_argument("документ с таким id уже существует!");
    }

    const std::vector<std::string_view> words = SplitIntoWordsNoStop(document);
    const double inv_word_count = 1.0 / words.size();

    for (const std::string_view word : words) {
        // Проверка на наличие спец символов в документе
        if (!IsValidWord(word)) {
            throw std::invalid_argument("документ содержит недопустимые символы!");
        }
        else {
            word_to_document_freqs_[{word.begin(), word.end()}][document_id] += inv_word_count;
            ids_and_words_with_freqs[document_id][std::string(word.begin(), word.end())] += inv_word_count;
        }
    }

    documents_.emplace(document_id, DocumentData{ ComputeAverageRating(ratings), status });
    document_ids_.insert(document_ids_.begin(), document_id);
}

std::vector<Document> SearchServer::FindTopDocuments(const std::string_view & raw_query) const {
    return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
}

std::vector<Document> SearchServer::FindTopDocuments(const std::string_view & raw_query, DocumentStatus status) const {
    return FindTopDocuments(raw_query, [status]([[maybe_unused]] int document_id, DocumentStatus document_status, [[maybe_unused]] int rating) {return ((document_status == status)); });
}

int SearchServer::GetDocumentCount() const {
    return documents_.size();
}

const std::map<std::string_view, double>& SearchServer::GetWordFrequencies(int document_id) const {
    static std::map<std::string_view, double> empty_result;
    if (ids_and_words_with_freqs.count(document_id) == 0)
        return empty_result;

    return ids_and_words_with_freqs.at(document_id);
}

void SearchServer::RemoveDocument(int document_id) {

    if (documents_.count(document_id) == 0)
        return;

    documents_.erase(document_id);
    document_ids_.erase(document_id);

    for (const auto word : GetWordFrequencies(document_id)) {
        word_to_document_freqs_.at({ word.first.begin(), word.first.end() }).erase(document_id);
    }

    ids_and_words_with_freqs.erase(document_id);
}

std::tuple<std::vector<std::string_view>, DocumentStatus> SearchServer::MatchDocument(const std::string_view & raw_query, int document_id) const {
    const Query_vector query = ParseQuerySeq(raw_query);

    for (const std::string_view& word : query.minus_words) {
        if (IsEmptyWord(word)) {
            continue;
        }

        if (word_to_document_freqs_.count({ word.begin(), word.end() }) == 0) {
            continue;
        }

        if (word_to_document_freqs_.at({ word.begin(), word.end() }).count(document_id)) {
            return { {}, documents_.at(document_id).status };
        }
    }

    std::vector<std::string_view> matched_words;

    for (const std::string_view& word : query.plus_words) {
        if (IsEmptyWord(word)) {
            continue;
        }
        if (word_to_document_freqs_.count({ word.begin(), word.end() }) == 0) {
            continue;
        }

        if (word_to_document_freqs_.at({ word.begin(), word.end() }).count(document_id)) {
            matched_words.push_back(word);
        }
    }

    {
        std::sort(matched_words.begin(), matched_words.end());
        auto last = std::unique(matched_words.begin(), matched_words.end());
        matched_words.erase(last, matched_words.end());
    }

    return { matched_words, documents_.at(document_id).status };
}

std::tuple<std::vector<std::string_view>, DocumentStatus> SearchServer::MatchDocument(
    std::execution::sequenced_policy seq,
    const std::string_view & raw_query,
    int document_id) const {

    const Query_vector query = ParseQuerySeq(raw_query);

    for (const std::string_view& word : query.minus_words) {
        if (IsEmptyWord(word)) {
            continue;
        }

        if (word_to_document_freqs_.count({ word.begin(), word.end() }) == 0) {
            continue;
        }

        if (word_to_document_freqs_.at({ word.begin(), word.end() }).count(document_id)) {
            return { {}, documents_.at(document_id).status };
        }
    }

    std::vector<std::string_view> matched_words(query.plus_words.size());
    std::transform(query.plus_words.begin(), query.plus_words.end(), matched_words.begin(), [this, document_id](std::string_view word) {
        if (word_to_document_freqs_.count({ word.begin(), word.end() }) == 0) {
            return std::string_view();
        }
        if (word_to_document_freqs_.at({ word.begin(), word.end() }).count(document_id)) {
            return word;
        }
        return std::string_view();
        }
    );

    {
        std::sort(matched_words.begin(), matched_words.end());
        auto last = std::unique(matched_words.begin(), matched_words.end());
        matched_words.erase(last, matched_words.end());
        if (!matched_words.empty() & IsEmptyWord(*matched_words.begin())) {
            matched_words.erase(matched_words.begin());
        }
    }

    return { matched_words, documents_.at(document_id).status };
}

std::tuple<std::vector<std::string_view>, DocumentStatus> SearchServer::MatchDocument(
    std::execution::parallel_policy par,
    const std::string_view & raw_query,
    int document_id) const {

    const Query_vector query = ParseQueryParall(raw_query);

    // Проверяю, содержится ли минус слова в документе с текущим id
    for (const std::string_view& word : query.minus_words) {
        if (IsEmptyWord(word)) {
            continue;
        }

        if (word_to_document_freqs_.count({ word.begin(), word.end() }) == 0) {
            continue;
        }

        if (word_to_document_freqs_.at({ word.begin(), word.end() }).count(document_id)) {
            return { {}, documents_.at(document_id).status };
        }
    }

    // Продолжаю обработку плюс слов в случае успеха
    std::vector<std::string_view> matched_words(query.plus_words.size());
    std::transform(std::execution::par, query.plus_words.begin(), query.plus_words.end(), matched_words.begin(), [this, document_id](std::string_view word) {
        if (word_to_document_freqs_.count({ word.begin(), word.end() }) == 0) {
            return std::string_view();
        }
        if (word_to_document_freqs_.at({ word.begin(), word.end() }).count(document_id)) {
            return word;
        }
        return std::string_view();
        }
    );

    {
        std::sort(std::execution::par, matched_words.begin(), matched_words.end());
        auto last = std::unique(std::execution::par, matched_words.begin(), matched_words.end());
        matched_words.erase(last, matched_words.end());
        if (!matched_words.empty() & IsEmptyWord(*matched_words.begin())) {
            matched_words.erase(matched_words.begin());
        }
    }

    return { matched_words, documents_.at(document_id).status };
}

//========================================================================================
// private
bool SearchServer::NegativeWordErr(const std::string_view & word) const {
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

bool SearchServer::IsValidWord(const std::string_view & word) {
    // A valid word must not contain special characters
    return std::none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
        });
}

bool SearchServer::IsEmptyWord(const std::string_view & word) {
    return word.empty();
}

bool SearchServer::IsStopWord(const std::string_view & word) const {
    return stop_words_.count({ word.begin(), word.end() }) > 0;
}

std::vector<std::string_view> SearchServer::SplitIntoWordsNoStop(const std::string_view & text) const {
    std::vector<std::string_view> words;
    for (const std::string_view& word : SplitIntoWordsView(text)) {
        if (!IsStopWord(word)) {
            words.push_back(word);
        }
    }
    return words;
}

int SearchServer::ComputeAverageRating(const std::vector<int>&ratings) {
    if (ratings.empty()) {
        return 0;
    }

    int rating_sum = std::accumulate(ratings.begin(), ratings.end(), 0);
    return rating_sum / static_cast<int>(ratings.size());
}

SearchServer::QueryWord SearchServer::ParseQueryWord(std::string_view text) const {
    bool is_minus = false;
    if (text[0] == '-') {
        is_minus = true;
        text = text.substr(1);
    }
    return { text, is_minus, IsStopWord(text) };
}

SearchServer::Query_vector SearchServer::ParseQueryWordSeq(std::vector<std::string_view> texts) const {
    Query_vector query;

    query.minus_words.resize(texts.size());
    query.plus_words.resize(texts.size());

    std::transform(texts.begin(), texts.end(), query.plus_words.begin(), [this](std::string_view& text) {
        if (!IsStopWord(text) && text[0] != '-') {
            return text;
        }
        return std::string_view();
        });

    std::transform(texts.begin(), texts.end(), query.minus_words.begin(), [this](std::string_view& text) {
        if (!IsStopWord(text) && text[0] == '-') {
            return text.substr(1);
        }
        return std::string_view();
        });

    return query;
}

SearchServer::Query_vector SearchServer::ParseQueryWordParall(std::vector<std::string_view> texts) const {
    Query_vector query;

    query.minus_words.resize(texts.size());
    query.plus_words.resize(texts.size());

    std::transform(std::execution::par, texts.begin(), texts.end(), query.plus_words.begin(), [this](std::string_view& text) {
        if (!IsStopWord(text) && text[0] != '-') {
            return text;
        }
        return std::string_view();
        });

    std::transform(std::execution::par, texts.begin(), texts.end(), query.minus_words.begin(), [this](std::string_view& text) {
        if (!IsStopWord(text) && text[0] == '-') {
            return text.substr(1);
        }
        return std::string_view();
        });

    return query;
}

SearchServer::Query SearchServer::ParseQuery(const std::string_view & text) const {
    Query query;
    for (const std::string_view& word : SplitIntoWordsView(text)) {
        const QueryWord query_word = ParseQueryWord(word);

        if (!query_word.is_stop) {
            if (query_word.is_minus) {
                query.minus_words.insert(query_word.data);
            }
            else {
                query.plus_words.insert(query_word.data);
            }
        }
    }
    return query;
}

SearchServer::Query_vector SearchServer::ParseQuerySeq(const std::string_view & text) const {
    return ParseQueryWordSeq(SplitIntoWordsView(text));;
}

SearchServer::Query_vector SearchServer::ParseQueryParall(const std::string_view & text) const {
    return ParseQueryWordParall(SplitIntoWordsView(text));
}

double SearchServer::ComputeWordInverseDocumentFreq(const std::string & word) const {
    return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
}

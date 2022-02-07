// Обработка ошибок/исключений в поисковой системе
/*
 * Задание
Это задание — итоговый проект третьего спринта. Вы будете сдавать его на проверку через репозиторий на GitHub.
Доработайте класс SearchServer. Примените механизм исключений и реализуйте в SearchServer обработку проблем.

Конструкторы класса SearchServer должны выбрасывать исключение invalid_argument, если любое из переданных стоп-слов
содержит недопустимые символы, то есть символы с кодами от 0 до 31. Такого требования не было в предыдущих заданиях,
так как известные вам на тот момент способы обработки ошибок не позволяли эффективно решить эту задачу в конструкторе.

Метод AddDocument больше не должен использовать возврат значения типа bool для сообщения об успехе или ошибке.
Вместо этого он должен выбрасывать исключение invalid_argument в следующих ситуациях:
Попытка добавить документ с отрицательным id;
Попытка добавить документ c id ранее добавленного документа;
Наличие недопустимых символов (с кодами от 0 до 31) в тексте добавляемого документа.

Методы FindDocument вместо возврата optional<vector<Document>> должны возвращать vector<Document> и выбрасывать
исключение invalid_argument в следующих ситуациях:
В словах поискового запроса есть недопустимые символы с кодами от 0 до 31;
Наличие более чем одного минуса перед словами, которых не должно быть в искомых документах, например, пушистый --кот.
В середине слов минусы разрешаются, например: иван-чай.
Отсутствие текста после символа «минус»: в поисковом запросе: "пушистый -".

Метод MatchDocument должен возвращать tuple<vector<string>, DocumentStatus>, выбрасывая исключение invalid_argument
в тех же ситуациях, что и метод FindDocument.

Метод GetDocumentId должен выбрасывать исключение out_of_range, если индекс переданного документа выходит
за пределы допустимого диапазона (0; количество документов).
*/

#include <algorithm>
#include <cassert>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;
const double MEASURE = 1e-6;

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

// -------------------------- Класс поисковой системы -------------------------------
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

int top_3_lesson_6(){
    try {
        SearchServer search_server("и в на"s);

        AddDocument(search_server, 1, "пушистый кот пушистый хвост"s, DocumentStatus::ACTUAL, {7, 2, 7});
        AddDocument(search_server, 1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2});
        AddDocument(search_server, -1, "пушистый пёс и модный ошейник"s, DocumentStatus::ACTUAL, {1, 2});
        AddDocument(search_server, 3, "большой пёс скво\x12рец евгений"s, DocumentStatus::ACTUAL, {1, 3, 2});
        AddDocument(search_server, 4, "большой пёс скворец евгений"s, DocumentStatus::ACTUAL, {1, 1, 1});

        FindTopDocuments(search_server, "пушистый -пёс"s);
        FindTopDocuments(search_server, "пушистый --кот"s);
        FindTopDocuments(search_server, "пушистый -"s);

        MatchDocuments(search_server, "пушистый пёс"s);
        MatchDocuments(search_server, "модный -кот"s);
        MatchDocuments(search_server, "модный --пёс"s);
        MatchDocuments(search_server, "пушистый - хвост"s);

        auto id = search_server.GetDocumentId(2);

    } catch (const exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}
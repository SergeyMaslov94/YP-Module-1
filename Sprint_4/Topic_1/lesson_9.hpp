//============================================================================
// ���� 9. ������� ���������� ������ ����������

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

//========================================================
// ������������� �������
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

// �������� ����������� ����� ����� ����
bool NegativeWordErr(const string& word) {
    // ���� ����� ������� '�����' ����������� �����
    if (word.size() == 1) {
        if (word[0] == '-') {
            return false;
        }
    }
    // ���� � ������� ����� ������ ������ ����� �����-�������
    if (word.size() > 1) {
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
// ����� ��������� �������
//========================================================
class SearchServer {
public:
    // �����������
    explicit SearchServer()
        = default;

    explicit SearchServer(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            if (IsValidWord(word)) {
                stop_words_.insert(word);
            }
            else {
                throw invalid_argument("� ����-������ ���������� ������������ �������!");
            }
        }
    }

    template <typename  StringCollection>
    explicit SearchServer(const StringCollection& collection) {
        for (const string& text : collection) {
            for (const string& word : SplitIntoWords(text)) {
                if (IsValidWord(word)) {
                    stop_words_.insert(word);
                }
                else {
                    throw invalid_argument("Stop-word contains invalid characters");
                }
            }
        }
    }

    // ��������� � ������� ����� ��������
    void AddDocument(int document_id, const string& document, DocumentStatus status, const vector<int>& ratings) {
        // �������� �� ������������� ID
        if (document_id < 0) {
            throw invalid_argument("�������� �������� ������������� id!");
        }
        // �������� �� ������� ���������� � ���������� ID
        if (documents_.count(document_id)) {
            throw invalid_argument("�������� � ����� id ��� ����������!");
        }

        const vector<string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();

        for (const string& word : words) {
            // �������� �� ������� ���� �������� � ���������
            if (!IsValidWord(word)) {
                throw invalid_argument("�������� �������� ������������ �������!");
            }
            else {
                word_to_document_freqs_[word][document_id] += inv_word_count;
            }
        }

        documents_.emplace(document_id, DocumentData{ ComputeAverageRating(ratings), status });
        document_ids_.push_back(document_id);
    }

    // ������� FindTopDocuments � ������� �� ����������� ������, � ��������� ����������� �� ��������� (DocumentStatus::ACTIVE)
    vector<Document> FindTopDocuments(const string& raw_query) const {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
    }

    // ������� FindTopDocuments � ������� ��������� ����������� �� �������
    vector<Document> FindTopDocuments(const string& raw_query, DocumentStatus status) const {
        return FindTopDocuments(raw_query, [status]([[maybe_unused]] int document_id, DocumentStatus document_status, [[maybe_unused]] int rating) {return ((document_status == status)); });
    }

    // ������� FindTopDocuments, � ������� �������� �������-�������� key_filter �� �������� ������� ����� ������������� ���������
    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string& raw_query, DocumentPredicate document_predicate) const {
        // ����� � ���� �����
        const Query query = ParseQuery(raw_query);
        //  ID - ������������� - �������
        auto matched_documents = FindAllDocuments(query, document_predicate);
        // ��������� �� �������������. ���� ������������� ����������, ��������� �� ��������
        sort(matched_documents.begin(), matched_documents.end(), [](const Document& lhs, const Document& rhs) {
            if (abs(lhs.relevance - rhs.relevance) < MEASURE) {
                return lhs.rating > rhs.rating;
            }
            else {
                return lhs.relevance > rhs.relevance;
            }
            });
        // ���������� ������ ������ MAX_RESULT_DOCUMENT_COUNT ����������
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

    int GetDocumentCount() const {
        return documents_.size();
    }

    // ����� ����������� �������� ������������� ��������� �� ��� ����������� ������
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
        return { text, is_minus,IsStopWord(text) };
    }

    struct Query {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWords(text)) {
            // ������������ ���� ����� ����
            if (!NegativeWordErr(word)) {
                throw invalid_argument("������������ ���� ����� ����!");
            }
            // �������� �� ������� ���� �������� � �������
            if (!IsValidWord(word)) {
                throw invalid_argument("�������� �������� ������������ �������!");
            }
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

    // ������� ���� ���������� � ������� ����������� ����� �� �������
    double ComputeWordInverseDocumentFreq(const string& word) const {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query& query, DocumentPredicate document_predicate) const {
        map<int, double> document_to_relevance;
        // ����������� ������������� ��� ������� ���������, ��� ����������� ����� �� �������
        for (const string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);

            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word)) {
                // ��������������� ��������� �� �������� �������� � ������� key_filter
                if (document_predicate(document_id, documents_.at(document_id).status, documents_.at(document_id).rating)) {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }
        // ������� ������������� ����������, ������� �������� ����� �����
        for (const string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word)) {
                document_to_relevance.erase(document_id);
            }
        }

        // ��������� ���������: ID - ������������� - �������
        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance) {
            matched_documents.push_back({ document_id, relevance, documents_.at(document_id).rating });
        }
        return matched_documents;
    }
};

//==============================================================
/* ����� ��� �������� ���� ����������.������������ ���
   ���������� ���������� �� ���������� �� ��������� �������� 
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

    Iterator begin() const{
        return start_;
    }

    Iterator end() const{
        return stop_;
    }

    size_t size() const{
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

// ���������� ��������� ������ ��� Document
ostream& operator<<(ostream& out, const Document& document) {
    out << "{ "s
        << "document_id = "s << document.id << ", "s
        << "relevance = "s << document.relevance << ", "s
        << "rating = "s << document.rating << " }"s;
    return out;
}

void PrintDocument(const Document& document) {
    cout << "{ "s
        << "document_id = "s << document.id << ", "s
        << "relevance = "s << document.relevance << ", "s
        << "rating = "s << document.rating << " }"s << endl;
}

//==============================================================
/* ����� ��������� ��������� �� ���������� �� ��������� �������� 
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

int top_1_lesson_9() {
    SearchServer search_server("and with"s);

    search_server.AddDocument(1, "funny pet and nasty rat"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
    search_server.AddDocument(2, "funny pet with curly hair"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
    search_server.AddDocument(3, "big cat nasty hair"s, DocumentStatus::ACTUAL, { 1, 2, 8 });
    search_server.AddDocument(4, "big dog cat Vladislav"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
    search_server.AddDocument(5, "big dog hamster Borya"s, DocumentStatus::ACTUAL, { 1, 1, 1 });

    const auto search_results = search_server.FindTopDocuments("curly dog"s);

    int page_size = 2;
    const auto pages = Paginate(search_results, page_size);

    for (auto page = pages.begin(); page != pages.end(); ++page) {
        cout << *page << endl;
        cout << "Page break"s << endl;
    }

    return 0;
}
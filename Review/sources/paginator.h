#pragma once
//==============================================================
/* Класс для хранения пары итераторов.Используется для
   разделения документов из результата на отдельные страницы
 */
//==============================================================
using namespace std;

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
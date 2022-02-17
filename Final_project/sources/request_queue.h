#pragma once
//==============================================================
/* Класс определяет очередь запросов и  является "оберткой" для методов поиска

Метод GetNoResultRequests возвращает сколько за
последние сутки (min_in_day_) было запросов, на которые
ничего не нашлось.
 */
//==============================================================
#include <string>
#include <deque>
#include "search_server.h"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server):
            search_server_(search_server),
            min_pass_(0),
            count_no_result_(0)
    {}

    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        return AddRequest(search_server_.FindTopDocuments(raw_query, document_predicate));
    }

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);

    std::vector<Document> AddFindRequest(const std::string& raw_query);

    int GetNoResultRequests() const;

private:
    const SearchServer& search_server_;
    const static int min_in_day_ = 1440;

    struct QueryResult {
        std::vector<Document> search_results;
        bool is_empty_result = false;
    };

    std::deque<QueryResult> requests_;
    int min_pass_;
    int count_no_result_;

    // метод добавляет результаты поиска в очередь хранения
    std::vector<Document> AddRequest(std::vector<Document> search_results);
};
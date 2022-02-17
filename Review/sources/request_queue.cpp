#include "request_queue.h"
//===================================================================================================
// public
std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
    return AddRequest(search_server_.FindTopDocuments(raw_query, status));
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    return AddRequest(search_server_.FindTopDocuments(raw_query));
}

int RequestQueue::GetNoResultRequests() const {
    return count_no_result_;
}
//===================================================================================================
// private
std::vector<Document> RequestQueue::AddRequest(std::vector<Document> search_results) {
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
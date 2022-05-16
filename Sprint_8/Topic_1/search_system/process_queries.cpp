#include "process_queries.h"

using namespace std;

std::vector<std::vector<Document>> ProcessQueries(
    const SearchServer& search_server,
    const std::vector<std::string>& queries) {

    if (queries.empty()) {
        return {};
    }

    std::vector<std::vector<Document>> documents_lists(queries.size());
    std::transform(std::execution::par, queries.begin(), queries.end(), documents_lists.begin(), 
        [&search_server](std::string query) {return search_server.FindTopDocuments(query); });

    return documents_lists;
}

std::list<Document> ProcessQueriesJoined(
    const SearchServer& search_server,
    const std::vector<std::string>& queries) {
    std::list<Document> long_documents_lists;

    for (const auto& local_documents : ProcessQueries(search_server, queries)) {
        for (const auto& document : local_documents) {
            long_documents_lists.push_back(document);
        }
    }
    return long_documents_lists;
}
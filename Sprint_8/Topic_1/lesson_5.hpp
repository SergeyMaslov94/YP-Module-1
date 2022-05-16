#include "search_system/search_server.h"
#include "benchmark.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <execution>

using namespace std;
//=======================================================================
std::vector<std::vector<Document>> ProcessQueriesSlow(
    const SearchServer& search_server,
    const std::vector<std::string>& queries) {
    std::vector<std::vector<Document>> documents_lists;

    for (const std::string& query : queries) {
        documents_lists.push_back(search_server.FindTopDocuments(query));
    }
    
    return documents_lists;
};
//=======================================================================
std::vector<std::vector<Document>> ProcessQueriesFast(
    const SearchServer& search_server,
    const std::vector<std::string>& queries) {

    std::vector<std::vector<Document>> documents_lists(queries.size());
    std::transform(std::execution::par, queries.begin(), queries.end(), documents_lists.begin(), [&search_server](std::string query) {return search_server.FindTopDocuments(query);});

    return documents_lists;
};
//=======================================================================
void debug_test() {
    SearchServer search_server("and with"s);

    int id = 0;
    for (
        const string& text : {
            "funny pet and nasty rat"s,
            "funny pet with curly hair"s,
            "funny pet and not very nasty rat"s,
            "pet with rat and rat and rat"s,
            "nasty rat with curly hair"s,
        }
        ) {
        search_server.AddDocument(++id, text, DocumentStatus::ACTUAL, { 1, 2 });
    }

    const vector<string> queries = {
        "nasty rat -not"s,
        "not very funny nasty pet"s,
        "curly hair"s
    };

    // последовательный алгоритм
    id = 0;
    for (
        const auto& documents : ProcessQueriesSlow(search_server, queries)
        ) {
        cout << documents.size() << " documents for query ["s << queries[id++] << "]"s << endl;
    }

    // параллельный алгоритм
    id = 0;
    for (
        const auto& documents : ProcessQueriesFast(search_server, queries)
        ) {
        cout << documents.size() << " documents for query ["s << queries[id++] << "]"s << endl;
    }
}

int lesson_5() {
    
    debug_test();

    benchmark();

    return 0;
}
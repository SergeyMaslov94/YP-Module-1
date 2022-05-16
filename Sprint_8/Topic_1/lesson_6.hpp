#include "search_system/search_server.h"
#include "search_system/process_queries.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <execution>
#include <list>

using namespace std;

int lesson_6() {
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

    for (const Document& document : ProcessQueriesJoined(search_server, queries)) {
        cout << "Document "s << document.id << " matched with relevance "s << document.relevance << endl;
    }

    return 0;
}
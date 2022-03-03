#include "document.h"
#include <iostream>

using namespace std;

Document::Document() :
        id(0),
        relevance(0.0),
        rating(0)
{}

Document::Document(int id_m, double relevance_m, int rating_m) {
    id = id_m;
    relevance = relevance_m;
    rating = rating_m;
}

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
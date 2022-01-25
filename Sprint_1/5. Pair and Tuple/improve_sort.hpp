#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Document {
    int id;
    double relevance;
    int rating;
};

void SortDocuments(vector<Document>& matched_documents) {
    sort(matched_documents.begin(), matched_documents.end(),
        
        /* Компоратор с логическими операциями */
        //[](const Document& lhs, const Document& rhs) {
        //    return (lhs.rating > rhs.rating) || lhs.rating == rhs.rating && (lhs.relevance > rhs.relevance);
        //});

        /* Компоратор с pair */
        [](const Document& lhs, const Document& rhs) {
            return(pair(lhs.rating, lhs.relevance) > pair(rhs.rating, rhs.relevance));
        });
}

int run() {
    vector<Document> documents = {
        {100, 0.5, 4},
        {101, 1.2, 4},
        {102, 0.3, 5}
    };
    SortDocuments(documents);
    for (const Document& document : documents) {
        cout << document.id << ' '
            << document.relevance << ' '
            << document.rating << endl;
    }

    return 0;
}
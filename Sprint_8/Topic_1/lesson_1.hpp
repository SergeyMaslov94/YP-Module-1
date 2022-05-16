#include <algorithm>
#include <utility>
#include <execution>
#include <vector>

#include "log_duration.hpp"

/*
Писать качественный и эффективный параллельный код непросто. Но авторы алгоритмов стандартной 
библиотеки C++ сделали это за вас! Достаточно подключить библиотеку <execution> и передать 
std::execution::par в качестве первого аргумента в нужный алгоритм.
*/

/*
Чтобы явно обозначить, что вам нужна последовательная — то есть 
непараллельная — реализация, вместо execution::par передайте execution::seq.
*/

using namespace std;

struct Document {
    int id;
    double relevance;
    int rating;
};

void SortDocuments(vector<Document>& documents) {
    LOG_DURATION("default");
    sort(documents.begin(), documents.end(),
        [](const Document& lhs, const Document& rhs) {
            return pair(lhs.rating, lhs.relevance)
                  > pair(rhs.rating, rhs.relevance);
        });
}

void SortDocumentsParall(vector<Document>& documents) {
    LOG_DURATION("parallel");
    sort(execution::par,  // 
        documents.begin(), documents.end(),
        [](const Document& lhs, const Document& rhs) {
            return pair(lhs.rating, lhs.relevance)
                  > pair(rhs.rating, rhs.relevance);
        });
}

int lesson_1() {

    std::vector<Document> documents;
    size_t num = 10000;

    for (auto i = 0; i < num; i++) {
        documents.push_back({i, 0, 0});
    }

    SortDocuments(documents);
    SortDocumentsParall(documents);

    return 0;
}
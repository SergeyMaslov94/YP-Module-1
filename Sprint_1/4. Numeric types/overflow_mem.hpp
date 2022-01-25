#include <cstdint>
#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>


using namespace std;

int run(){
    const auto min = numeric_limits<int64_t>::min();
    const auto max = numeric_limits<uint64_t>::max();

    // выведите 5 чисел
    cout << min << endl;
    cout << max << endl;
    cout << min+max << endl;
    cout << 2 * min << endl;
    cout << 2 * max << endl;

    return 0;
}

//=====================================================
// Техника безопасности


using namespace std;

struct Document {
    int id;
    int rating;
};

void PrintDocuments(vector<Document> documents, size_t skip_start, size_t skip_finish) 
{
    if (documents.empty())
        return;
    
    if (skip_start >= documents.size())
        return;

    sort(documents.begin(), documents.end(), [](Document lhs, Document rhs) {return lhs.rating > rhs.rating; });

    // отсортировать документы и вывести не все
    for (size_t i = skip_start; i + skip_finish < documents.size(); ++i)
    {
        auto document = documents[i];
        cout << "{ id = " << document.id << ", rating = " << document.rating << " }" << endl;
    }
}

int run_2() {
    PrintDocuments(
        {
            {100, 5},
            {101, 7},
            {102, -4},
            {103, 9},
            {104, 1}
        },
        1,
        2
    );
    return 0;
}

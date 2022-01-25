#include <iostream>

using namespace std;

class SearchServer {
public:
    enum class DocumentStatus {
        ACTUAL,
        IRRELEVANT,
        BANNED,
        REMOVED,
    };
};

int run() {
    // выведите численное значение статуса BANNED
    std::cout << static_cast<int>(SearchServer::DocumentStatus::BANNED) << endl;
    return 0;
}
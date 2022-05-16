//===========================================================================
// Функция распараллеливает обработку нескольких запросов к поисковой системе
//===========================================================================

#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <execution>
#include "document.h"
#include "search_server.h"

std::vector<std::vector<Document>> ProcessQueries(
    const SearchServer& search_server,
    const std::vector<std::string>& queries);

std::list<Document> ProcessQueriesJoined(
    const SearchServer& search_server,
    const std::vector<std::string>& queries);
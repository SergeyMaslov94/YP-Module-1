// СПРИНТ 3. ИТОГОВЫЙ ПРОЕКТ. ОБРАБОТКА ОШИБОК(ИСКЛЮЧЕНИЙ) В ПОИСКОВОЙ СИСТЕМЕ
//============================================================================
#include <iostream>
#include "sources/search_server.h"
#include "sources/unit_tests.h"
#include "sources/request_queue.h"
#include "sources/paginator.h"
#include "sources/document.h"
#include "sources/log_duration.h"

using namespace std;

//==============================================================
int main()
{
    // ТЕСТИРОВАНИЕ
    //UnitTestsSearchServer tests;
    //tests.TestSearchServer();

    // Если вы видите эту строку, значит все тесты прошли успешно
    //cout << "Search server testing finished"s << endl;

    // ОТЛАДКА
    try {
        SearchServer search_server("and with"s);
        RequestQueue request_queue(search_server);

        search_server.AddDocument(1, "funny pet and nasty rat"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
        search_server.AddDocument(2, "funny pet with curly hair"s, DocumentStatus::ACTUAL, { 1, 2, 3 });
        search_server.AddDocument(3, "big cat nasty hair"s, DocumentStatus::ACTUAL, { 1, 2, 8 });
        search_server.AddDocument(4, "big dog cat Vladislav"s, DocumentStatus::ACTUAL, { 1, 3, 2 });
        search_server.AddDocument(5, "big dog hamster Borya"s, DocumentStatus::ACTUAL, { 1, 1, 1 });

        // ДИАГНОСТИКА ПРОФИЛИРОВЩИКОМ
        {   //  метода MatchDocuments
            std::string querry = "big dog";
            std::cout << "Matching dokumentov po zaprosu: " << querry <<std::endl;

            LOG_DURATION("Operation time", std::cerr);
            for (auto document_id = 1; document_id <= 5;) {
                auto [matched_words, status] = search_server.MatchDocument(querry, document_id);
                PrintMatchDocumentResult(document_id, matched_words, status);
                document_id++;
            }
        }
        
        {   // метода FindTopDocuments
            std::string querry = "big dog";
            std::cout << "Rezultaty poiska po zaprosu: " << querry << std::endl;

            LOG_DURATION("Operation time", std::cout);
            auto top_documents = search_server.FindTopDocuments(querry);
            for (const auto& document : top_documents)
                PrintDocument(document);
        }

        // Определяем, сколько запросов остались без ответа
        const auto search_results = request_queue.AddFindRequest("curly dog"s);
        cout << "Всего запросов, оставшихся без ответа: "s << request_queue.GetNoResultRequests() << endl;

        // Разделяем вывод результатов поиска на отдельные страницы
        int page_size = 2;
        const auto pages = Paginate(search_results, page_size);

        for (auto page = pages.begin(); page != pages.end(); ++page) {
            cout << *page << endl;
            cout << "Page break"s << endl;
        }

    } catch (const exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}
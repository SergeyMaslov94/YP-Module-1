//
// Created by Home on 05.03.2022.
//
#include "test_example_functions.h"
#include "search_server.h"
#include "remove_duplicates.h"
#include <iostream>
#include <cassert>

using namespace std;

// Тест метода для получения частот слов по id документа
void TestGetWordFrequencies() {
    // документ существует
    {
        const map<std::string_view, double> reference{{"funny", 0.25}, {"pet", 0.25}, {"nasty", 0.25}, {"rat", 0.25} };

        SearchServer search_server("and with"s);
        search_server.AddDocument(1, "funny pet and nasty rat"s, DocumentStatus::ACTUAL, {7,1,7});
        auto result = search_server.GetWordFrequencies(1);
        assert(result == reference);
     }

     // документ не найден
    {
        SearchServer search_server("and with"s);
        search_server.AddDocument(1, "funny pet and nasty rat"s, DocumentStatus::ACTUAL, {7,1,7});
        auto result = search_server.GetWordFrequencies(0);
        assert(result.empty());
    }
}
// Тест метода для удаления дубликатов
void TestRemoveDuplicates() {
    // дубликаты есть
    {
        SearchServer search_server("and with"s);
        search_server.AddDocument(1, "funny pet and nasty rat"s, DocumentStatus::ACTUAL, {7,1,7});
        // повторы
        search_server.AddDocument(2, "funny funny pet pet and nasty rat rat"s, DocumentStatus::ACTUAL, {1, 2});
        // изменен порядок
        search_server.AddDocument(3, "pet funny and rat nasty"s, DocumentStatus::ACTUAL, {1, 2});
        RemoveDuplicates(search_server);

        assert(search_server.GetDocumentCount() == 1);
    }

    // дупликатов нет
    {
        SearchServer search_server("and with"s);
        search_server.AddDocument(1, "funny pet and nasty rat"s, DocumentStatus::ACTUAL, {7,1,7});
        search_server.AddDocument(2, "funny pet rat"s, DocumentStatus::ACTUAL, {7,2});
        RemoveDuplicates(search_server);

        assert(search_server.GetDocumentCount() == 2);
    }
}

void TestExampleFunctions() {
    TestGetWordFrequencies();
    TestRemoveDuplicates();
}

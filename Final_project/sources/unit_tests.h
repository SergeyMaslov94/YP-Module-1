#pragma once
//========================================================
// Фреймворк и модульные тесты поисковой системы
//========================================================
#include <string>
#include <iostream>

using namespace std;

class UnitTestsSearchServer {
public:
    // Функция TestSearchServer является точкой входа для запуска тестов
    void TestSearchServer();

private:
    // Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
    void TestExcludeStopWordsFromAddedDocumentContent();

    // Тест на добавление документов
    void TestSearchAddDocuments();

    // Стоп-слова исключаются из текста документов
    void TestNoStopWords();

    // Поддержка минус - слов. Документы, содержащие минус - слова поискового запроса, не должны включаться в результаты поиска.
    void TestSupportMinusWords();

    // При матчинге документа по поисковому запросу должны быть возвращены все слова из поискового запроса, присутствующие в документе
    void TestDocumentMatching();

    // Сортировка найденных документов по релевантности
    void TestSortByRelevance();

    // Рейтинг добавленного документа равен среднему арифметическому оценок документа
    void TestCalculateRatings();

    // Фильтрация результатов поиска с использованием предиката, задаваемого пользователем
    void TestFilteringUsingPredictFunction();

    // Поиск документов, имеющих заданный статус
    void TestSearchDocumentsByStatus();

    // Корректное вычисление релевантности найденных документов
    void TestCorrectCalcRelevance();

    // Посик по пустому запросу
    void TestEmptyContent();

    // Введенные плюс и минус слова одинаковы
    void TestSamePlusMinusWord();
};

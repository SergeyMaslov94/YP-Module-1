#pragma once
//========================================================================================
#include <string>
#include <vector>

// Структура для хранения документов
struct Document {
    Document();
    Document(int id_m, double relevance_m, int rating_m);

    int id;
    double relevance;
    int rating;
};

// Статус документов
enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

// Вывод информации по документу
void PrintDocument(const Document& document);
void PrintMatchDocumentResult(int document_id, const std::vector<std::string>& words, DocumentStatus status);
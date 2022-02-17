#pragma once
//========================================================================================
// Структура для хранения документов
struct Document {
    Document() :
            id(0),
            relevance(0.0),
            rating(0)
    {}

    Document(int id_m, double relevance_m, int rating_m) {
        id = id_m;
        relevance = relevance_m;
        rating = rating_m;
    }

    int id;
    double relevance;
    int rating;
};

//========================================================================================
// Статус документов
enum class DocumentStatus {
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};
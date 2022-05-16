#pragma once
//========================================================
// Обертки для отладки и чтение
//========================================================
#include "search_server.h"

// Читаем строку
std::string ReadLine();

// Читаем цифру
int ReadLineWithNumber();

void AddDocument(SearchServer& search_server, int document_id, const std::string& document, DocumentStatus status, const std::vector<int>& ratings);
void FindTopDocuments(const SearchServer& search_server, const std::string& raw_query);
void MatchDocuments(const SearchServer& search_server, const std::string& query);

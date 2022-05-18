#include "remove_duplicates.h"
#include <iostream>

void RemoveDuplicates(SearchServer& search_server) {

    std::set<std::set<std::string_view>> documents;
    std::vector<int> ids_duplicate;
    int previous_size = 0;

    for (const auto& document_id : search_server) {
        // не учитываем частоты
        std::set<std::string_view> words;
        for(auto word_and_freqs : search_server.GetWordFrequencies(document_id)) {
            words.insert(words.begin(), word_and_freqs.first);
        }
        documents.insert(documents.begin(), words);

        if(previous_size == documents.size()) {
            ids_duplicate.push_back(document_id);
            continue;
        }
        previous_size++;
    }

    // удаляем дубликаты
    for(const auto& id : ids_duplicate) {
        std::cout << "Found duplicate document id " << id << std::endl;
        search_server.RemoveDocument(id);
    }
}


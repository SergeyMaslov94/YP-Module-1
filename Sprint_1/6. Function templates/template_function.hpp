#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

template <typename Term>
map<Term, int> ComputeTermFreqs(const vector<Term>& terms) {
    map<Term, int> term_freqs;
    for (const Term& term : terms) {
        ++term_freqs[term];
    }
    return term_freqs;
}

pair<string, int> FindMaxFreqAnimal(const vector<pair<string, int>>& animals) {
    // верните животного с максимальной частотой
    map<pair<string, int>, int> term_freqs = ComputeTermFreqs(animals);

    int max_value = term_freqs.at(animals[0]);
    int min_value = animals[0].second;

    pair<string, int> max_term = animals[0];

    for (auto [term, freq] : term_freqs)
    {
       
        if (freq == max_value)
        {
            if (term.second > min_value)
            {
                min_value = term.second;
                max_term = term;
            }
        }

        if (freq > max_value)
        {
            max_value = term_freqs.at(term);
            max_term = term;
        }
    }

    
    return max_term;
}

int run() {
    const vector<pair<string, int>> animals = {
        {"Murka"s, 5},
        {"Belka"s, 6},
        {"Murka"s, 7},
        {"Murka"s, 5},
        {"Belka"s, 6},
    };

    const pair<string, int> max_freq_animal = FindMaxFreqAnimal(animals);
    
    cout << max_freq_animal.first << " "s
        << max_freq_animal.second << endl;

    return 0;
}
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <set>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include "../log_duration.hpp"


using namespace std;

struct Cat {
    string breed;
    string name;
    int age;
};

ostream& operator<<(ostream& os, const Cat& cat) {
    return os << '{' << cat.breed << ", "s << cat.name << ", "s << cat.age << '}';
}

// Компаратор, сравнивающий породы кошек
struct CatBreedComparator {
    // Помечаем компаратор как «прозрачный», чтобы с его помощью можно было сравнивать
    // не только кошек с кошками, но и со строками, задающими породу кошек
    using is_transparent = std::true_type;

    bool operator()(const Cat& lhs, const Cat& rhs) const {
        return lhs.breed < rhs.breed;
    }
    bool operator()(const Cat& lhs, const string& rhs_breed) const {
        return lhs.breed < rhs_breed;
    }
    bool operator()(const string& lhs_breed, const Cat& rhs) const {
        return lhs_breed < rhs.breed;
    }
};

void topic_1()
{
    // Множество кошек. Кошки считаются эквивалентными, если их породы совпадают.
    // В такой контейнер не получится поместить двух кошек одинаковой породы.
    set<Cat, CatBreedComparator> cats;

    cats.insert({ "siamese"s, "Tom"s, 3 });
    cats.insert({ "bengal"s, "Leo"s, 2 });
    cats.insert({ "birman"s, "Tiger"s, 5 });
    // Ginger вставлен не будет, так как множество уже содержит кошку сиамской породы
    cats.insert({ "siamese"s, "Ginger"s, 2 });

    // Выводим кошек в стандартный вывод, по одной на каждой строке
    copy(cats.begin(), cats.end(), ostream_iterator<Cat>(cout, "\n"));

    // Кошка, эквивалентная Мурке бенгальской породы, во множестве cats есть - это Leo
    assert(cats.count({ "bengal"s, "Murka"s, 6 }) == 1);

    // Компаратор CatBreedComparator позволяет искать кошек, указывая их породу
    if (auto it = cats.find("siamese"s); it != cats.end()) {
        cout << "Found cat: "s << *it << endl; // Выведет {siamese, Tom, 3}
    }
}

std::vector<pair<string, int>> GetSortedWordCounts(vector<string> words) {
    unordered_map<string, int> counts_map;

    {
        LOG_DURATION("Fillings"s);

        for (auto& word : words) {
            ++counts_map[move(word)];
        }
    }

    std::cout << "Содержимое const_map:" << std::endl;
    // выводим первые 10 слов
    for (auto [iter, i] = tuple(counts_map.begin(), 0); i < 10 && iter != counts_map.end(); ++i, ++iter) {
        cout << iter->first << " - "s << iter->second << endl;
    }

    {
        LOG_DURATION("Moving & sorting"s);

        vector<pair<string, int>> counts_vector(move_iterator(counts_map.begin()), move_iterator(counts_map.end()));

        sort(counts_vector.begin(), counts_vector.end(), [](const auto& l, const auto& r) {
            return l.second > r.second;
            });

        return counts_vector;
    }
}

void topic_2()
{
    vector<string> words;
    string word;

    fstream myfile("D:/Projects/Yandex_Practicum/Yandex-Practicum/Sprint_9/canterbury.txt");
    if (myfile.is_open())
    {
        while(myfile >> word) {  
            words.push_back(word);
        }
        myfile.close();
    }

    auto counts_vector = GetSortedWordCounts(move(words));

}

//=======================================================================================
int lesson_1() {

    //topic_1();
    topic_2();

    return 0;
}
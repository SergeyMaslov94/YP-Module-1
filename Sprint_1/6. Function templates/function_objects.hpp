// Шаблоны Функций: Специализация шаблонов

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

struct Animal {
    string name;
    int age;
    double weight;
};

enum class AnimalSortKey {
    AGE,     // по полю age
    WEIGHT,  // по полю weight
    RELATIVE_WEIGHT  // по weight / age
};

// Версия функции SortBy для "любых" аргументов 
template <typename Container, typename KeyMapper>
void SortBy(Container& container, KeyMapper key_mapper, bool reverse = false) {
    // теперь можно сортировать контейнер
    // с компаратором key_mapper(lhs) < key_mapper(rhs)
    sort(container.begin(), container.end(), [key_mapper, reverse](const auto& lhs, const auto& rhs)
    { 
        if(reverse)
            return key_mapper(lhs) > key_mapper(rhs);
            
        return key_mapper(lhs) < key_mapper(rhs);
    });

}

// Конкретная версия функции SortBy для конкретных аргументов
void SortBy(vector <Animal>& animals, AnimalSortKey sort_key, bool reverse = false)
{
    switch (sort_key) {

    case AnimalSortKey::AGE:
        return SortBy(animals, [](const auto& x) { return x.age;  }, reverse);
    
    case AnimalSortKey::WEIGHT:
        return SortBy(animals, [](const auto& x) { return x.weight; }, reverse);
    
    case AnimalSortKey::RELATIVE_WEIGHT:
        return SortBy(animals, [](const auto& x) { return x.weight / x.age; }, reverse);
    }
}

void PrintNames(const vector<Animal>& animals) 
{
    for (const Animal& animal : animals) 
    {
        cout << animal.name << ' ';
    }
    
    cout << endl;
}

int function_objects() 
{

    vector<Animal> animals = 
    {
        {"Murka"s,   10, 5},
        {"Belka"s,   5,  1.5},
        {"Georgiy"s, 2,  4.5},
        {"Rurik"s,   12, 3.1},
    };

    PrintNames(animals);

    SortBy(animals, [](const Animal& animal) { return animal.name; }, true);
    PrintNames(animals);

    SortBy(animals, AnimalSortKey::RELATIVE_WEIGHT);
    PrintNames(animals);

    return 0;
}
/*
Урок 4: Итераторы в конструкторах контейнеров
Напишите функцию MakeVector, аналогичную функции MakeSet из урока. Она должна быть шаблоном, принимающим
итераторы на начало и конец диапазона элементов и возвращающим полученный из этих элементов вектор.

Урок 5: Итераторы в методах контейнеров
Итераторы в методах контейнеров
Напишите функцию-шаблон EraseAndPrint. Она должна принимать контейнер и итератор на позицию, которую надо удалить.
После этого EraseAndPrint должна выводить на первой строчке часть контейнера до удалённого элемента,
а на второй строчке — часть контейнера после удалённого элемента.
*/

#include <set>
#include <vector>
#include <iostream>

using namespace std;
// Урок 2. Понятие интервалов
template<typename Iter>
void PrintRange(Iter start, Iter stop) {
    while(start != stop) {
        cout << *start << " "s;
        start++;
    }
    cout << endl;
}

// Урок 3: Концепция полуинтервалов
template<typename Container, typename Element>
void FindAndPrint(Container container, const Element& element) {
    auto iter = find(container.begin(), container.end(), element);
    PrintRange(container.begin(), iter);
    PrintRange(iter, container.end());
}

// Урок 4: Итераторы в конструкторах контейнеров
template<typename Iter>
auto MakeVector(Iter container_begin, Iter container_end) {
    return std::vector(container_begin,container_end);
}

// Урок 5: Итераторы в методах контейнеров

/*
// Задание 1
template<typename Container, typename Iterator>
void EraseAndPrint(Container& container, Iterator iterator) {
    auto new_iterator = container.erase(iterator);
    PrintRange(container.begin(), new_iterator);
    PrintRange(new_iterator, container.end());
}
*/
/*
// Задание 2
template<typename Container>
void EraseAndPrint(Container& container, size_t index) {
    auto new_iterator = container.erase(container.begin() + index);
    PrintRange(container.begin(), new_iterator);
    PrintRange(new_iterator, container.end());
}
*/

// Задание 3
template<typename Container>
void EraseAndPrint(Container& container, size_t delete_pos, size_t left_range, size_t right_range) {
    container.erase(container.begin() + delete_pos);
    PrintRange(container.begin(), container.end());

    container.erase(container.begin() + left_range, container.begin() + right_range);
    PrintRange(container.begin(), container.end());
}

int top_1_lesson_2_to_5() {
    set<int> test = {1, 1, 1, 2, 3, 4, 5, 5};

    // Урок 4
    set<string> unique_langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    auto vector_langs = MakeVector(unique_langs.begin(), unique_langs.end());

    // Урок 5
    vector<string> langs = {"Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s};
    //EraseAndPrint(langs, langs.begin() + 2);
    //EraseAndPrint(langs, 3);
    EraseAndPrint(langs, 0, 0, 2);

    FindAndPrint(test, 3);
    FindAndPrint(test, 0);
    return 0;
}
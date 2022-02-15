// ====================================================
// Урок 4. Задание 1.
// Поиск в отсортированном векторе, словаре и множестве
#include <iostream>
#include <set>
#include <algorithm>

using namespace std;
/*
Напишите функцию FindNearestElement для множества целых чисел numbers и данного числа border. 
Она должна возвращать итератор на элемент множества, ближайший к border. Если ближайших элементов 
несколько, верните итератор на наименьший из них. Если множество пустое, верните итератор на конец.
Для поиска примените методы поиска по множеству. Они более эффективны, чем простой перебор элементов.
*/

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border) {
    // set<int>::const_iterator — тип итераторов для константного множества целых чисел
    if (numbers.empty())
        return numbers.end();

    auto begin = numbers.begin();
    auto end = numbers.end();
    auto iterator = numbers.lower_bound(border);
    
    if(distance(iterator, end) == 0)
        return prev(iterator, 1);

    if (distance(begin, iterator) == 0) {
        return iterator;
    }

    if (*iterator == border) {
        return iterator;
    }
    /*
    Ближайшим элементом может оказаться элемент меньше, чем border, сам border или элемент больше border. 
    Используйте алгоритм lower_bound, чтобы найти позицию элемента, который будет не меньше, чем border.
    Найденный элемент будет равным border или ближайшим с бó‎льшей стороны. Если возможно, сделайте шаг назад
    и найдите ближайший элемент с меньшей стороны. Останется только сравнить, насколько они отстают от border,
    и выбрать ближайший. Когда вы в начале контейнера, шагать назад не получится.
    Используйте метод контейнера lower_bound, а не общую функцию, чтобы ускорить свой код.
    */
    auto prev_val = prev(iterator, 1);
    bool is_left = (border - *prev_val) <= (*iterator - border);

    return is_left ? prev_val : iterator;
}

int top_2_lesson_4_1() {
    set<int> numbers = { 1, 4, 6 };

    cout << *FindNearestElement(numbers, 0) << " " << *FindNearestElement(numbers, 3) << " "
        << *FindNearestElement(numbers, 5) << " " << *FindNearestElement(numbers, 6) << " "
        << *FindNearestElement(numbers, 100) << endl;

    set<int> empty_set;

    cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
    return 0;
}
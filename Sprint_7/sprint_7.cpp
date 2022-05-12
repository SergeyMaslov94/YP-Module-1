// ЧУТОК К ВЕРСИИ CPP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#include "Topic_1/lesson_1.hpp"
//#include "Topic_1/list.h"
// 
//#include "Topic_2/lesson_1.hpp"
//#include "Topic_2/lesson_2_stack_vector/lesson_2.hpp"
//#include "Topic_2/lesson_3_string_view/lesson_3.hpp"
//#include "Topic_2/lesson_4_vacab/lesson_4.hpp"
//#include "Topic_2/lesson_5_simple_vector/lesson_5.hpp"

//#include "Topic_3/lesson_1.hpp"
//#include "Topic_3/lesson_2.hpp"
//#include "Topic_3/lesson_3.hpp"
//#include "Topic_3/lesson_4.hpp"
//#include "Topic_3/lesson_5/lesson_5.hpp"

#include "tmp/tmp.hpp"

#include "final/final_main.hpp"

#include <vector>
#include <deque>
#include <iostream>
#include <cassert>

using namespace std;

//============================================
// ТЕМА 1. Санитайзер
// Урок 4. Отладочные макроопределения
//int main()
//{
//	lesson_1(); 
//}

// Урок 9. Исправляем список

// функция возвращает функциональный объект,
// выполняющий вставку в выбранный список
//template <class Type>
//auto MakeInsertingFunction(vector<SingleLinkedList<Type>>& lists, int x) {
//    return [&lists, x](const Type value) {
//        lists[x].PushFront(value);
//    };
//}
//
//template <class T>
//void InsertRange(int from, int to, T push_function) {
//    for (int i = from; i < to; ++i) {
//        push_function(i);
//    }
//}
//
//int main() {
//    // main тестирует вектор, в нём нет ошибок.
//    // не меняйте код этой функции
//    // помимо этих тестов, список должен проходить все обычные тесты списка.
//    // Ищите ошибки в коде списка и
//    // других функциях этого файла
//
//    vector<SingleLinkedList<int>> lists_a(10);
//
//    auto push_to_2a = MakeInsertingFunction(lists_a, 2);
//    auto push_to_5a = MakeInsertingFunction(lists_a, 5);
//    auto push_to_7a = MakeInsertingFunction(lists_a, 7);
//
//    InsertRange(10, 12, push_to_2a);
//    InsertRange(12, 14, push_to_5a);
//    InsertRange(14, 16, push_to_7a);
//
//    assert(lists_a[2] == SingleLinkedList<int>({ 11, 10 }));
//    assert(lists_a[5] == SingleLinkedList<int>({ 13, 12 }));
//    assert(lists_a[7] == SingleLinkedList<int>({ 15, 14 }));
//
//    vector<SingleLinkedList<int>> lists_b = lists_a;
//
//    auto push_to_2b = MakeInsertingFunction(lists_b, 2);
//    auto push_to_5b = MakeInsertingFunction(lists_b, 5);
//    auto push_to_7b = MakeInsertingFunction(lists_b, 7);
//
//    InsertRange(20, 22, push_to_2b);
//    InsertRange(22, 24, push_to_5b);
//    InsertRange(24, 26, push_to_7b);
//
//    assert(lists_b[2] == SingleLinkedList<int>({ 21, 20, 11, 10 }));
//    assert(lists_b[5] == SingleLinkedList<int>({ 23, 22, 13, 12 }));
//    assert(lists_b[7] == SingleLinkedList<int>({ 25, 24, 15, 14 }));
//
//    lists_a[2].PopFront();
//    lists_a[5].InsertAfter(lists_a[5].begin(), 100);
//    lists_b[5].EraseAfter(next(lists_b[5].begin()));
//    lists_b[7].Clear();
//
//    assert(lists_a[2] == SingleLinkedList<int>({ 10 }));
//    assert(lists_a[5] == SingleLinkedList<int>({ 13, 100, 12 }));
//    assert(lists_b[5] == SingleLinkedList<int>({ 23, 22, 12 }));
//    assert(lists_b[7] == SingleLinkedList<int>());
//    return 0;
//}

//============================================
// ТЕМА 2. ЭФФЕКТИВНЫЕ ЛИНЕЙНЫЕ КОНТЕЙНЕРЫ
//int main() {
	//lesson_1(); // урок 6. Эффективный список
	//lesson_2(); // урок 7. Проще, быстрее std::array
	//lesson_3(); // урок 8. Помощник в работе со строками
	//lesson_4(); // урок 9. Простейший словарь 
	//lesson_5();   // урок 10. Разработка Simplevector
//}
//============================================
// ТЕМА 3. CЕМАНТИКА ПЕРЕМЕЩЕНИЙ
//int main() {
//	//lesson_1(); // урок 1. Временные объекты
//	//lesson_2(); // Copy elision и NRVO
//	//lesson_3(); // Функция move и другие перемещения. СЧИТАЛКА ИОСИФА
//	//lesson_4(); // Копировать нельзя, ПЕРЕМЕСТИТЬ! Эксперименты с классом, которому запрещенно копировать. ЗАДАЧА С ПЕРДЛОЖЕНИЯМИ
//	lesson_5(); // СЧИТАЛКА ИОСИФА и ЗАДАЧА С ПРЕДЛОЖЕНИЯМИ на move итераторах
//	/*А здорово получилось код сократить, с использованием make_move_iterator*/
//}

//============================================
// РАЗБИРАЮСЬ С УМНЫМ УКАЗАТЕЛЕМ 
//int main() {
//	tmp();
//}

//============================================
// ФИНАЛЬНЫЙ ПРОЕКТ СПРИНТА 7
/* пишем simple_vector поддерживающий перемещение*/
int main() {
	tmp();
	final();
}

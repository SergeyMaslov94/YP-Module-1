//#include "Topic_1/theory_1.hpp"
//#include "Topic_1/theory_2.hpp"
//#include "Topic_1/lesson_3.hpp"
//#include "Topic_1/lesson_4.hpp"
//#include "Topic_1/theory_3.hpp"
//#include "Topic_1/lesson_5.hpp"
//#include "Topic_1/lesson_6.hpp"
//#include "Topic_1/lesson_7.hpp"
//#include "Topic_1/lesson_9.hpp"

//#include "Topic_2/theory_1.hpp"
//#include "Topic_2/lesson_3.hpp"

//#include "Topic_3/theory_1.hpp"
//#include "Topic_3/lesson_5.hpp"
//#include "Topic_3/lesson_6.hpp"
//#include "Topic_3/lesson_7.hpp"
//#include "Topic_3/lesson_7_1.hpp"
//#include "Topic_3/lesson_8.hpp"
//#include "Topic_3/lesson_9.hpp"
#include "Topic_3/lesson_10.hpp"

int main()
{
    // ТУТ ЕСТЬ ГЕНЕРАТОРЫ СЛУЧАЙНЫХ ЧИСЕЛ
    //===============================================
    /* TOPIC_1: Профлируем и ускоряем */
    //theory_1();
    //theory_2();
    //lesson_3();
    //lesson_4(); // профилируем используя классы
    /*
    //---------------------------------------------
    * КАКАЯ ТО ВУНДЕР ХЕРНЯ С МАКРОСАМИ
    #define PROFILE_CONCAT_INTERNAL(X, Y) X ## Y
    #define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
    #define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
    #define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)
    //----------------------------------------------
    */
    //theory_3(); // теория по использованию макросов
    //lesson_5(); // профилируем используя макросы
    //lesson_6(); // В 50 раз увеличена скорость реверса вектора 
    //lesson_7();
    //lesson_9(); // Немного про RAM и кэш-линии

    //===============================================
    // ВАЖНАЯ ХРЕНЬ!
    /* TOPIC_2: Потоковые хитрости */
    //theory_1(); // ЗАПИСЬ В ФАЙЛ И НЮАНСЫ, flush - сброс буфера
     
    // Разработайте класс StreamUntier, который выполняет следующие действия:
    // - В конструкторе — «отвязывание» потока, переданного в конструктор как аргумент.
    // - В деструкторе — «привязывание» того потока, который был отвязан.
    // lesson_3();

    //===============================================
    /* TOPIC_3: ТЕОРИЯ БЫСТРОДЕЙСТВИЯ */
    //theory_1();
    //lesson_5(); 
    //lesson_6(); // Логарифмическая сложность
    //lesson_7(); // Опасности экспоненты
    //lesson_7_1();
    //lesson_8(); // Аммортизированная сложность - НИЧЕ НЕ ПОНЯЛ НО ОЧЕНЬ ИНТЕРЕСНО
    //lesson_9(); // Урок в котором find_if лучше upper_bound
    lesson_10();
}

/*
//================================================================================
//sprint_5 3 >numbers2.txt <numbers.txt !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// ОТОКОВЫЕ ХИТОРОСТИ: Связь потоков в поисках оптимального ввода/вывода
// main с аргументами означает, что программа принимает аргументы командной строки,
// при запуске из консоли их количество будет сохранено в переменную argc,
// а значения — в argv.
#include <iostream>
#include <string>
#include "Topic_1/log_duration.h"
using namespace std;
// Тип const char** будет обсуждаться позже в курсе, работать с ним можно
// отчасти как с вектором: argv[0] — нулевой аргумент, argv[1] — первый. 
// argv[i] можно конвертировать в string
int main(int argc, const char** argv) {
    // Нулевой аргумент — это всегда имя программы,
    // поэтому нам нужен первый
    int arg = stoi(argv[1]);

    if (arg == 1) {
        LOG_DURATION("endl"s);
        int i;
        while (cin >> i) {
            cout << i * i << endl;
        }
    }

    // НЕ ОТВЯЗЫВАЕМ ПОТОКИ ДРУГ ОТ ДРУГА. ПРОИЗВОДИТЕЛЬНОСТЬ ТАКАЯ ЖЕ КАК И БЫЛА С endl
    if (arg == 2) {
        // Чтобы "\n" воспринималось не как перевод строки, а как 
        // слэш и буква 'n', добавим перед ним ещё один слеш:
        // два слеша в строковом (и символьном) литерале воспринимаются
        // как один (\) 
        LOG_DURATION("\\n"s);
        int i;
        while (cin >> i) {
            cout << i * i << "\n"s;
        }
    }


    // ОТВЯЗЫВАЕМ ПОТОКИ. Производительность возрасла
    
    //Совсем не очевидно, но чтение из cin заставляет буфер cout 
    //и cerr опустошаться, как если бы мы добавили cout.flush() 
    //перед ним. В этом причина медленной работы программы с \n. 
    //К счастью, есть способ исправить ситуацию, отвязав cin 
    //от cout так: cin.tie(nullptr);

    if (arg == 3) {
        LOG_DURATION("\\n with tie"s);
        auto tied_before  = cin.tie(nullptr); // ОТВЯЗАЛИ CIN ОТ COUT
        int i;
        while (cin >> i) {
            cout << i * i << "\n"s;
        }
        cin.tie(tied_before); // ПРИВЯЗАЛИ ОБРАТНО
    }
}
*/
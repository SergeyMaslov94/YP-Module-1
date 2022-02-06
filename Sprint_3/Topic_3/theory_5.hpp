/*
Если во время работы конструктора выбрасывается исключение, инициализация объекта считается незавершённой.
При этом деструктор такого объекта вызван не будет, что логично: объект фактически не создан.
Зато будут вызваны деструкторы тех его полей, которые к этому моменту уже сконструированы.
*/

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

class Greeter {
public:
    Greeter(const string& name)
            : name_(name)  // Сохраняем name_ внутри
    {
        cout << "Hello, "s << name_ << endl;
    }

    ~Greeter() {
        cout << "Goodbye, "s << name_ << endl;
    }

private:
    string name_;
};

class Thrower {
public:
    Thrower()
            : field_("field"s) // передаём параметры конструктору поля field_
    {
        cout << "Thrower()"s << endl;
        // Исключение, выброшенное здесь, будет поймано за пределами конструктора
        throw runtime_error("Oops"s);
    }

    ~Thrower() {
        cout << "~Thrower()"s << endl;
    }

private:
    Greeter field_;
};

int top_3_theory_5() {
    try {
        Thrower t;
    } catch (const runtime_error& e) {
        cout << e.what() << endl;
    }

    return 0;
}
/*
 Сначала будет сконструировано поле field_ класса Thrower. Об этом говорит вывод Hello, field.
 Затем при выполнении тела конструктора Thrower будет выведена строка Thrower() и выброшено исключение,
 обработчик которого находится за пределами конструктора. В процессе раскрутки стека вызовется
 деструктор сконструированного поля field_, что подтверждает вывод строки Goodbye, field.
 А вот деструктор Thrower вызван не будет, так как конструирование объекта не было завершено.
 */
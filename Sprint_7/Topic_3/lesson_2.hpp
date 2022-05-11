#include <iostream>
#include <utility>

using namespace std;

class SchrodingerCat {
public:
    SchrodingerCat() = default;
    SchrodingerCat(const SchrodingerCat& other) {
        cout << "Cctor"s << endl;
    }
    SchrodingerCat& operator=(const SchrodingerCat& other) {
        cout << "Op="s << endl;
        return *this;
    }
    ~SchrodingerCat() = default;
};

class Box {
public:
    // достанем пару котов
    pair<SchrodingerCat, SchrodingerCat> GetCats() {
        SchrodingerCat cat1;
        SchrodingerCat cat2;
        is_empty_ = true;
        // временная пара котов -> пара, которая возвращается из функции
        return { cat1, cat2 };
    }
    bool HasCat() {
        return !is_empty_;
    }

private:
    bool is_empty_ = false;
};

int lesson_2() {
    Box black_box;
    if (black_box.HasCat()) {
        pair<SchrodingerCat, SchrodingerCat> cat_pair;
        // копируем пару котов в переменную cat_pair
        cat_pair = black_box.GetCats();
    }
    return 0;
}
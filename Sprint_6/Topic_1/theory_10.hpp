#include "octopus.hpp"
#include <cassert>
#include <iostream>

using namespace std;

int theory_10() {
    {
        Octopus octopus1(3);
        octopus1.GetTentacle(1).LinkTo(octopus1.GetTentacle(2));
        Octopus octopus2;

        octopus2 = octopus1;
        // Этот assert выстрелит, так как операция присваивания присвоит массив 
        // указателей на щупальца первого осьминога второму, и осьминоги будут 
        // использовать один и тот же набор щупалец.
        assert(&octopus2.GetTentacle(1) != &octopus1.GetTentacle(1));
        assert(octopus2.GetTentacle(1).GetLinkedTentacle() == &octopus1.GetTentacle(2));

        // После присваивания оба осьминога будут ссылаться на одни и те же щупальца

        // Деструктор octopus2 удалит щупальца, на которые ссылается octopus1,
        // и все указатели octopus1 станут невалидными.

        // Деструктор octopus1 выполнит удаление щупальцев с невалидными указателями,
        // что приведёт к неопределёному поведению.
    }
    cout << "OK"s << endl;
    return 0;
}
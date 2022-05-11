#include "josephus_permutation.hpp"
#include "sentence.hpp"

int lesson_5() {
    // Считалка ИОСИФА на move итераторах
    TestIntVector();
    TestAvoidsCopying();

    // Задача разделения н предложения по лексемам
    TestSplitting();
    TestSplittingNotCopy();
    
    return 0;
}
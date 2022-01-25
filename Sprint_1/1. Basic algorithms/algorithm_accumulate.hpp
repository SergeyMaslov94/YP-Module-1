#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>

using namespace std;

double Average(const vector<int>& xs) {
    if (xs.empty())
        return 0;
    // посчитайте среднее арифметическое: поделите сумму на размер
    // суммирование начните с 0.0 - тогда результат выражения будет вещественным числом
    double value = accumulate(xs.begin(), xs.end(), 0.0);
    return value/xs.size();
}

string Combine( string tail, string word )
{
    return(tail + " -"s + word);
}

string AddStopWords(const string& query, const set<string>& stop_words) {
    
    string combine_words = accumulate(stop_words.begin(), stop_words.end(), ""s, Combine);
    return query + combine_words;
}

//======================================================

int task_1() {
    // не меняйте код main
    cout << Average({ 1, 2, 2 }) << endl;
    return 0;
}

/*
Алгоритм accumulate может работать не только с вектором, но и с другими контейнерами 
— например, с set. Напишите функцию AddStopWords, которая принимает строку-запрос 
и множество стоп-слов и возвращает новую строку. В новой строке после запроса 
идут стоп-слова через пробел, перед каждым стоит дефис. Передайте в accumulate 
четвёртым аргументом функцию, сворачивающую множество стоп-слов в «хвост» 
нужного формата.
*/

int task_2() {
    string query = "some tasty oranges"s;
    set<string> stop_words = {"of", "in"};

    cout << AddStopWords(query, stop_words) << endl;
    return 0;
}

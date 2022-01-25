#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

vector<string> ReadLine()
{
    vector<string> words;
    string str;
    int N;
    cin >> N;

    for (auto i = 0; i < N; i++)
    {
        cin >> str;
        words.push_back(str);
    }

    return words;
}

vector<string> SplitIntoWords(const string& text)
{
    vector<string> words;
    string word;
    bool number_words_flag = false;

    if (text.empty()) {
        return words;
    }

    for (const char c : text)
    {
        if (c == ' ')
        {
            if (!word.empty() && number_words_flag)
            {
                words.push_back(word);
            }
            else
            {
                number_words_flag = true;
            }
            word = ""s;
        }
        else
        {
            word += c;
        }
    }
    words.push_back(word);

    return words;
}

void run() {
    /* считываю данные */
    auto words = ReadLine();

    // было что то введенно после числа N, сортируем в лексикографическом порядке по возрастанию
    if (!words.empty())
    {
        sort(words.begin(), words.end(), [](string v1, string v2)
            {
                return(lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end(), [](char cl, char cr) { return tolower(cl) < tolower(cr); }));
            }
        );

        for (const auto word : words)
        {
            cout << word << " "s;
        }
        cout << endl;
    }
    // если ничего не введено
    else
    {
        cout << " "s << endl;
    }
}

//=================================================================================================
vector<int> ReadNumbers() {
    int count;
    cin >> count;

    vector<int> numbers;
    for (int i = 0; i < count; ++i) {
        int number;
        cin >> number;
        numbers.push_back(number);
    }

    return numbers;
}

void PrintNumbers(const vector<int>& numbers) {
    for (auto number : numbers) {
        cout << number << " "s;
    }
}

void run2() {
    vector<int> numbers = ReadNumbers();

    sort(numbers.begin(), numbers.end(),
        [](int n1, int n2) {
            /*
               Напишите тело лямбда-функции самостоятельно.
               Сначала должны располагаться чётные числа в порядке возрастания,
               а следом за ними — нечётные числа в порядке убывания
            */

            // по возрастанию
            if (n1 % 2 && n2 % 2)
                return(n2 < n1);

            // по убыванию
            else if (!(n1 % 2) && !(n2 % 2))
                return (n2 > n1);

            else if (!(n1 % 2) && (n2 % 2))
                return true;

            else if ((n1 % 2) && !(n2 % 2))
                return false;
        });

    PrintNumbers(numbers);
}
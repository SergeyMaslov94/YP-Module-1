/* Тестируем по плану */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Определяет, будет ли слово палиндромом
// text может быть строкой, содержащей строчные символы английского алфавита и пробелы
// Пустые строки и строки, состоящие только из пробелов, — это не палиндромы
std::vector<char> ParseText(const string& text)
{
    std::vector<char> sample;

    for (const auto liter : text)
    {
        if (liter != ' ')
        {
            sample.push_back(liter);
        }
    }

    return sample;
}

bool IsPalindrome(const string& text) {
    
    // Напишите недостающий код
    auto sample = ParseText(text);

    if (sample.empty())
        return false;
     
    std::vector<char> orig_sample = sample;

    reverse(sample.begin(), sample.end());

    if (sample == orig_sample)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int lesson_3() {
    string text;
    getline(cin, text);

    if (IsPalindrome(text)) {
        cout << "palindrome"s << endl;
    }
    else {
        cout << "not a palindrome"s << endl;
    }

    return 0;
}
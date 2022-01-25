// Фреймворк юнит теста с шаблонами
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

void Assert(bool value, const string& value_str, const string& file, const string& func, unsigned line) 
{
    // Реализуйте тело функции Assert
    if (!value)
    {
        cout << file << "(" << line << "): " << func << ": ASSERT(" << value_str << ") " << "failed." << endl;
        abort();
    }
}

void AssertHint(bool value, const string& hint, const string& value_str, const string& file, const string& func, unsigned line)
{
    // Реализуйте тело функции Assert
    if (!value)
    {
        if (hint.empty())
        {
            cout << file << "(" << line << "): " << func << ": ASSERT(" << value_str << ") " << "failed." << endl;
            abort();
        }
        else
        {
            cout << file << "(" << line << "): " << func << ": ASSERT(" << value_str << ") " << "failed. Hint: " << hint << endl;
            abort();
        }
    }
}

#define ASSERT(expr) Assert((expr), #expr, __FILE__, __FUNCTION__, __LINE__)
#define ASSERT_HINT(expr, hint) AssertHint((expr), (hint), #expr, __FILE__, __FUNCTION__, __LINE__) 

int top_2_lesson_3() 
{
    string hello = "hello"s;
    ASSERT(hello.empty());
    ASSERT_HINT(2 + 2 == 5, "This will fail"s);
    return 0;
}
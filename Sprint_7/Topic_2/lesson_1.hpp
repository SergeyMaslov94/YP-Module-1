#include <list>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

class Editor {
public:
    Editor();
    // сдвинуть курсор влево
    void Left();
    // сдвинуть курсор вправо 
    void Right();
    // вставить символ token
    void Insert(char token);
    // вырезать не более tokens символов, начиная с текущей позиции курсора
    void Cut(size_t tokens = 1);
    // cкопировать не более tokens символов, начиная с текущей позиции курсора
    void Copy(size_t tokens = 1);
    // вставить содержимое буфера в текущую позицию курсора
    void Paste();
    // получить текущее содержимое текстового редактора
    std::string GetText() const;

private:
    std::list<char> text;
    std::list<char> bufer;
    std::list<char>::iterator cursor_pos;
};
Editor::Editor() {
    text.push_back('|');
    cursor_pos = text.begin();
};

// сдвинуть курсор влево
void Editor::Left() {
    if (text.size() == 1) {
        return;
    }

    if (cursor_pos == text.begin()) {
        return;
    }

    auto right = cursor_pos; ++right;
    auto left = cursor_pos; --left;
    std::reverse(left, right);
    cursor_pos = left;
};

// сдвинуть курсор вправо 
void Editor::Right() {
    if (text.size() == 1) {
        return;
    }

    if (cursor_pos == text.end()) {
        return;
    }

    auto left = cursor_pos;
    auto right = cursor_pos;
    ++right;

    if (right == text.end()) {
        return;
    }

    std::reverse(left, ++right);
    cursor_pos = --right;
};

// вставить символ token
void Editor::Insert(char token) {
    text.insert(cursor_pos, token);
};

// вырезать не более tokens символов, начиная с текущей позиции курсора
void Editor::Cut(size_t tokens) {
    if (!bufer.empty()) {
        bufer.clear();
    }

    if (text.size() == 0) {
        return;
    }
   
    while(tokens > 0 && text.size() != 1) {
        auto it = cursor_pos; it++;
        
        if (it == text.end()) {
            return;
        }

        bufer.push_back(*it);
        text.erase(it);
        --tokens;
    }
};

// cкопировать не более tokens символов, начиная с текущей позиции курсора
void Editor::Copy(size_t tokens) {
    if (!bufer.empty()) {
        bufer.clear();
    }

    if (text.size() == 0) {
        return;
    }

    auto it = cursor_pos; it++;
    
    while (tokens > 0 && text.size() != 1) {
        if (it == text.end()) {
            return;
        }

        bufer.push_back(*it);
        --tokens;
        it++;
    }
};

// вставить содержимое буфера в текущую позицию курсора
void Editor::Paste() {
    if (bufer.empty()) {
        return;
    }

    for (const auto& letter : bufer) {
        text.insert(cursor_pos, letter);
    }
};

// получить текущее содержимое текстового редактора
std::string Editor::GetText() const {
    auto result_list = text;
    result_list.remove('|');

    return string(result_list.begin(), result_list.end());
};

int lesson_1() {
    Editor editor;

    // курсор влево в пустом редакторе
    {
        editor.Left();
    }

    const string text = "hello, world"s;
    for (char c : text) {
        editor.Insert(c);
    }
    // Текущее состояние редактора: `hello, world|`
    for (size_t i = 0; i < text.size(); ++i) {
        editor.Left();
    }
    // Текущее состояние редактора: `|hello, world`
    editor.Cut(7);
    // Текущее состояние редактора: `|world`
    // в буфере обмена находится текст `hello, `
    for (size_t i = 0; i < 5; ++i) {
        editor.Right();
    }
    // Текущее состояние редактора: `world|`
    editor.Insert(',');
    editor.Insert(' ');
    // Текущее состояние редактора: `world, |`
    editor.Paste();
    // Текущее состояние редактора: `world, hello, |`
    editor.Left();
    editor.Left();
    //Текущее состояние редактора: `world, hello|, `
    editor.Cut(3);  // Будут вырезаны 2 символа
    // Текущее состояние редактора: `world, hello|`
    cout << editor.GetText();
    return 0;
}
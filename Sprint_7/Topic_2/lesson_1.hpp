#include <list>
#include <string>

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
    if (cursor_pos == text.begin()) {
        return;
    }
    std::reverse(text.begin(), cursor_pos);
};

// сдвинуть курсор вправо 
void Editor::Right() {};

// вставить символ token
void Editor::Insert(char token) {
    text.insert(cursor_pos, token);
};

// вырезать не более tokens символов, начиная с текущей позиции курсора
void Editor::Cut(size_t tokens) {};

// cкопировать не более tokens символов, начиная с текущей позиции курсора
void Editor::Copy(size_t tokens) {};

// вставить содержимое буфера в текущую позицию курсора
void Editor::Paste() {};

// получить текущее содержимое текстового редактора
std::string Editor::GetText() const { return ""s; };
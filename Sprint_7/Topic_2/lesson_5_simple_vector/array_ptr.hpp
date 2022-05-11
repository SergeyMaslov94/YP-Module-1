#include <cassert>
#include <string>
#include <iostream>

using namespace std;

// Умный указатель, удаляющий связанный объект при своём разрушении.
// Параметр шаблона T задаёт тип объекта, на который ссылается указатель
template <typename T>
class ScopedPtr {
public:
    // Конструктор по умолчанию создаёт нулевой указатель,
    // так как поле ptr_ имеет значение по умолчанию nullptr
    ScopedPtr() = default;

    // Создаёт указатель, ссылающийся на переданный raw_ptr.
    // raw_ptr ссылается либо на объект, созданный в куче при помощи new,
    // либо является нулевым указателем
    // Спецификатор noexcept обозначает, что метод не бросает исключений
    explicit ScopedPtr(T* raw_ptr) noexcept {
        // Реализуйте самостоятельно
        ptr_ = raw_ptr;
    }

    // Удаляем у класса конструктор копирования
    ScopedPtr(const ScopedPtr&) = delete;

    // Деструктор. Удаляет объект, на который ссылается умный указатель.
    ~ScopedPtr() {
        // Реализуйте тело деструктора самостоятельно
        delete ptr_;
    }

    // Возвращает указатель, хранящийся внутри ScopedPtr
    T* GetRawPtr() const noexcept {
        // Напишите код метода самостоятельно
        return ptr_;
    }

    // Прекращает владение объектом, на который ссылается умный указатель.
    // Возвращает прежнее значение "сырого" указателя и устанавливает поле ptr_ в null
    T* Release() noexcept {
        // Реализуйте самостоятельно
        auto ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }

    //========================================
    // Оператор приведения к типу bool позволяет узнать, ссылается ли умный указатель
    // на какой-либо объект
    explicit operator bool() const noexcept {
        // Реализуйте самостоятельно
        if (ptr_ == nullptr)
            return false;
        return true;
    }

    // Оператор разыменования возвращает ссылку на объект
    // Выбрасывает исключение std::logic_error, если указатель нулевой
    T& operator*() const {
        // Реализуйте самостоятельно
        if (ptr_ == nullptr)
            throw std::logic_error("null pointer");

        return *ptr_;
    }

    // Оператор -> должен возвращать указатель на объект
    // Выбрасывает исключение std::logic_error, если указатель нулевой
    T* operator->() const {
        // Реализуйте самостоятельно
        if (ptr_ == nullptr)
            throw std::logic_error("null pointer");

        return ptr_;
    }
    //========================================

private:
    T* ptr_ = nullptr;
};
#pragma once
#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iterator>
#include <utility>
#include "array_ptr.hpp"

//====================================================================================
// Структура - обертка для конструктора SimpleVector с резервированием
// Черт его знает, зачем тебе этот ReserveProxyObj сдался, но я в чужие дела не лезу.
struct ReserveProxyObj {

    ReserveProxyObj(size_t capacity_to_reserve) {
        capacity = capacity_to_reserve;
    }

    size_t capacity = 0;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}
//====================================================================================
template <typename Type>
class SimpleVector {
private:
    ArrayPtr<Type> ptr_;
    size_t size_ = 0;
    size_t capacity_ = 0;

public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) {
        ptr_.SetRawPtr(size); 
        std::fill(ptr_.GetRawPtr(), ptr_.GetRawPtr() + size, Type());
        size_ = size;
        capacity_ = size;
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) {
        ptr_.SetRawPtr(size);
        std::fill(ptr_.GetRawPtr(), ptr_.GetRawPtr() + size, value);
        size_ = size;
        capacity_ = size;
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) {
        ptr_.SetRawPtr(init.size());
        std::copy(init.begin(), init.end(), ptr_.GetRawPtr());
        size_ = init.size();
        capacity_ = init.size();
    }

    // Конструктор с резервированием, использующий
    // дополнительный класс - обёртку
    SimpleVector(ReserveProxyObj init) {
        ptr_.SetRawPtr(init.capacity);
        std::fill(ptr_.GetRawPtr(), ptr_.GetRawPtr() + init.capacity, Type());
        capacity_ = init.capacity;
    }

    // Конструктор копирования
    SimpleVector(const SimpleVector& other) {
        if (other.GetSize() == 0) {
            return;
        }
        ptr_.SetRawPtr(other.GetSize());
        size_t cnt = 0;
        for (const auto val : other) {
            ptr_.GetRawPtr()[cnt++] = val;
        }
        size_ = other.GetSize();
        capacity_ = other.GetSize();
    }

    // Конструктор перемещения
    SimpleVector(SimpleVector&& other)
        : ptr_(std::move(other.ptr_.Release())) {
        size_ = std::exchange(other.size_, 0);
        capacity_ = std::exchange(other.capacity_, 0);
    }

    //====================================================================================
    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        assert(index < GetSize());
        return ptr_.GetRawPtr()[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        assert(index < GetSize());
        return ptr_.GetRawPtr()[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("");
        }
        return *(ptr_.GetRawPtr() + index);
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("");
        }
        return *(ptr_.GetRawPtr() + index);
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size <= size_) {
            size_ = new_size;
            return;
        }

        if (new_size <= capacity_) {
            for (auto it = ptr_.GetRawPtr() + size_; it < ptr_.GetRawPtr() + new_size; it++) {
                *it = std::move(Type());
            }
            size_ = new_size;
            return;
        }

        if (new_size > capacity_) {
            ArrayPtr<Type> tmp;
            tmp.SetRawPtr(ptr_.Release());
            ptr_.SetRawPtr(new_size); 
            
            std::move(tmp.GetRawPtr(), tmp.GetRawPtr() + size_, ptr_.GetRawPtr());
            for (auto it = ptr_.GetRawPtr() + size_; it < ptr_.GetRawPtr() + new_size; it++) {
                *it = std::move(Type());
            }

            size_ = new_size;
            capacity_ = std::max(new_size, capacity_ * 2);
        }
    }
    // Задает ёмкость вектора. 
    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            ArrayPtr<Type> tmp;
            tmp.SetRawPtr(ptr_.Release());
            ptr_.SetRawPtr(new_capacity);

            std::copy(tmp.GetRawPtr(), tmp.GetRawPtr() + size_, ptr_.GetRawPtr());
            for (auto it = ptr_.GetRawPtr() + size_; it < ptr_.GetRawPtr() + new_capacity; it++) {
                *it = std::move(Type());
            }

            capacity_ = new_capacity;
        }
    }

    //====================================================================================
    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return ptr_.GetRawPtr();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return ptr_.GetRawPtr() + size_;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return cbegin();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return cend();
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return ptr_.GetRawPtr();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return ptr_.GetRawPtr() + size_;
    }

    //=======================================================================
    SimpleVector& operator=(const SimpleVector& rhs) {
        if (rhs.ptr_.GetRawPtr() == ptr_.GetRawPtr()) {
            return *this;
        }
        auto tmp(rhs);
        this->swap(tmp);
        return *this;
    }

    // Добавляет элемент в конец вектора (копирование)
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        if (capacity_ == 0) {
            ptr_.SetRawPtr(1);
            ptr_.GetRawPtr()[0] = item;
            size_++;
            capacity_++;
            return;
        }

        if (size_ < capacity_) {
            ptr_.GetRawPtr()[size_] = item;
            size_++;
            return;
        }

        ArrayPtr<Type> tmp;
        tmp.SetRawPtr(ptr_.Release());
        ptr_.SetRawPtr(capacity_ * 2);
        std::copy(tmp.GetRawPtr(), tmp.GetRawPtr() + size_, begin());
        ptr_.GetRawPtr()[size_] = item;

        size_++;
        capacity_ = capacity_ * 2;
    }

    // Добавляет элемент в конец вектора (перемещение)
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(Type&& item) {
        if (capacity_ == 0) {
            ptr_.SetRawPtr(1);
            ptr_.GetRawPtr()[0] = std::move(item);
            size_++;
            capacity_++;
            return;
        }

        if (size_ < capacity_) {
            ptr_.GetRawPtr()[size_] = std::move(item);
            size_++;
            return;
        }

        ArrayPtr<Type> tmp;
        tmp.SetRawPtr(ptr_.Release());
        ptr_.SetRawPtr(capacity_ * 2);
        std::move(tmp.GetRawPtr(), tmp.GetRawPtr() + size_, begin());
        ptr_.GetRawPtr()[size_] = std::move(item);

        size_++;
        capacity_ = capacity_ * 2;
    }

    // Вставляет значение value в позицию pos (копирование)
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        assert(pos >= begin() && pos <= end()); // добавлена проверка попадания pos на интервал [begin(), end()]

        if (capacity_ == 0) {
            ptr_.SetRawPtr(1);
            ptr_.GetRawPtr()[0] = value;
            size_++;
            capacity_++;
            return ptr_;
        }

        if (size_ < capacity_) {
            std::copy_backward(const_cast<Iterator>(pos), end(), end() + 1);
            size_t idx = pos - begin();
            ptr_.GetRawPtr()[idx] = value;

            size_++;
            return ptr_.GetRawPtr() + idx;
        }

        ArrayPtr<Type> tmp;
        size_t idx = pos - begin();
        tmp.SetRawPtr(ptr_.Release());
        ptr_.SetRawPtr(capacity_ * 2);

        std::copy(tmp.GetRawPtr(), tmp.GetRawPtr() + idx, begin());
        ptr_.GetRawPtr()[idx] = value;
        std::copy(tmp.GetRawPtr() + idx, tmp.GetRawPtr() + size_, begin() + idx + 1);

        size_++;
        capacity_ = capacity_ * 2;
        return begin() + idx;
    }

    // Вставляет значение value в позицию pos (перемещение)
    // Возвращает итератор на вставленное значение
    Iterator Insert(ConstIterator pos, Type&& value) {
        assert(pos >= begin() && pos <= end()); // добавлена проверка попадания pos на интервал [begin(), end()]

        if (capacity_ == 0) {
            ptr_.SetRawPtr(1);
            ptr_.GetRawPtr()[0] = std::move(value);
            size_++;
            capacity_++;
            return ptr_.GetRawPtr();
        }

        if (size_ < capacity_) {
            std::move_backward(const_cast<Iterator>(pos), end(), end() + 1);
            size_t idx = pos - begin();
            ptr_.GetRawPtr()[idx] = std::move(value);

            size_++;
            return ptr_.GetRawPtr() + idx;
        }

        ArrayPtr<Type> tmp;
        size_t idx = pos - begin();
        tmp.SetRawPtr(ptr_.Release());
        ptr_.SetRawPtr(capacity_ * 2);

        std::move(tmp.GetRawPtr(), tmp.GetRawPtr() + idx, begin());
        ptr_.GetRawPtr()[idx] = std::move(value);
        std::move(tmp.GetRawPtr() + idx, tmp.GetRawPtr() + size_, begin() + idx + 1);

        size_++;
        capacity_ = capacity_ * 2;
        return begin() + idx;
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        assert(!IsEmpty()); // добавлена проверка на пустой вектор
        size_--;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        assert(!IsEmpty()); // добавлена проверка на пустой вектор
        assert(pos >= begin() && pos < end()); // добавлена проверка попадания pos на интервал [begin(), end())

        auto idx = pos - begin();
        std::move(const_cast<Iterator>(pos) + 1, end(), begin() + idx);

        size_--;
        return begin() + idx;
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        ArrayPtr<Type> tmp(ptr_.Release());
        ptr_.SetRawPtr(other.ptr_.Release());
        other.ptr_.SetRawPtr(tmp.Release());

        std::swap(other.size_, size_);
        std::swap(other.capacity_, capacity_);
    }
};

//====================================================================================
template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    }
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs > lhs);
}
#pragma once
#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <cmath>
#include <stdexcept>

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
    Type* p_ = nullptr;
    size_t size_ = 0;
    size_t capacity_ = 0;

public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) {
        p_ = new Type[size];
        std::fill(p_, p_ + size, Type());
        size_ = size;
        capacity_ = size;
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) {
        p_ = new Type[size];
        std::fill(p_, p_ + size, value);
        size_ = size;
        capacity_ = size;
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) {
        p_ = new Type[init.size()];
        std::copy(init.begin(), init.end(), p_);
        size_ = init.size();
        capacity_ = init.size();
    }

    // Конструктор с резервированием, использующий
    // дополнительный класс - обёртку
    SimpleVector(ReserveProxyObj init) {
        p_ = new Type[init.capacity];
        std::fill(p_, p_ + init.capacity, Type());
        capacity_ = init.capacity;
    }

    ~SimpleVector() {
        if (p_ != nullptr) {
            delete[] p_;
        }
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
        return p_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return p_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("");
        }
        return *(p_ + index);
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("");
        }
        return *(p_ + index);
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
            std::fill(p_ + size_, p_ + new_size, Type());
            size_ = new_size;
            return;
        }

        if (new_size > capacity_) {
            Type* tmp = p_;
            p_ = new Type[new_size];
            std::copy(tmp, tmp + size_, p_);
            std::fill(p_ + size_, p_ + new_size, Type());
            delete[] tmp;

            size_ = new_size;
            capacity_ = std::max(new_size, capacity_ * 2);
        }
    }
    // Задает ёмкость вектора. 
    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            Type* tmp = p_;
            p_ = new Type[new_capacity];
            std::copy(tmp, tmp + size_, p_);
            std::fill(p_ + size_, p_ + new_capacity, Type());
            delete[] tmp;

            capacity_ = new_capacity;
        }
    }

    //====================================================================================
    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        if (capacity_ == 0) {
            return nullptr;
        }
        return p_;
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        if (capacity_ == 0) {
            return nullptr;
        }
        return p_ + size_;
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
        if (capacity_ == 0) {
            return nullptr;
        }
        return p_;
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        if (capacity_ == 0) {
            return nullptr;
        }
        return p_ + size_;
    }

    //=======================================================================
    SimpleVector(const SimpleVector& other) {
        if (other.GetSize() == 0) {
            return;
        }
        p_ = new Type[other.GetSize()];
        size_t cnt = 0;
        for (const auto val : other) {
            p_[cnt++] = val;
        }
        size_ = other.GetSize();
        capacity_ = other.GetSize();
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (rhs.p_ == p_) {
            return *this;
        }
        auto tmp(rhs);
        this->swap(tmp);
        return *this;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        if (capacity_ == 0) {
            p_ = new Type[1];
            p_[0] = item;
            size_++;
            capacity_++;
            return;
        }

        if (size_ < capacity_) {
            p_[size_] = item;
            size_++;
            return;
        }

        auto tmp = p_;
        p_ = new Type[capacity_ * 2];
        std::copy(tmp, tmp + size_, begin());
        p_[size_] = item;
        delete[] tmp;

        size_++;
        capacity_ = capacity_ * 2;
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        if (capacity_ == 0) {
            p_ = new Type[1];
            p_[0] = value;
            size_++;
            capacity_++;
            return p_;
        }

        if (size_ < capacity_) {
            std::copy_backward(const_cast<Iterator>(pos), end(), end() + 1);
            size_t idx = pos - begin();
            p_[idx] = value;

            size_++;
            return p_ + idx;
        }

        auto tmp = p_;
        size_t idx = pos - begin();
        p_ = new Type[capacity_ * 2];
        std::copy(tmp, tmp + idx, begin());
        p_[idx] = value;
        std::copy(tmp + idx, tmp + size_, begin() + idx + 1);
        delete[] tmp;

        size_++;
        capacity_ = capacity_ * 2;
        return begin() + idx;
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        if (size_ == 0) {
            return;
        }
        size_--;
    }


    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        if (size_ == 0) {
            return begin();
        }

        auto idx = pos - begin();
        std::copy(const_cast<Iterator>(pos) + 1, end(), begin() + idx);

        size_--;
        return begin() + idx;
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        std::swap(other.p_, p_);
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
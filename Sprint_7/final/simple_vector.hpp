#pragma once
#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iterator>
#include <utility>

//====================================================================================
// ��������� - ������� ��� ������������ SimpleVector � ���������������
// ���� ��� �����, ����� ���� ���� ReserveProxyObj ������, �� � � ����� ���� �� ����.
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

    void Fill(Type* begin, Type* end, const Type& value) {
        auto size = end - begin;
        for (auto i = 0; i < size; i++) {
            begin[i] = value;
        }
    }

    void Fill(Type* begin, Type* end, Type&& value) {
        auto size = end - begin;
        for (auto i = 0; i < size; i++) {
            if (i == 0) {
                begin[i] = std::move(value);
            }
            else {
                begin[i] = std::move(Type());
            }
        }
    }

public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // ������ ������ �� size ���������, ������������������ ��������� �� ���������
    explicit SimpleVector(size_t size) {
        p_ = new Type[size];
        Fill(p_, p_ + size, Type());
        size_ = size;
        capacity_ = size;
    }

    // ������ ������ �� size ���������, ������������������ ��������� value
    SimpleVector(size_t size, const Type& value) {
        p_ = new Type[size];
        Fill(p_, p_ + size, value);
        size_ = size;
        capacity_ = size;
    }

    // ������ ������ �� std::initializer_list
    SimpleVector(std::initializer_list<Type> init) {
        p_ = new Type[init.size()];
        std::copy(init.begin(), init.end(), p_);
        size_ = init.size();
        capacity_ = init.size();
    }

    // ����������� � ���������������, ������������
    // �������������� ����� - ������
    SimpleVector(ReserveProxyObj init) {
        p_ = new Type[init.capacity];
        Fill(p_, p_ + init.capacity, Type());
        capacity_ = init.capacity;
    }

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

    SimpleVector(SimpleVector&& other)
        : p_(std::move(other.p_)) {
        size_ = std::exchange(other.size_, 0);
        capacity_ = std::exchange(other.capacity_, 0);
        other.p_ = nullptr;
    }

    ~SimpleVector() {
        if (p_ != nullptr) {
            delete[] p_;
        }
    }

    //====================================================================================
    // ���������� ���������� ��������� � �������
    size_t GetSize() const noexcept {
        return size_;
    }

    // ���������� ����������� �������
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // ��������, ������ �� ������
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // ���������� ������ �� ������� � �������� index
    Type& operator[](size_t index) noexcept {
        return p_[index];
    }

    // ���������� ����������� ������ �� ������� � �������� index
    const Type& operator[](size_t index) const noexcept {
        return p_[index];
    }

    // ���������� ����������� ������ �� ������� � �������� index
    // ����������� ���������� std::out_of_range, ���� index >= size
    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("");
        }
        return *(p_ + index);
    }

    // ���������� ����������� ������ �� ������� � �������� index
    // ����������� ���������� std::out_of_range, ���� index >= size
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("");
        }
        return *(p_ + index);
    }

    // �������� ������ �������, �� ������� ��� �����������
    void Clear() noexcept {
        size_ = 0;
    }

    // �������� ������ �������.
    // ��� ���������� ������� ����� �������� �������� �������� �� ��������� ��� ���� Type
    void Resize(size_t new_size) {
        if (new_size <= size_) {
            size_ = new_size;
            return;
        }

        if (new_size <= capacity_) {
            Fill(p_ + size_, p_ + new_size, Type());
            size_ = new_size;
            return;
        }

        if (new_size > capacity_) {
            Type* tmp = p_;
            p_ = new Type[new_size];
            std::move(tmp, tmp + size_, p_);
            Fill(p_ + size_, p_ + new_size, Type());
            delete[] tmp;

            size_ = new_size;
            capacity_ = std::max(new_size, capacity_ * 2);
        }
    }
    // ������ ������� �������. 
    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            Type* tmp = p_;
            p_ = new Type[new_capacity];
            std::copy(tmp, tmp + size_, p_);
            Fill(p_ + size_, p_ + new_capacity, Type());
            delete[] tmp;

            capacity_ = new_capacity;
        }
    }

    //====================================================================================
    // ���������� �������� �� ������ �������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    Iterator begin() noexcept {
        if (capacity_ == 0) {
            return nullptr;
        }
        return p_;
    }

    // ���������� �������� �� �������, ��������� �� ���������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    Iterator end() noexcept {
        if (capacity_ == 0) {
            return nullptr;
        }
        return p_ + size_;
    }

    // ���������� ����������� �������� �� ������ �������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    ConstIterator begin() const noexcept {
        return cbegin();
    }

    // ���������� �������� �� �������, ��������� �� ���������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    ConstIterator end() const noexcept {
        return cend();
    }

    // ���������� ����������� �������� �� ������ �������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    ConstIterator cbegin() const noexcept {
        if (capacity_ == 0) {
            return nullptr;
        }
        return p_;
    }

    // ���������� �������� �� �������, ��������� �� ���������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    ConstIterator cend() const noexcept {
        if (capacity_ == 0) {
            return nullptr;
        }
        return p_ + size_;
    }

    //=======================================================================
    SimpleVector& operator=(const SimpleVector& rhs) {
        if (rhs.p_ == p_) {
            return *this;
        }
        auto tmp(rhs);
        this->swap(tmp);
        return *this;
    }

    // ��������� ������� � ����� �������
    // ��� �������� ����� ����������� ����� ����������� �������
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

    void PushBack(Type&& item) {
        if (capacity_ == 0) {
            p_ = new Type[1];
            p_[0] = std::move(item);
            size_++;
            capacity_++;
            return;
        }

        if (size_ < capacity_) {
            p_[size_] = std::move(item);
            size_++;
            return;
        }

        auto tmp = p_;
        p_ = new Type[capacity_ * 2];
        std::move(tmp, tmp + size_, begin());
        p_[size_] = std::move(item);
        delete[] tmp;

        size_++;
        capacity_ = capacity_ * 2;
    }

    // ��������� �������� value � ������� pos.
    // ���������� �������� �� ����������� ��������
    // ���� ����� �������� �������� ������ ��� �������� ���������,
    // ����������� ������� ������ ����������� �����, � ��� ������� ������������ 0 ����� ������ 1
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

    Iterator Insert(ConstIterator pos, Type&& value) {
        if (capacity_ == 0) {
            p_ = new Type[1];
            p_[0] = std::move(value);
            size_++;
            capacity_++;
            return p_;
        }

        if (size_ < capacity_) {
            std::move_backward(const_cast<Iterator>(pos), end(), end() + 1);
            size_t idx = pos - begin();
            p_[idx] = std::move(value);

            size_++;
            return p_ + idx;
        }

        auto tmp = p_;
        size_t idx = pos - begin();
        p_ = new Type[capacity_ * 2];
        std::move(tmp, tmp + idx, begin());
        p_[idx] = std::move(value);
        std::move(tmp + idx, tmp + size_, begin() + idx + 1);
        delete[] tmp;

        size_++;
        capacity_ = capacity_ * 2;
        return begin() + idx;
    }

    // "�������" ��������� ������� �������. ������ �� ������ ���� ������
    void PopBack() noexcept {
        if (size_ == 0) {
            return;
        }
        size_--;
    }


    // ������� ������� ������� � ��������� �������
    Iterator Erase(ConstIterator pos) {
        if (size_ == 0) {
            return begin();
        }

        auto idx = pos - begin();
        std::move(const_cast<Iterator>(pos) + 1, end(), begin() + idx);

        size_--;
        return begin() + idx;
    }

    // ���������� �������� � ������ ��������
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
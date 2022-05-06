#pragma once

#include <array>
#include <stdexcept>
#include <iterator>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) {
        if (a_size > capacity_) {
            throw invalid_argument("");
        }
        size_ = a_size;
    }

    T& operator[](size_t index) {
        if (index >= size_) {
            throw out_of_range("");
        }
        return arr_[index];
    }
    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw out_of_range("");
        }
        return arr_[index];
    }

    array<T, N>::iterator begin() {
        return arr_.begin();
    }
    array<T, N>::iterator end() {
        return arr_.begin() + size_;
    }
    array<T, N>::const_iterator begin() const {
        return arr_.cbegin();
    }
    array<T, N>::const_iterator end() const {
        return arr_.cbegin() + size_;
    }

    size_t Size() const {
        return size_;
    }
    size_t Capacity() const {
        return capacity_;
    }

    void PushBack(const T& value) {
        if (size_ >= capacity) {
            throw overflow_error("");
        }
        arr_[size_] = value;
        ++size_;
    }
    T PopBack() {
        if (size_ == 0) {
            throw underflow_error("");
        }
        T value = arr_[size_ - 1];
        --size_;
        return value;
    }
private:
    array<T, N> arr_;
    size_t size_;
    size_t capacity_ = N;
};
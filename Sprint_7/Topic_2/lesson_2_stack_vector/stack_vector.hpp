#pragma once

#include <array>
#include <stdexcept>

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) {
        if (a_size <= N) {
            size_ = a_size;
        }
        else {
            throw invalid_argument("");
        }
    };

    T& operator[](size_t index) {
        return array_.at(index);
    };

    const T& operator[](size_t index) const {
        return array_.at(index);
    };

    auto begin() {
        return array_.begin();
    };

    auto end() {
        return (array_.begin() + size_);
    };

    auto begin() const {
        return array_.cbegin();
    };
  
    auto end() const {
        return (array_.cbegin() + size_);
    };

    size_t Size() const {
        return size_;
    };

    size_t Capacity() const {
        return N;
    };

    void PushBack(const T& value) {
        if (size_ == N) {
            throw overflow_error("");
        }

        array_[size_] = value;
        size_++;
    };

    T PopBack() {
        if (size_ == 0) {
            throw underflow_error("");
        }
        return array_[--size_];
    };

private:
    std::array<T, N> array_;
    size_t size_;
};
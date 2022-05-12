#include <stdexcept> // содержит std::logic_error

template <typename T>
class ArrayPtr {
public:
    ArrayPtr() = default;

    explicit ArrayPtr(T* raw_ptr) noexcept
        : ptr_(raw_ptr) {
    }

    explicit ArrayPtr(size_t size) noexcept
        : ptr_(new T[size]) {
    }

    // Запрещаем копирование указателя
    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr() {
        if (ptr_) {
            delete[] ptr_;
        }
    }

    T* GetRawPtr() const noexcept {
        return ptr_;
    }

    void SetRawPtr(T* raw_ptr) noexcept {
        assert(!ptr_);
        ptr_ = raw_ptr;
    }

    void SetRawPtr(size_t size) noexcept {
        assert(!ptr_);
        ptr_ = new T[size];
    }

    T* Release() noexcept {
        T* p = ptr_;
        ptr_ = nullptr;
        return p;
    }
    // =========================================================
    explicit operator bool() const {
        return ptr_ != nullptr;
    }

    T* operator->() const {
        using namespace std::literals;
        if (!ptr_) {
            throw std::logic_error("Scoped ptr is null"s);
        }
        return ptr_;
    }

    T& operator*() const {
        using namespace std::literals;
        if (!ptr_) {
            throw std::logic_error("Scoped ptr is null"s);
        }
        return *ptr_;
    }
    // =========================================================

private:
    T* ptr_ = nullptr;
};
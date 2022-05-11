#include <cassert>
#include <string>
#include <iostream>

using namespace std;

// ����� ���������, ��������� ��������� ������ ��� ���� ����������.
// �������� ������� T ����� ��� �������, �� ������� ��������� ���������
template <typename T>
class ScopedPtr {
public:
    // ����������� �� ��������� ������ ������� ���������,
    // ��� ��� ���� ptr_ ����� �������� �� ��������� nullptr
    ScopedPtr() = default;

    // ������ ���������, ����������� �� ���������� raw_ptr.
    // raw_ptr ��������� ���� �� ������, ��������� � ���� ��� ������ new,
    // ���� �������� ������� ����������
    // ������������ noexcept ����������, ��� ����� �� ������� ����������
    explicit ScopedPtr(T* raw_ptr) noexcept {
        // ���������� ��������������
        ptr_ = raw_ptr;
    }

    // ������� � ������ ����������� �����������
    ScopedPtr(const ScopedPtr&) = delete;

    // ����������. ������� ������, �� ������� ��������� ����� ���������.
    ~ScopedPtr() {
        // ���������� ���� ����������� ��������������
        delete ptr_;
    }

    // ���������� ���������, ���������� ������ ScopedPtr
    T* GetRawPtr() const noexcept {
        // �������� ��� ������ ��������������
        return ptr_;
    }

    // ���������� �������� ��������, �� ������� ��������� ����� ���������.
    // ���������� ������� �������� "������" ��������� � ������������� ���� ptr_ � null
    T* Release() noexcept {
        // ���������� ��������������
        auto ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }

    //========================================
    // �������� ���������� � ���� bool ��������� ������, ��������� �� ����� ���������
    // �� �����-���� ������
    explicit operator bool() const noexcept {
        // ���������� ��������������
        if (ptr_ == nullptr)
            return false;
        return true;
    }

    // �������� ������������� ���������� ������ �� ������
    // ����������� ���������� std::logic_error, ���� ��������� �������
    T& operator*() const {
        // ���������� ��������������
        if (ptr_ == nullptr)
            throw std::logic_error("null pointer");

        return *ptr_;
    }

    // �������� -> ������ ���������� ��������� �� ������
    // ����������� ���������� std::logic_error, ���� ��������� �������
    T* operator->() const {
        // ���������� ��������������
        if (ptr_ == nullptr)
            throw std::logic_error("null pointer");

        return ptr_;
    }
    //========================================

private:
    T* ptr_ = nullptr;
};
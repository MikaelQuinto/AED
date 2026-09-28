#ifndef AED_STACK_H
#define AED_STACK_H

#include "LinkedList.h"

#include <stdexcept>

namespace adt_detail {

template <typename T>
class DynamicArray {
private:
    T* data_;
    int size_;
    int capacity_;

    void grow() {
        int newCapacity = capacity_ == 0 ? 1 : capacity_ * 2;
        T* newData = new T[newCapacity];

        for (int i = 0; i < size_; ++i) {
            newData[i] = data_[i];
        }

        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }

public:
    DynamicArray() : data_(nullptr), size_(0), capacity_(0) {}

    DynamicArray(const DynamicArray& other)
        : data_(other.capacity_ == 0 ? nullptr : new T[other.capacity_]),
          size_(other.size_), capacity_(other.capacity_) {
        for (int i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this == &other) {
            return *this;
        }

        T* newData = other.capacity_ == 0
                         ? nullptr
                         : new T[other.capacity_];

        for (int i = 0; i < other.size_; ++i) {
            newData[i] = other.data_[i];
        }

        delete[] data_;
        data_ = newData;
        size_ = other.size_;
        capacity_ = other.capacity_;
        return *this;
    }

    ~DynamicArray() {
        delete[] data_;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            grow();
        }
        data_[size_] = value;
        ++size_;
    }

    void push_front(const T& value) {
        if (size_ == capacity_) {
            grow();
        }

        for (int i = size_; i > 0; --i) {
            data_[i] = data_[i - 1];
        }

        data_[0] = value;
        ++size_;
    }

    bool pop_back() {
        if (size_ == 0) {
            return false;
        }
        --size_;
        return true;
    }

    bool pop_front() {
        if (size_ == 0) {
            return false;
        }

        for (int i = 1; i < size_; ++i) {
            data_[i - 1] = data_[i];
        }

        --size_;
        return true;
    }

    T& front() {
        if (size_ == 0) {
            throw std::runtime_error("Contenedor vacio");
        }
        return data_[0];
    }

    const T& front() const {
        if (size_ == 0) {
            throw std::runtime_error("Contenedor vacio");
        }
        return data_[0];
    }

    T& back() {
        if (size_ == 0) {
            throw std::runtime_error("Contenedor vacio");
        }
        return data_[size_ - 1];
    }

    const T& back() const {
        if (size_ == 0) {
            throw std::runtime_error("Contenedor vacio");
        }
        return data_[size_ - 1];
    }

    int size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }
};

} // namespace adt_detail

template <typename T>
class StackList {
private:
    SinglyLinkedList<T> data_;

public:
    StackList() = default;

    void push(const T& value) {
        data_.push_front(value);
    }

    bool pop() {
        return data_.pop_front();
    }

    T& top() {
        return data_.front();
    }

    const T& top() const {
        return data_.front();
    }

    int size() const {
        return data_.size();
    }

    bool empty() const {
        return data_.empty();
    }
};

template <typename T>
class StackTwoQueues {
private:
    SinglyLinkedList<T> main_;
    SinglyLinkedList<T> auxiliary_;

public:
    StackTwoQueues() = default;

    void push(const T& value) {
        auxiliary_.push_back(value);

        while (!main_.empty()) {
            auxiliary_.push_back(main_.front());
            main_.pop_front();
        }

        main_ = auxiliary_;
        auxiliary_.clear();
    }

    bool pop() {
        return main_.pop_front();
    }

    T& top() {
        return main_.front();
    }

    const T& top() const {
        return main_.front();
    }

    int size() const {
        return main_.size();
    }

    bool empty() const {
        return main_.empty();
    }
};

template <typename T>
class StackVector {
private:
    adt_detail::DynamicArray<T> data_;

public:
    StackVector() = default;

    void push(const T& value) {
        data_.push_back(value);
    }

    bool pop() {
        return data_.pop_back();
    }

    T& top() {
        return data_.back();
    }

    const T& top() const {
        return data_.back();
    }

    int size() const {
        return data_.size();
    }

    bool empty() const {
        return data_.empty();
    }
};

#endif // AED_STACK_H

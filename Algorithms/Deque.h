#ifndef AED_DEQUE_H
#define AED_DEQUE_H

#include "Stack.h"

#include <stdexcept>

template <typename T>
class DequeList {
private:
    DoublyLinkedList<T> data_;

public:
    DequeList() = default;

    void push_front(const T& value) {
        data_.push_front(value);
    }

    void push_back(const T& value) {
        data_.push_back(value);
    }

    bool pop_front() {
        return data_.pop_front();
    }

    bool pop_back() {
        return data_.pop_back();
    }

    T& front() {
        return data_.front();
    }

    const T& front() const {
        return data_.front();
    }

    T& back() {
        return data_.back();
    }

    const T& back() const {
        return data_.back();
    }

    int size() const {
        return data_.size();
    }

    bool empty() const {
        return data_.empty();
    }
};

template <typename T>
class DequeVector {
private:
    adt_detail::DynamicArray<T> data_;

public:
    DequeVector() = default;

    void push_front(const T& value) {
        data_.push_front(value);
    }

    void push_back(const T& value) {
        data_.push_back(value);
    }

    bool pop_front() {
        return data_.pop_front();
    }

    bool pop_back() {
        return data_.pop_back();
    }

    T& front() {
        return data_.front();
    }

    const T& front() const {
        return data_.front();
    }

    T& back() {
        return data_.back();
    }

    const T& back() const {
        return data_.back();
    }

    int size() const {
        return data_.size();
    }

    bool empty() const {
        return data_.empty();
    }
};

template <typename T>
class DequeCircular {
private:
    T* data_;
    int capacity_;
    int front_;
    int size_;

    int position(int index) const {
        return (front_ + index) % capacity_;
    }

    void grow() {
        int newCapacity = capacity_ * 2;
        T* newData = new T[newCapacity];

        for (int i = 0; i < size_; ++i) {
            newData[i] = data_[position(i)];
        }

        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
        front_ = 0;
    }

public:
    explicit DequeCircular(int capacity = 4)
        : data_(nullptr), capacity_(capacity), front_(0), size_(0) {
        if (capacity_ < 1) {
            capacity_ = 1;
        }
        data_ = new T[capacity_];
    }

    DequeCircular(const DequeCircular& other)
        : data_(new T[other.capacity_]), capacity_(other.capacity_),
          front_(0), size_(other.size_) {
        for (int i = 0; i < size_; ++i) {
            data_[i] = other.data_[other.position(i)];
        }
    }

    DequeCircular& operator=(const DequeCircular& other) {
        if (this == &other) {
            return *this;
        }

        T* newData = new T[other.capacity_];
        for (int i = 0; i < other.size_; ++i) {
            newData[i] = other.data_[other.position(i)];
        }

        delete[] data_;
        data_ = newData;
        capacity_ = other.capacity_;
        front_ = 0;
        size_ = other.size_;
        return *this;
    }

    ~DequeCircular() {
        delete[] data_;
    }

    void push_front(const T& value) {
        if (size_ == capacity_) {
            grow();
        }

        front_ = (front_ - 1 + capacity_) % capacity_;
        data_[front_] = value;
        ++size_;
    }

    void push_back(const T& value) {
        if (size_ == capacity_) {
            grow();
        }

        data_[position(size_)] = value;
        ++size_;
    }

    bool pop_front() {
        if (size_ == 0) {
            return false;
        }

        front_ = (front_ + 1) % capacity_;
        --size_;
        return true;
    }

    bool pop_back() {
        if (size_ == 0) {
            return false;
        }

        --size_;
        return true;
    }

    T& front() {
        if (size_ == 0) {
            throw std::runtime_error("Deque vacio");
        }
        return data_[front_];
    }

    const T& front() const {
        if (size_ == 0) {
            throw std::runtime_error("Deque vacio");
        }
        return data_[front_];
    }

    T& back() {
        if (size_ == 0) {
            throw std::runtime_error("Deque vacio");
        }
        return data_[position(size_ - 1)];
    }

    const T& back() const {
        if (size_ == 0) {
            throw std::runtime_error("Deque vacio");
        }
        return data_[position(size_ - 1)];
    }

    int size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }
};

#endif // AED_DEQUE_H

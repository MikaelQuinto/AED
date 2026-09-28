#ifndef AED_VECTOR_H
#define AED_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

template <typename T>
class Vector {
public:
    using size_type = std::size_t;

private:
    T* data_;
    size_type size_;
    size_type capacity_;

    void changeCapacity(size_type newCapacity) {
        T* newData = new T[newCapacity];

        for (size_type i = 0; i < size_; ++i) {
            newData[i] = std::move(data_[i]);
        }

        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }

public:
    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    explicit Vector(size_type count)
        : data_(count == 0 ? nullptr : new T[count]{}),
          size_(count),
          capacity_(count) {}

    Vector(size_type count, const T& value)
        : data_(count == 0 ? nullptr : new T[count]),
          size_(count),
          capacity_(count) {
        for (size_type i = 0; i < size_; ++i) {
            data_[i] = value;
        }
    }

    Vector(std::initializer_list<T> values)
        : data_(values.size() == 0 ? nullptr : new T[values.size()]),
          size_(values.size()),
          capacity_(values.size()) {
        size_type i = 0;
        for (const T& value : values) {
            data_[i] = value;
            ++i;
        }
    }

    Vector(const Vector& other)
        : data_(other.capacity_ == 0 ? nullptr : new T[other.capacity_]),
          size_(other.size_),
          capacity_(other.capacity_) {
        for (size_type i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    Vector(Vector&& other) noexcept
        : data_(other.data_),
          size_(other.size_),
          capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~Vector() {
        delete[] data_;
    }

    Vector& operator=(const Vector& other) {
        if (this == &other) {
            return *this;
        }

        T* newData = other.capacity_ == 0
                         ? nullptr
                         : new T[other.capacity_];

        for (size_type i = 0; i < other.size_; ++i) {
            newData[i] = other.data_[i];
        }

        delete[] data_;
        data_ = newData;
        size_ = other.size_;
        capacity_ = other.capacity_;

        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        delete[] data_;

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }

    void push_back(T value) {
        if (size_ == capacity_) {
            size_type newCapacity = capacity_ == 0 ? 1 : capacity_ * 2;
            changeCapacity(newCapacity);
        }

        data_[size_] = std::move(value);
        ++size_;
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }

    T& operator[](size_type index) {
        return data_[index];
    }

    const T& operator[](size_type index) const {
        return data_[index];
    }

    T& at(size_type index) {
        if (index >= size_) {
            throw std::out_of_range("Vector index out of range");
        }
        return data_[index];
    }

    const T& at(size_type index) const {
        if (index >= size_) {
            throw std::out_of_range("Vector index out of range");
        }
        return data_[index];
    }

    T& front() {
        return data_[0];
    }

    const T& front() const {
        return data_[0];
    }

    T& back() {
        return data_[size_ - 1];
    }

    const T& back() const {
        return data_[size_ - 1];
    }

    T* data() {
        return data_;
    }

    const T* data() const {
        return data_;
    }

    size_type size() const {
        return size_;
    }

    size_type capacity() const {
        return capacity_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void clear() {
        size_ = 0;
    }

    void reserve(size_type newCapacity) {
        if (newCapacity > capacity_) {
            changeCapacity(newCapacity);
        }
    }

    void resize(size_type newSize) {
        resize(newSize, T{});
    }

    void resize(size_type newSize, const T& value) {
        if (newSize > capacity_) {
            size_type newCapacity = capacity_ == 0 ? 1 : capacity_;

            while (newCapacity < newSize) {
                newCapacity *= 2;
            }

            changeCapacity(newCapacity);
        }

        for (size_type i = size_; i < newSize; ++i) {
            data_[i] = value;
        }

        size_ = newSize;
    }

    T* begin() {
        return data_;
    }

    const T* begin() const {
        return data_;
    }

    T* end() {
        return data_ == nullptr ? nullptr : data_ + size_;
    }

    const T* end() const {
        return data_ == nullptr ? nullptr : data_ + size_;
    }
};

#endif // AED_VECTOR_H

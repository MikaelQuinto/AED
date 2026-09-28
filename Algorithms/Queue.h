#ifndef AED_QUEUE_H
#define AED_QUEUE_H

#include "Stack.h"

template <typename T>
class QueueList {
private:
    SinglyLinkedList<T> data_;

public:
    QueueList() = default;

    void enqueue(const T& value) {
        data_.push_back(value);
    }

    bool dequeue() {
        return data_.pop_front();
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
class QueueTwoStacks {
private:
    StackList<T> input_;
    StackList<T> output_;

    void prepareOutput() {
        if (!output_.empty()) {
            return;
        }

        while (!input_.empty()) {
            T value = input_.top();
            input_.pop();
            output_.push(value);
        }
    }

public:
    QueueTwoStacks() = default;

    void enqueue(const T& value) {
        input_.push(value);
    }

    bool dequeue() {
        prepareOutput();
        return output_.pop();
    }

    T& front() {
        prepareOutput();
        return output_.top();
    }

    int size() const {
        return input_.size() + output_.size();
    }

    bool empty() const {
        return input_.empty() && output_.empty();
    }
};

template <typename T>
class QueueVector {
private:
    adt_detail::DynamicArray<T> data_;

public:
    QueueVector() = default;

    void enqueue(const T& value) {
        data_.push_back(value);
    }

    bool dequeue() {
        return data_.pop_front();
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

#endif // AED_QUEUE_H

#ifndef AED_LINKED_LIST_H
#define AED_LINKED_LIST_H

#include <iostream>

template <typename T>
class SinglyLinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    int size_;

    void copyFrom(const SinglyLinkedList& other) {
        Node* current = other.head_;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }

public:
    SinglyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    SinglyLinkedList(const SinglyLinkedList& other)
        : head_(nullptr), tail_(nullptr), size_(0) {
        copyFrom(other);
    }

    SinglyLinkedList& operator=(const SinglyLinkedList& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    ~SinglyLinkedList() {
        clear();
    }

    void push_front(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = head_;
        head_ = newNode;

        if (tail_ == nullptr) {
            tail_ = newNode;
        }

        ++size_;
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);

        if (tail_ == nullptr) {
            head_ = newNode;
            tail_ = newNode;
        } else {
            tail_->next = newNode;
            tail_ = newNode;
        }

        ++size_;
    }

    bool pop_front() {
        if (head_ == nullptr) {
            return false;
        }

        Node* oldHead = head_;
        head_ = head_->next;
        delete oldHead;
        --size_;

        if (head_ == nullptr) {
            tail_ = nullptr;
        }

        return true;
    }

    bool pop_back() {
        if (head_ == nullptr) {
            return false;
        }

        if (head_ == tail_) {
            delete head_;
            head_ = nullptr;
            tail_ = nullptr;
            size_ = 0;
            return true;
        }

        Node* current = head_;
        while (current->next != tail_) {
            current = current->next;
        }

        delete tail_;
        tail_ = current;
        tail_->next = nullptr;
        --size_;
        return true;
    }

    bool search(const T& value) const {
        Node* current = head_;

        while (current != nullptr) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }

        return false;
    }

    bool insert(int position, const T& value) {
        if (position < 0 || position > size_) {
            return false;
        }

        if (position == 0) {
            push_front(value);
            return true;
        }

        if (position == size_) {
            push_back(value);
            return true;
        }

        Node* previous = head_;
        for (int i = 1; i < position; ++i) {
            previous = previous->next;
        }

        Node* newNode = new Node(value);
        newNode->next = previous->next;
        previous->next = newNode;
        ++size_;
        return true;
    }

    bool remove(const T& value) {
        if (head_ == nullptr) {
            return false;
        }

        if (head_->data == value) {
            return pop_front();
        }

        Node* previous = head_;
        Node* current = head_->next;

        while (current != nullptr && current->data != value) {
            previous = current;
            current = current->next;
        }

        if (current == nullptr) {
            return false;
        }

        previous->next = current->next;
        if (current == tail_) {
            tail_ = previous;
        }

        delete current;
        --size_;
        return true;
    }

    void merge(const SinglyLinkedList& other) {
        Node* current = other.head_;
        int count = other.size_;

        for (int i = 0; i < count; ++i) {
            push_back(current->data);
            current = current->next;
        }
    }

    void clear() {
        while (head_ != nullptr) {
            Node* next = head_->next;
            delete head_;
            head_ = next;
        }

        tail_ = nullptr;
        size_ = 0;
    }

    int size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void print() const {
        Node* current = head_;
        while (current != nullptr) {
            std::cout << current->data << ' ';
            current = current->next;
        }
        std::cout << '\n';
    }
};

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T data;
        Node* previous;
        Node* next;

        Node(const T& value)
            : data(value), previous(nullptr), next(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    int size_;

    void copyFrom(const DoublyLinkedList& other) {
        Node* current = other.head_;
        while (current != nullptr) {
            push_back(current->data);
            current = current->next;
        }
    }

public:
    DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

    DoublyLinkedList(const DoublyLinkedList& other)
        : head_(nullptr), tail_(nullptr), size_(0) {
        copyFrom(other);
    }

    DoublyLinkedList& operator=(const DoublyLinkedList& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    ~DoublyLinkedList() {
        clear();
    }

    void push_front(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = head_;

        if (head_ == nullptr) {
            tail_ = newNode;
        } else {
            head_->previous = newNode;
        }

        head_ = newNode;
        ++size_;
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        newNode->previous = tail_;

        if (tail_ == nullptr) {
            head_ = newNode;
        } else {
            tail_->next = newNode;
        }

        tail_ = newNode;
        ++size_;
    }

    bool pop_front() {
        if (head_ == nullptr) {
            return false;
        }

        Node* oldHead = head_;
        head_ = head_->next;
        delete oldHead;
        --size_;

        if (head_ == nullptr) {
            tail_ = nullptr;
        } else {
            head_->previous = nullptr;
        }

        return true;
    }

    bool pop_back() {
        if (tail_ == nullptr) {
            return false;
        }

        Node* oldTail = tail_;
        tail_ = tail_->previous;
        delete oldTail;
        --size_;

        if (tail_ == nullptr) {
            head_ = nullptr;
        } else {
            tail_->next = nullptr;
        }

        return true;
    }

    bool search(const T& value) const {
        Node* current = head_;

        while (current != nullptr) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }

        return false;
    }

    bool insert(int position, const T& value) {
        if (position < 0 || position > size_) {
            return false;
        }

        if (position == 0) {
            push_front(value);
            return true;
        }

        if (position == size_) {
            push_back(value);
            return true;
        }

        Node* current = head_;
        for (int i = 0; i < position; ++i) {
            current = current->next;
        }

        Node* newNode = new Node(value);
        newNode->previous = current->previous;
        newNode->next = current;
        current->previous->next = newNode;
        current->previous = newNode;
        ++size_;
        return true;
    }

    bool remove(const T& value) {
        Node* current = head_;

        while (current != nullptr && current->data != value) {
            current = current->next;
        }

        if (current == nullptr) {
            return false;
        }

        if (current == head_) {
            return pop_front();
        }

        if (current == tail_) {
            return pop_back();
        }

        current->previous->next = current->next;
        current->next->previous = current->previous;
        delete current;
        --size_;
        return true;
    }

    void merge(const DoublyLinkedList& other) {
        Node* current = other.head_;
        int count = other.size_;

        for (int i = 0; i < count; ++i) {
            push_back(current->data);
            current = current->next;
        }
    }

    void clear() {
        while (head_ != nullptr) {
            Node* next = head_->next;
            delete head_;
            head_ = next;
        }

        tail_ = nullptr;
        size_ = 0;
    }

    int size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void print() const {
        Node* current = head_;
        while (current != nullptr) {
            std::cout << current->data << ' ';
            current = current->next;
        }
        std::cout << '\n';
    }

    void print_reverse() const {
        Node* current = tail_;
        while (current != nullptr) {
            std::cout << current->data << ' ';
            current = current->previous;
        }
        std::cout << '\n';
    }
};

template <typename T>
class CircularLinkedList {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* tail_;
    int size_;

    void copyFrom(const CircularLinkedList& other) {
        if (other.tail_ == nullptr) {
            return;
        }

        Node* current = other.tail_->next;
        for (int i = 0; i < other.size_; ++i) {
            push_back(current->data);
            current = current->next;
        }
    }

public:
    CircularLinkedList() : tail_(nullptr), size_(0) {}

    CircularLinkedList(const CircularLinkedList& other)
        : tail_(nullptr), size_(0) {
        copyFrom(other);
    }

    CircularLinkedList& operator=(const CircularLinkedList& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    ~CircularLinkedList() {
        clear();
    }

    void push_front(const T& value) {
        Node* newNode = new Node(value);

        if (tail_ == nullptr) {
            tail_ = newNode;
            tail_->next = tail_;
        } else {
            newNode->next = tail_->next;
            tail_->next = newNode;
        }

        ++size_;
    }

    void push_back(const T& value) {
        push_front(value);
        tail_ = tail_->next;
    }

    bool pop_front() {
        if (tail_ == nullptr) {
            return false;
        }

        Node* head = tail_->next;

        if (head == tail_) {
            delete head;
            tail_ = nullptr;
        } else {
            tail_->next = head->next;
            delete head;
        }

        --size_;
        return true;
    }

    bool pop_back() {
        if (tail_ == nullptr) {
            return false;
        }

        if (tail_->next == tail_) {
            delete tail_;
            tail_ = nullptr;
            size_ = 0;
            return true;
        }

        Node* previous = tail_->next;
        while (previous->next != tail_) {
            previous = previous->next;
        }

        previous->next = tail_->next;
        delete tail_;
        tail_ = previous;
        --size_;
        return true;
    }

    bool search(const T& value) const {
        if (tail_ == nullptr) {
            return false;
        }

        Node* current = tail_->next;
        for (int i = 0; i < size_; ++i) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }

        return false;
    }

    bool insert(int position, const T& value) {
        if (position < 0 || position > size_) {
            return false;
        }

        if (position == 0) {
            push_front(value);
            return true;
        }

        if (position == size_) {
            push_back(value);
            return true;
        }

        Node* previous = tail_->next;
        for (int i = 1; i < position; ++i) {
            previous = previous->next;
        }

        Node* newNode = new Node(value);
        newNode->next = previous->next;
        previous->next = newNode;
        ++size_;
        return true;
    }

    bool remove(const T& value) {
        if (tail_ == nullptr) {
            return false;
        }

        Node* previous = tail_;
        Node* current = tail_->next;

        for (int i = 0; i < size_; ++i) {
            if (current->data == value) {
                if (size_ == 1) {
                    delete current;
                    tail_ = nullptr;
                    size_ = 0;
                    return true;
                }

                previous->next = current->next;
                if (current == tail_) {
                    tail_ = previous;
                }

                delete current;
                --size_;
                return true;
            }

            previous = current;
            current = current->next;
        }

        return false;
    }

    void merge(const CircularLinkedList& other) {
        if (other.tail_ == nullptr) {
            return;
        }

        Node* current = other.tail_->next;
        int count = other.size_;

        for (int i = 0; i < count; ++i) {
            push_back(current->data);
            current = current->next;
        }
    }

    void clear() {
        if (tail_ == nullptr) {
            return;
        }

        Node* current = tail_->next;
        tail_->next = nullptr;

        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }

        tail_ = nullptr;
        size_ = 0;
    }

    int size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void print() const {
        if (tail_ != nullptr) {
            Node* current = tail_->next;
            for (int i = 0; i < size_; ++i) {
                std::cout << current->data << ' ';
                current = current->next;
            }
        }
        std::cout << '\n';
    }
};

#endif // AED_LINKED_LIST_H

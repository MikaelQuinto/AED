#ifndef AED_HASH_TABLE_H
#define AED_HASH_TABLE_H

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

template <typename Key>
struct HashFunction;

template <>
struct HashFunction<int> {
    std::size_t operator()(int key) const {
        long long value = key;
        if (value < 0) {
            value = -value;
        }
        return static_cast<std::size_t>(value);
    }
};

template <>
struct HashFunction<std::string> {
    std::size_t operator()(const std::string& key) const {
        std::size_t hash = 0;

        for (char character : key) {
            hash = hash * 31 + static_cast<unsigned char>(character);
        }

        return hash;
    }
};

template <typename Key, typename Value>
class HashTable {
private:
    struct Node {
        Key key;
        Value value;
        Node* next;

        Node(const Key& newKey, const Value& newValue, Node* nextNode)
            : key(newKey), value(newValue), next(nextNode) {}
    };

    Node** buckets_;
    int capacity_;
    int size_;
    HashFunction<Key> hashFunction_;

    int bucketIndex(const Key& key) const {
        return static_cast<int>(hashFunction_(key) % capacity_);
    }

    Node* findNode(const Key& key) {
        Node* current = buckets_[bucketIndex(key)];

        while (current != nullptr) {
            if (current->key == key) {
                return current;
            }
            current = current->next;
        }

        return nullptr;
    }

    const Node* findNode(const Key& key) const {
        const Node* current = buckets_[bucketIndex(key)];

        while (current != nullptr) {
            if (current->key == key) {
                return current;
            }
            current = current->next;
        }

        return nullptr;
    }

    void rehash(int newCapacity) {
        Node** newBuckets = new Node*[newCapacity]{};

        for (int i = 0; i < capacity_; ++i) {
            Node* current = buckets_[i];

            while (current != nullptr) {
                Node* next = current->next;
                int newIndex = static_cast<int>(
                    hashFunction_(current->key) % newCapacity
                );

                current->next = newBuckets[newIndex];
                newBuckets[newIndex] = current;
                current = next;
            }
        }

        delete[] buckets_;
        buckets_ = newBuckets;
        capacity_ = newCapacity;
    }

    void copyFrom(const HashTable& other) {
        for (int i = 0; i < other.capacity_; ++i) {
            const Node* current = other.buckets_[i];

            while (current != nullptr) {
                insert(current->key, current->value);
                current = current->next;
            }
        }
    }

public:
    explicit HashTable(int capacity = 11)
        : buckets_(nullptr), capacity_(capacity), size_(0) {
        if (capacity_ < 1) {
            capacity_ = 1;
        }
        buckets_ = new Node*[capacity_]{};
    }

    HashTable(const HashTable& other)
        : buckets_(new Node*[other.capacity_]{}),
          capacity_(other.capacity_), size_(0) {
        copyFrom(other);
    }

    HashTable& operator=(const HashTable& other) {
        if (this == &other) {
            return *this;
        }

        clear();
        delete[] buckets_;

        capacity_ = other.capacity_;
        buckets_ = new Node*[capacity_]{};
        copyFrom(other);
        return *this;
    }

    ~HashTable() {
        clear();
        delete[] buckets_;
    }

    void insert(const Key& key, const Value& value) {
        Node* existing = findNode(key);

        if (existing != nullptr) {
            existing->value = value;
            return;
        }

        if ((size_ + 1) * 4 > capacity_ * 3) {
            rehash(capacity_ * 2);
        }

        int index = bucketIndex(key);
        buckets_[index] = new Node(key, value, buckets_[index]);
        ++size_;
    }

    bool remove(const Key& key) {
        int index = bucketIndex(key);
        Node* current = buckets_[index];
        Node* previous = nullptr;

        while (current != nullptr && current->key != key) {
            previous = current;
            current = current->next;
        }

        if (current == nullptr) {
            return false;
        }

        if (previous == nullptr) {
            buckets_[index] = current->next;
        } else {
            previous->next = current->next;
        }

        delete current;
        --size_;
        return true;
    }

    bool contains(const Key& key) const {
        return findNode(key) != nullptr;
    }

    Value& at(const Key& key) {
        Node* node = findNode(key);

        if (node == nullptr) {
            throw std::out_of_range("Clave no encontrada");
        }

        return node->value;
    }

    const Value& at(const Key& key) const {
        const Node* node = findNode(key);

        if (node == nullptr) {
            throw std::out_of_range("Clave no encontrada");
        }

        return node->value;
    }

    Value& operator[](const Key& key) {
        Node* node = findNode(key);

        if (node == nullptr) {
            insert(key, Value{});
            node = findNode(key);
        }

        return node->value;
    }

    void clear() {
        for (int i = 0; i < capacity_; ++i) {
            Node* current = buckets_[i];

            while (current != nullptr) {
                Node* next = current->next;
                delete current;
                current = next;
            }

            buckets_[i] = nullptr;
        }

        size_ = 0;
    }

    int size() const {
        return size_;
    }

    int bucket_count() const {
        return capacity_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void print() const {
        for (int i = 0; i < capacity_; ++i) {
            std::cout << i << ':';
            const Node* current = buckets_[i];

            while (current != nullptr) {
                std::cout << " [" << current->key
                          << " -> " << current->value << ']';
                current = current->next;
            }

            std::cout << '\n';
        }
    }
};

#endif // AED_HASH_TABLE_H

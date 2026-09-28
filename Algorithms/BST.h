#ifndef AED_BST_H
#define AED_BST_H

#include <iostream>
#include <queue>
#include <string>

template <typename T, typename V>
struct node {
    T key;
    V value;
    node* child[2] = {};
};

template <typename T, typename V>
class BST {
private:
    node<T, V>* root_;
    int size_;

    static node<T, V>* minimum(node<T, V>* current) {
        if (current == nullptr) {
            return nullptr;
        }

        while (current->child[0] != nullptr) {
            current = current->child[0];
        }
        return current;
    }

    static node<T, V>* maximum(node<T, V>* current) {
        if (current == nullptr) {
            return nullptr;
        }

        while (current->child[1] != nullptr) {
            current = current->child[1];
        }
        return current;
    }

    static node<T, V>* clone(const node<T, V>* current) {
        if (current == nullptr) {
            return nullptr;
        }

        node<T, V>* copy = new node<T, V>{current->key, current->value};
        copy->child[0] = clone(current->child[0]);
        copy->child[1] = clone(current->child[1]);
        return copy;
    }

    static void destroy(node<T, V>* current) {
        if (current == nullptr) {
            return;
        }

        destroy(current->child[0]);
        destroy(current->child[1]);
        delete current;
    }

    node<T, V>* removeNode(node<T, V>* current,
                           const T& key,
                           bool& removed) {
        if (current == nullptr) {
            return nullptr;
        }

        if (key < current->key) {
            current->child[0] = removeNode(current->child[0], key, removed);
        } else if (current->key < key) {
            current->child[1] = removeNode(current->child[1], key, removed);
        } else {
            if (current->child[0] == nullptr) {
                node<T, V>* right = current->child[1];
                delete current;
                removed = true;
                return right;
            }

            if (current->child[1] == nullptr) {
                node<T, V>* left = current->child[0];
                delete current;
                removed = true;
                return left;
            }

            node<T, V>* next = minimum(current->child[1]);
            current->key = next->key;
            current->value = next->value;
            current->child[1] = removeNode(
                current->child[1], next->key, removed
            );
        }

        return current;
    }

    static void printNode(const node<T, V>* current) {
        std::cout << current->key << ':' << current->value << ' ';
    }

    static void printInOrder(const node<T, V>* current) {
        if (current == nullptr) {
            return;
        }

        printInOrder(current->child[0]);
        printNode(current);
        printInOrder(current->child[1]);
    }

    static void printPreOrder(const node<T, V>* current) {
        if (current == nullptr) {
            return;
        }

        printNode(current);
        printPreOrder(current->child[0]);
        printPreOrder(current->child[1]);
    }

    static void printPostOrder(const node<T, V>* current) {
        if (current == nullptr) {
            return;
        }

        printPostOrder(current->child[0]);
        printPostOrder(current->child[1]);
        printNode(current);
    }

public:
    class iterator {
    private:
        node<T, V>* root_;
        node<T, V>* current_;

    public:
        iterator(node<T, V>* root, node<T, V>* current)
            : root_(root), current_(current) {}

        node<T, V>& operator*() const {
            return *current_;
        }

        node<T, V>* operator->() const {
            return current_;
        }

        iterator& operator++() {
            if (current_ == nullptr) {
                return *this;
            }

            if (current_->child[1] != nullptr) {
                current_ = BST::minimum(current_->child[1]);
                return *this;
            }

            node<T, V>* next = nullptr;
            node<T, V>* ancestor = root_;

            while (ancestor != nullptr) {
                if (current_->key < ancestor->key) {
                    next = ancestor;
                    ancestor = ancestor->child[0];
                } else if (ancestor->key < current_->key) {
                    ancestor = ancestor->child[1];
                } else {
                    break;
                }
            }

            current_ = next;
            return *this;
        }

        iterator operator++(int) {
            iterator previous = *this;
            ++(*this);
            return previous;
        }

        bool operator==(const iterator& other) const {
            return current_ == other.current_;
        }

        bool operator!=(const iterator& other) const {
            return current_ != other.current_;
        }
    };

    BST() : root_(nullptr), size_(0) {}

    BST(const BST& other)
        : root_(clone(other.root_)), size_(other.size_) {}

    BST& operator=(const BST& other) {
        if (this == &other) {
            return *this;
        }

        node<T, V>* newRoot = clone(other.root_);
        destroy(root_);
        root_ = newRoot;
        size_ = other.size_;
        return *this;
    }

    ~BST() {
        destroy(root_);
    }

    node<T, V>* search(const T& key) {
        node<T, V>* current = root_;

        while (current != nullptr) {
            if (key < current->key) {
                current = current->child[0];
            } else if (current->key < key) {
                current = current->child[1];
            } else {
                return current;
            }
        }

        return nullptr;
    }

    const node<T, V>* search(const T& key) const {
        const node<T, V>* current = root_;

        while (current != nullptr) {
            if (key < current->key) {
                current = current->child[0];
            } else if (current->key < key) {
                current = current->child[1];
            } else {
                return current;
            }
        }

        return nullptr;
    }

    bool insert(const T& key, const V& value) {
        if (root_ == nullptr) {
            root_ = new node<T, V>{key, value};
            ++size_;
            return true;
        }

        node<T, V>* current = root_;

        while (true) {
            if (key < current->key) {
                if (current->child[0] == nullptr) {
                    current->child[0] = new node<T, V>{key, value};
                    ++size_;
                    return true;
                }
                current = current->child[0];
            } else if (current->key < key) {
                if (current->child[1] == nullptr) {
                    current->child[1] = new node<T, V>{key, value};
                    ++size_;
                    return true;
                }
                current = current->child[1];
            } else {
                current->value = value;
                return false;
            }
        }
    }

    bool remove(const T& key) {
        bool removed = false;
        root_ = removeNode(root_, key, removed);

        if (removed) {
            --size_;
        }
        return removed;
    }

    node<T, V>* predecessor(const T& key) {
        node<T, V>* current = root_;
        node<T, V>* previous = nullptr;

        while (current != nullptr) {
            if (key < current->key) {
                current = current->child[0];
            } else if (current->key < key) {
                previous = current;
                current = current->child[1];
            } else {
                if (current->child[0] != nullptr) {
                    return maximum(current->child[0]);
                }
                return previous;
            }
        }

        return previous;
    }

    node<T, V>* successor(const T& key) {
        node<T, V>* current = root_;
        node<T, V>* next = nullptr;

        while (current != nullptr) {
            if (key < current->key) {
                next = current;
                current = current->child[0];
            } else if (current->key < key) {
                current = current->child[1];
            } else {
                if (current->child[1] != nullptr) {
                    return minimum(current->child[1]);
                }
                return next;
            }
        }

        return next;
    }

    node<T, V>* sucessor(const T& key) {
        return successor(key);
    }

    iterator begin() {
        return iterator(root_, minimum(root_));
    }

    iterator end() {
        return iterator(root_, nullptr);
    }

    int size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void in_order() const {
        printInOrder(root_);
        std::cout << '\n';
    }

    void pre_order() const {
        printPreOrder(root_);
        std::cout << '\n';
    }

    void post_order() const {
        printPostOrder(root_);
        std::cout << '\n';
    }

    void level_order() const {
        if (root_ == nullptr) {
            std::cout << '\n';
            return;
        }

        std::queue<const node<T, V>*> pending;
        pending.push(root_);

        while (!pending.empty()) {
            const node<T, V>* current = pending.front();
            pending.pop();
            printNode(current);

            if (current->child[0] != nullptr) {
                pending.push(current->child[0]);
            }
            if (current->child[1] != nullptr) {
                pending.push(current->child[1]);
            }
        }

        std::cout << '\n';
    }
};

#endif // AED_BST_H

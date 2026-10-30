#ifndef AED_ALGHORITHMS_RB_H
#define AED_ALGHORITHMS_RB_H

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

namespace aed {

// Ordered set with O(log n) insertion and lookup. Duplicate keys are ignored.
template <typename T, typename Compare = std::less<T>>
class RBTree {
private:
    enum class Color { Red, Black };

    struct Node {
        T value;
        Color color = Color::Red;
        Node* parent = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;

        explicit Node(const T& value) : value(value) {}
    };

    Compare compare_;
    Node* root_ = nullptr;
    std::size_t size_ = 0;

    static Color color_of(const Node* node) noexcept {
        // Missing children are black.
        return node ? node->color : Color::Black;
    }

    void rotate_left(Node* node) noexcept {
        Node* pivot = node->right;
        node->right = pivot->left;
        if (pivot->left) {
            pivot->left->parent = node;
        }

        pivot->parent = node->parent;
        if (!node->parent) {
            root_ = pivot;
        } else if (node == node->parent->left) {
            node->parent->left = pivot;
        } else {
            node->parent->right = pivot;
        }

        pivot->left = node;
        node->parent = pivot;
    }

    void rotate_right(Node* node) noexcept {
        Node* pivot = node->left;
        node->left = pivot->right;
        if (pivot->right) {
            pivot->right->parent = node;
        }

        pivot->parent = node->parent;
        if (!node->parent) {
            root_ = pivot;
        } else if (node == node->parent->left) {
            node->parent->left = pivot;
        } else {
            node->parent->right = pivot;
        }

        pivot->right = node;
        node->parent = pivot;
    }

    void fix_insert(Node* node) noexcept {
        while (node != root_ && color_of(node->parent) == Color::Red) {
            Node* parent = node->parent;
            Node* grandparent = parent->parent;

            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;
                if (color_of(uncle) == Color::Red) {
                    // Red uncle: recolor and continue at the grandparent.
                    parent->color = Color::Black;
                    uncle->color = Color::Black;
                    grandparent->color = Color::Red;
                    node = grandparent;
                } else {
                    if (node == parent->right) {
                        node = parent;
                        rotate_left(node); // LR -> LL
                    }
                    node->parent->color = Color::Black;
                    node->parent->parent->color = Color::Red;
                    rotate_right(node->parent->parent); // LL
                }
            } else {
                Node* uncle = grandparent->left;
                if (color_of(uncle) == Color::Red) {
                    parent->color = Color::Black;
                    uncle->color = Color::Black;
                    grandparent->color = Color::Red;
                    node = grandparent;
                } else {
                    if (node == parent->left) {
                        node = parent;
                        rotate_right(node); // RL -> RR
                    }
                    node->parent->color = Color::Black;
                    node->parent->parent->color = Color::Red;
                    rotate_left(node->parent->parent); // RR
                }
            }
        }
        root_->color = Color::Black;
    }

    static void destroy(Node* node) noexcept {
        if (!node) {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    static void collect_inorder(const Node* node, std::vector<T>& result) {
        if (!node) {
            return;
        }
        collect_inorder(node->left, result);
        result.push_back(node->value);
        collect_inorder(node->right, result);
    }

public:
    RBTree() = default;
    explicit RBTree(Compare compare) : compare_(std::move(compare)) {}
    ~RBTree() { destroy(root_); }

    RBTree(const RBTree&) = delete;
    RBTree& operator=(const RBTree&) = delete;

    RBTree(RBTree&& other)
        : compare_(std::move(other.compare_)),
          root_(std::exchange(other.root_, nullptr)),
          size_(std::exchange(other.size_, 0)) {}

    RBTree& operator=(RBTree&& other) {
        if (this != &other) {
            compare_ = std::move(other.compare_);
            destroy(root_);
            root_ = std::exchange(other.root_, nullptr);
            size_ = std::exchange(other.size_, 0);
        }
        return *this;
    }

    // Returns false if an equivalent key already exists.
    bool insert(const T& value) {
        Node* parent = nullptr;
        Node* current = root_;
        bool goes_left = false;

        while (current) {
            parent = current;
            if (compare_(value, current->value)) {
                goes_left = true;
                current = current->left;
            } else if (compare_(current->value, value)) {
                goes_left = false;
                current = current->right;
            } else {
                return false;
            }
        }

        Node* node = new Node(value);
        node->parent = parent;
        if (!parent) {
            root_ = node;
        } else if (goes_left) {
            parent->left = node;
        } else {
            parent->right = node;
        }

        fix_insert(node);
        ++size_;
        return true;
    }

    [[nodiscard]] bool contains(const T& value) const {
        const Node* current = root_;
        while (current) {
            if (compare_(value, current->value)) {
                current = current->left;
            } else if (compare_(current->value, value)) {
                current = current->right;
            } else {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] std::size_t size() const noexcept { return size_; }
    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

    [[nodiscard]] std::vector<T> inorder() const {
        std::vector<T> result;
        result.reserve(size_);
        collect_inorder(root_, result);
        return result;
    }
};

}

#endif // AED_ALGHORITHMS_RB_H

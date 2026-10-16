#ifndef AED_ALGHORITHMS_AVL_H
#define AED_ALGHORITHMS_AVL_H

#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace aed {

// Ordered set with O(log n) insertion and lookup. Duplicate keys are ignored.
template <typename T, typename Compare = std::less<T>>
class AVLTree {
private:
    struct Node {
        T value;
        int height = 1;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        explicit Node(const T& value) : value(value) {}
    };

    Compare compare_;
    std::unique_ptr<Node> root_;
    std::size_t size_ = 0;

    static int height_of(const Node* node) noexcept {
        return node ? node->height : 0;
    }

    static void update_height(Node* node) noexcept {
        node->height = 1 + std::max(height_of(node->left.get()),
                                    height_of(node->right.get()));
    }

    static int balance(const Node* node) noexcept {
        return height_of(node->left.get()) - height_of(node->right.get());
    }

    static void rotate_right(std::unique_ptr<Node>& subtree) noexcept {
        auto pivot = std::move(subtree->left);
        subtree->left = std::move(pivot->right);
        update_height(subtree.get());
        pivot->right = std::move(subtree);
        update_height(pivot.get());
        subtree = std::move(pivot);
    }

    static void rotate_left(std::unique_ptr<Node>& subtree) noexcept {
        auto pivot = std::move(subtree->right);
        subtree->right = std::move(pivot->left);
        update_height(subtree.get());
        pivot->left = std::move(subtree);
        update_height(pivot.get());
        subtree = std::move(pivot);
    }

    bool insert_into(std::unique_ptr<Node>& node, const T& value) {
        if (!node) {
            node = std::make_unique<Node>(value);
            return true;
        }

        bool inserted;
        if (compare_(value, node->value)) {
            inserted = insert_into(node->left, value);
        } else if (compare_(node->value, value)) {
            inserted = insert_into(node->right, value);
        } else {
            return false;
        }

        if (!inserted) {
            return false;
        }

        update_height(node.get());
        const int factor = balance(node.get());
        if (factor > 1) {
            if (balance(node->left.get()) < 0) {
                rotate_left(node->left); // LR -> LL
            }
            rotate_right(node); // LL
        } else if (factor < -1) {
            if (balance(node->right.get()) > 0) {
                rotate_right(node->right); // RL -> RR
            }
            rotate_left(node); // RR
        }
        return true;
    }

    static void collect_inorder(const Node* node, std::vector<T>& result) {
        if (!node) {
            return;
        }
        collect_inorder(node->left.get(), result);
        result.push_back(node->value);
        collect_inorder(node->right.get(), result);
    }

public:
    AVLTree() = default;
    explicit AVLTree(Compare compare) : compare_(std::move(compare)) {}

    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    AVLTree(AVLTree&& other)
        : compare_(std::move(other.compare_)), root_(std::move(other.root_)),
          size_(std::exchange(other.size_, 0)) {}

    AVLTree& operator=(AVLTree&& other) {
        if (this != &other) {
            compare_ = std::move(other.compare_);
            root_ = std::move(other.root_);
            size_ = std::exchange(other.size_, 0);
        }
        return *this;
    }

    // Returns false if an equivalent key already exists.
    bool insert(const T& value) {
        if (!insert_into(root_, value)) {
            return false;
        }
        ++size_;
        return true;
    }

    [[nodiscard]] bool contains(const T& value) const {
        const Node* current = root_.get();
        while (current) {
            if (compare_(value, current->value)) {
                current = current->left.get();
            } else if (compare_(current->value, value)) {
                current = current->right.get();
            } else {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] std::size_t size() const noexcept { return size_; }
    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
    [[nodiscard]] int height() const noexcept { return height_of(root_.get()); }

    [[nodiscard]] std::vector<T> inorder() const {
        std::vector<T> result;
        result.reserve(size_);
        collect_inorder(root_.get(), result);
        return result;
    }
};

} // namespace aed

#endif // AED_ALGHORITHMS_AVL_H

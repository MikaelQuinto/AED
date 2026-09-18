#ifndef AED_ABB_H
#define AED_ABB_H

#include <iostream>

template <typename T>
struct Node {
    T data;
    Node* left;
    Node* right;

    Node(T value) : data(value), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinarySearchTree {
private:
    Node<T>* root;

    // ---------- Private recursive helpers ----------

    Node<T>* insert(Node<T>* node, T value) {
        if (node == nullptr) {
            return new Node<T>(value);
        }
        if (value < node->data) {
            node->left = insert(node->left, value);
        } else if (value > node->data) {
            node->right = insert(node->right, value);
        }
        return node;
    }

    Node<T>* findMin(Node<T>* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node<T>* remove(Node<T>* node, T value) {
        if (node == nullptr) return nullptr;

        if (value < node->data) {
            node->left = remove(node->left, value);
        } else if (value > node->data) {
            node->right = remove(node->right, value);
        } else {
            // Caso 1: sin hijos
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            // Caso 2: un solo hijo
            else if (node->left == nullptr) {
                Node<T>* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node<T>* temp = node->left;
                delete node;
                return temp;
            }
            // Caso 3: dos hijos
            else {
                Node<T>* successor = findMin(node->right);
                node->data = successor->data;
                node->right = remove(node->right, successor->data);
            }
        }
        return node;
    }

    Node<T>* search(Node<T>* node, T value) {
        if (node == nullptr || node->data == value) {
            return node;
        }
        if (value < node->data) {
            return search(node->left, value);
        }
        return search(node->right, value);
    }

    int level(Node<T>* node, T value, int depth) {
        if (node == nullptr) {
            return -1;
        }

        if (node->data == value) {
            return depth;
        }

        if (value < node->data) {
            return level(node->left, value, depth + 1);
        }

        return level(node->right, value, depth + 1);
    }

    void inorder(Node<T>* node) {
        if (node == nullptr) return;
        inorder(node->left);
        std::cout << node->data << " ";
        inorder(node->right);
    }

    void preorder(Node<T>* node) {
        if (node == nullptr) return;
        std::cout << node->data << " ";
        preorder(node->left);
        preorder(node->right);
    }

    void postorder(Node<T>* node) {
        if (node == nullptr) return;
        postorder(node->left);
        postorder(node->right);
        std::cout << node->data << " ";
    }

    void clear(Node<T>* node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    int height(Node<T>* node) {
        if (node == nullptr) return -1;
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);
        return 1 + std::max(leftHeight, rightHeight);
    }

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        clear(root);
    }

    // ---------- Public interface ----------

    void insert(T value) {
        root = insert(root, value);
    }

    void remove(T value) {
        root = remove(root, value);
    }

    bool search(T value) {
        return search(root, value) != nullptr;
    }

    int level(T value) {
        return level(root, value, 0);
    }

    void inorder() {
        inorder(root);
        std::cout << std::endl;
    }

    void preorder() {
        preorder(root);
        std::cout << std::endl;
    }

    void postorder() {
        postorder(root);
        std::cout << std::endl;
    }

    int height() {
        return height(root);
    }

    bool isEmpty() {
        return root == nullptr;
    }
};

#endif //AED_ABB_H

#ifndef AED_HEAP_H
#define AED_HEAP_H

#include <vector>
#include <stdexcept>
#include <functional>

template <typename T, typename Comparator = std::greater<T>>
class Heap {
private:
    std::vector<T> data;
    Comparator comp;

    void siftUp(int index);
    void siftDown(int index);

public:
    Heap() = default;

    bool isEmpty() const;
    int size() const;
    void insert(const T& value);
    T extractTop();
    const T& peekTop() const;
};

// ---------- Implementation ----------

template <typename T, typename Comparator>
bool Heap<T, Comparator>::isEmpty() const {
    return data.empty();
}

template <typename T, typename Comparator>
int Heap<T, Comparator>::size() const {
    return data.size();
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::siftUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (comp(data[parent], data[index])) {
            std::swap(data[index], data[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::siftDown(int index) {
    int n = data.size();

    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int candidate = index;

        if (left < n && comp(data[candidate], data[left])) {
            candidate = left;
        }
        if (right < n && comp(data[candidate], data[right])) {
            candidate = right;
        }
        if (candidate == index) {
            break;
        }

        std::swap(data[index], data[candidate]);
        index = candidate;
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::insert(const T& value) {
    data.push_back(value);
    siftUp(data.size() - 1);
}

template <typename T, typename Comparator>
T Heap<T, Comparator>::extractTop() {
    if (data.empty()) {
        throw std::runtime_error("Heap is empty");
    }

    T top = data[0];
    data[0] = data.back();
    data.pop_back();

    if (!data.empty()) {
        siftDown(0);
    }

    return top;
}

template <typename T, typename Comparator>
const T& Heap<T, Comparator>::peekTop() const {
    if (data.empty()) {
        throw std::runtime_error("Heap is empty");
    }
    return data[0];
}

#endif //AED_HEAP_H
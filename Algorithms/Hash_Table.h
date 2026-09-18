#ifndef AED_HASH_TABLE_H
#define AED_HASH_TABLE_H

#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
#include <cstdint>

template <typename K, typename V>
struct HashNode {
    K key;
    V value;
    HashNode* next;

    HashNode(K k, V v) : key(k), value(v), next(nullptr) {}
};

template <typename K, typename V, typename Hash = std::hash<K>>
class HashTable {
private:
    std::vector<HashNode<K, V>*> table;  // cada posicion es una lista enlazada (bucket)
    int count;                           // cantidad de elementos guardados
    double maxLoad;                      // factor de carga maximo antes de hacer rehash
    Hash hasher;

    // ---------- Private helpers ----------

    // Mezcla de bits (splitmix64): evita que patrones en las claves generen colisiones
    static uint64_t mix(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    // Semilla aleatoria: protege contra inputs disenados para provocar colisiones
    static uint64_t seed() {
        static const uint64_t s =
            std::chrono::steady_clock::now().time_since_epoch().count();
        return s;
    }

    // La capacidad siempre es potencia de 2, asi usamos & en vez de %
    static int nextPowerOfTwo(int n) {
        int p = 1;
        while (p < n) p <<= 1;
        return p;
    }

    int index(const K& key, int capacity) {
        return (int)(mix((uint64_t)hasher(key) + seed()) & (uint64_t)(capacity - 1));
    }

    HashNode<K, V>* findNode(const K& key) {
        HashNode<K, V>* current = table[index(key, (int)table.size())];
        while (current != nullptr) {
            if (current->key == key) return current;
            current = current->next;
        }
        return nullptr;
    }

    // Reubica todos los nodos en una tabla de nueva capacidad (sin crear nodos nuevos)
    void rehash(int newCapacity) {
        std::vector<HashNode<K, V>*> newTable(newCapacity, nullptr);
        for (HashNode<K, V>* head : table) {
            while (head != nullptr) {
                HashNode<K, V>* nextNode = head->next;
                int i = index(head->key, newCapacity);
                head->next = newTable[i];
                newTable[i] = head;
                head = nextNode;
            }
        }
        table.swap(newTable);
    }

    // Inserta un nodo nuevo (se asume que la clave NO existe)
    HashNode<K, V>* insertNew(const K& key, const V& value) {
        if (count + 1 > maxLoad * table.size()) {
            rehash((int)table.size() * 2);
        }
        int i = index(key, (int)table.size());
        HashNode<K, V>* node = new HashNode<K, V>(key, value);
        node->next = table[i];
        table[i] = node;
        count++;
        return node;
    }

public:
    HashTable(int initialCapacity = 16, double maxLoadFactor = 0.75)
        : table(nextPowerOfTwo(initialCapacity < 1 ? 1 : initialCapacity), nullptr),
          count(0), maxLoad(maxLoadFactor) {}

    ~HashTable() {
        clear();
    }

    // Copiar la tabla duplicaria punteros: lo deshabilitamos para evitar doble delete
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    // ---------- Public interface ----------

    // Inserta la clave; si ya existe, actualiza su valor
    void insert(K key, V value) {
        HashNode<K, V>* node = findNode(key);
        if (node != nullptr) {
            node->value = value;
        } else {
            insertNew(key, value);
        }
    }

    bool remove(K key) {
        int i = index(key, (int)table.size());
        HashNode<K, V>* current = table[i];
        HashNode<K, V>* prev = nullptr;

        while (current != nullptr) {
            if (current->key == key) {
                if (prev == nullptr) {
                    table[i] = current->next;   // era la cabeza del bucket
                } else {
                    prev->next = current->next;
                }
                delete current;
                count--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    bool search(K key) {
        return findNode(key) != nullptr;
    }

    bool contains(K key) {
        return search(key);
    }

    // Devuelve puntero al valor (nullptr si no existe). Permite leer y modificar
    V* find(K key) {
        HashNode<K, V>* node = findNode(key);
        return node == nullptr ? nullptr : &node->value;
    }

    // Devuelve el valor o defaultValue si la clave no existe (no inserta)
    V get(K key, V defaultValue = V()) {
        HashNode<K, V>* node = findNode(key);
        return node == nullptr ? defaultValue : node->value;
    }

    // Igual que std::map: si la clave no existe, la crea con V()
    V& operator[](K key) {
        HashNode<K, V>* node = findNode(key);
        if (node == nullptr) node = insertNew(key, V());
        return node->value;
    }

    // Reserva espacio para n elementos (evita rehash durante inserciones masivas)
    void reserve(int n) {
        int needed = nextPowerOfTwo((int)(n / maxLoad) + 1);
        if (needed > (int)table.size()) rehash(needed);
    }

    // Recorre todos los pares (clave, valor)
    void forEach(std::function<void(const K&, V&)> action) {
        for (HashNode<K, V>* head : table) {
            while (head != nullptr) {
                action(head->key, head->value);
                head = head->next;
            }
        }
    }

    std::vector<K> keys() {
        std::vector<K> result;
        result.reserve(count);
        forEach([&](const K& k, V&) { result.push_back(k); });
        return result;
    }

    std::vector<V> values() {
        std::vector<V> result;
        result.reserve(count);
        forEach([&](const K&, V& v) { result.push_back(v); });
        return result;
    }

    void clear() {
        for (int i = 0; i < (int)table.size(); i++) {
            HashNode<K, V>* current = table[i];
            while (current != nullptr) {
                HashNode<K, V>* temp = current->next;
                delete current;
                current = temp;
            }
            table[i] = nullptr;
        }
        count = 0;
    }

    int size() {
        return count;
    }

    int capacity() {
        return (int)table.size();
    }

    double loadFactor() {
        return (double)count / table.size();
    }

    bool isEmpty() {
        return count == 0;
    }

    // Imprime cada bucket con su lista (util para depurar)
    void print() {
        for (int i = 0; i < (int)table.size(); i++) {
            std::cout << "[" << i << "]:";
            HashNode<K, V>* current = table[i];
            while (current != nullptr) {
                std::cout << " (" << current->key << ": " << current->value << ")";
                current = current->next;
            }
            std::cout << std::endl;
        }
    }
};

// =====================================================================
//  Utilidades para problemas: frecuencias y multiconjuntos
// =====================================================================

// Cuenta cuantas veces aparece cada valor del arreglo
template <typename T>
void buildFrequency(const std::vector<T>& arr, HashTable<T, int>& freq) {
    freq.reserve((int)arr.size());
    for (const T& x : arr) {
        freq[x]++;
    }
}

// Cantidad de valores distintos
template <typename T>
int countDistinct(const std::vector<T>& arr) {
    HashTable<T, int> freq;
    buildFrequency(arr, freq);
    return freq.size();
}

// Hay algun elemento repetido?
template <typename T>
bool hasDuplicates(const std::vector<T>& arr) {
    HashTable<T, bool> seen;
    seen.reserve((int)arr.size());
    for (const T& x : arr) {
        if (seen.contains(x)) return true;
        seen[x] = true;
    }
    return false;
}

// B es una permutacion de A? (mismos valores con las mismas multiplicidades)
template <typename T>
bool sameMultiset(const std::vector<T>& a, const std::vector<T>& b) {
    if (a.size() != b.size()) return false;

    HashTable<T, int> freq;
    buildFrequency(a, freq);

    for (const T& x : b) {
        int* c = freq.find(x);
        if (c == nullptr || *c == 0) return false;   // no existe o ya se agoto
        (*c)--;
    }
    return true;  // mismo tamano y nunca nos pasamos => son iguales
}

// Valor mas frecuente y su frecuencia (arreglo no vacio)
template <typename T>
std::pair<T, int> mostFrequent(const std::vector<T>& arr) {
    HashTable<T, int> freq;
    buildFrequency(arr, freq);

    std::pair<T, int> best(arr[0], 0);
    freq.forEach([&](const T& value, int& c) {
        if (c > best.second) best = std::make_pair(value, c);
    });
    return best;
}

// Indice del primer elemento que aparece exactamente una vez (-1 si no hay)
template <typename T>
int firstUniqueIndex(const std::vector<T>& arr) {
    HashTable<T, int> freq;
    buildFrequency(arr, freq);
    for (int i = 0; i < (int)arr.size(); i++) {
        if (freq[arr[i]] == 1) return i;
    }
    return -1;
}

#endif //AED_HASH_TABLE_H

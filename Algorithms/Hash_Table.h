#ifndef AED_HASH_TABLE_H
#define AED_HASH_TABLE_H

#include <vector>
#include <functional>
#include <utility>

template <typename K, typename V>
class HashTable {
private:
    struct Entry {
        K key;
        V value;
        bool used;

        Entry() : used(false) {}
    };

    std::vector<Entry> table;
    int count;
    double maxLoad;
    std::hash<K> hasher;

    int index(const K& key) const {
        return hasher(key) % table.size();
    }

    void rehash() {
        std::vector<Entry> oldTable = table;

        table.clear();
        table.resize(oldTable.size() * 2);
        count = 0;

        for (const Entry& entry : oldTable) {
            if (entry.used) {
                insert(entry.key, entry.value);
            }
        }
    }

public:
    HashTable(int capacity = 16, double load = 0.7)
        : table(capacity), count(0), maxLoad(load) {}

    void insert(const K& key, const V& value) {
        if ((count + 1.0) / table.size() > maxLoad) {
            rehash();
        }

        int i = index(key);

        while (table[i].used) {
            if (table[i].key == key) {
                table[i].value = value;
                return;
            }

            i++;

            if (i == table.size())
                i = 0;
        }

        table[i].key = key;
        table[i].value = value;
        table[i].used = true;
        count++;
    }

    V& operator[](const K& key) {
        if ((count + 1.0) / table.size() > maxLoad) {
            rehash();
        }

        int i = index(key);

        while (table[i].used) {
            if (table[i].key == key) {
                return table[i].value;
            }

            i++;

            if (i == table.size())
                i = 0;
        }

        table[i].key = key;
        table[i].value = V();
        table[i].used = true;
        count++;

        return table[i].value;
    }

    V get(const K& key, V defaultValue = V()) const {
        int i = index(key);

        while (table[i].used) {
            if (table[i].key == key) {
                return table[i].value;
            }

            i++;

            if (i == table.size())
                i = 0;
        }

        return defaultValue;
    }

    bool contains(const K& key) const {
        int i = index(key);

        while (table[i].used) {
            if (table[i].key == key)
                return true;

            i++;

            if (i == table.size())
                i = 0;
        }

        return false;
    }

    int size() const {
        return count;
    }

    bool isEmpty() const {
        return count == 0;
    }
};

#endif
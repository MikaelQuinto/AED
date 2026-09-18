#include <iostream>
#include "Algorithms/Hash_Table.h"

using namespace std;

int main() {
    int n, q;
    cin >> n >> q;

    HashTable<int, int> hash;

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        hash[x]++;
    }

    for (int i = 0; i < q; i++) {
        int x;
        cin >> x;
        cout << hash.get(x, 0) << '\n';
    }
}
#include <iostream>
#include <vector>

# include "Algorithms/ABB.h"
using namespace std;

int main() {
    cout << "Hello World!" << endl;
    int n;
    cin >> n;

    BinarySearchTree<int> root;

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        root.insert(x);
    }

    int q;
    cin >> q;
    vector<int> queries;

    for (int i = 0; i < q; i++) {
        int y;
        cin >> y;
        queries.push_back(y);
    }

    cout << root.height() << endl;

    for (int i = 0; i < q; i++) {

    }
}
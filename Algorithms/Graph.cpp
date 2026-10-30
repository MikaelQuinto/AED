#include <bits/stdc++.h>
using namespace std;

int main() {

    int n, m;
    cin >> n >> m;
    vector<int> p(n), sz(n, 1), ed(n, 0);
    iota(p.begin(), p.end(), 0);

    auto find = [&](int x) {
        int r = x;
        while (p[r] != r) r = p[r];
        while (p[x] != x) {
            int y = p[x];
            p[x] = r;
            x = y;
        }
        return r;
    };

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        --u;
        --v;
        int a = find(u), b = find(v);
        if (a == b) {
            ++ed[a];
        } else {
            if (sz[a] < sz[b]) swap(a, b);
            p[b] = a;
            sz[a] += sz[b];
            ed[a] += ed[b] + 1;
        }
    }

    for (int i = 0; i < n; ++i) {
        if (p[i] == i && ed[i] > sz[i]) {
            cout << "NO\n";
            return 0;
        }
    }
    cout << "SI\n";
    return 0;
}
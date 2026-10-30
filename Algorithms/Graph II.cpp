#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, d, k;
    cin >> n >> d >> k;

    vector<int> go(n + 1);
    iota(go.begin(), go.end(), 0);
    for (int i = 0; i < k; ++i) {
        int a, b;
        cin >> a >> b;
        go[a] = b;
    }

    vector<int> dist(n + 1, -1);
    queue<int> q;
    dist[1] = 0;
    q.push(1);

    while (!q.empty()) {
        int x = q.front();
        q.pop();
        for (int step = 1; step <= d && x + step <= n; ++step) {
            int y = go[x + step];
            if (dist[y] == -1) {
                dist[y] = dist[x] + 1;
                q.push(y);
            }
        }
    }

    cout << dist[n] << '\n';
    return 0;
}

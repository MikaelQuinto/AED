#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
    int n, t;
    cin >> n >> t;

    vector<vector<int>> graph(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    vector<int> parent(n + 1, 0);
    queue<int> q;
    q.push(1);
    parent[1] = -1;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int next : graph[node]) {
            if (parent[next] == 0) {
                parent[next] = node;
                q.push(next);
            }
        }
    }

    vector<int> path;
    for (int node = t; node != -1; node = parent[node]) {
        path.push_back(node);
    }

    cout << path.size() << '\n';
    for (int i = path.size() - 1; i >= 0; i--) {
        cout << path[i] << (i == 0 ? '\n' : ' ');
    }

    return 0;
}



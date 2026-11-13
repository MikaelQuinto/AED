#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

vector<long long> dijkstra(vector<vector<pair<int, int>>>& graph) {
    const long long INF = LLONG_MAX / 4;
    vector<long long> distance(graph.size(), INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> q;

    distance[1] = 0;
    q.push({0, 1});

    while (!q.empty()) {
        long long currentDistance = q.top().first;
        int node = q.top().second;
        q.pop();

        if (currentDistance != distance[node]) continue;

        for (auto edge : graph[node]) {
            int next = edge.first;
            int weight = edge.second;
            if (currentDistance + weight < distance[next]) {
                distance[next] = currentDistance + weight;
                q.push({distance[next], next});
            }
        }
    }

    return distance;
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> graph(n + 1);
    vector<vector<pair<int, int>>> reverseGraph(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        graph[u].push_back({v, w});
        reverseGraph[v].push_back({u, w});
    }

    vector<long long> go = dijkstra(graph);
    vector<long long> back = dijkstra(reverseGraph);

    int count = 0;
    long long total = 0;
    const long long INF = LLONG_MAX / 4;

    for (int city = 1; city <= n; city++) {
        if (go[city] != INF && back[city] != INF) {
            count++;
            total += go[city] + back[city];
        }
    }

    cout << count << ' ' << total << '\n';
    return 0;
}

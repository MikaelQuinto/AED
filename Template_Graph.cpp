#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = LLONG_MAX / 4;

int n;
vector<pair<int, ll>> adj[100005];  // {vecino, peso}

// BFS: distancia en número de aristas
vector<int> bfs(int s) {
    vector<int> dist(n + 1, -1);
    queue<int> q;
    dist[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        /*____ procesar u ____*/
        for (auto [v, w] : adj[u]) {
            if (dist[v] != -1) continue;
            /*____ condición extra ____*/
            dist[v] = dist[u] + 1;
            q.push(v);
        }
    }
    return dist;
}

// Dijkstra: distancia mínima con pesos
vector<ll> dijkstra(int s) {
    vector<ll> dist(n + 1, INF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
    dist[s] = 0;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;
        /*____ procesar u ____*/
        for (auto [v, w] : adj[u]) {
            /*____ ajustar w o condición ____*/
            if (d + w < dist[v]) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

int main() {
    int m;
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
        int u, v; ll w = 1;
        cin >> u >> v /*>> w*/;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});  // quitar si es dirigido
    }
    /*____ tu solución: bfs(1) o dijkstra(1) ____*/
}
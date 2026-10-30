#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    vector<string> g(n);
    for (int i = 0; i < n; i++) cin >> g[i];

    const int INF = INT_MAX;
    vector<vector<int>> dist(n, vector<int>(m, INF));
    deque<pair<int, int>> dq;

    dist[0][0] = 0;
    dq.push_back({0, 0});

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!dq.empty()) {
        auto [r, c] = dq.front();
        dq.pop_front();

        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k];
            int nc = c + dc[k];

            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;

            int w = (g[nr][nc] == '#') ? 1 : 0;
            if (dist[r][c] + w < dist[nr][nc]) {
                dist[nr][nc] = dist[r][c] + w;
                if (w == 0) dq.push_front({nr, nc});
                else dq.push_back({nr, nc});
            }
        }
    }

    cout << dist[n - 1][m - 1] << "\n";
    return 0;

}
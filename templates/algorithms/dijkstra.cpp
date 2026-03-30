/*
 * 最短路：Dijkstra（堆）、Bellman-Ford / SPFA、Floyd。
 */
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = (1LL << 60);

// 邻接表：to, w
vector<vector<pair<int, ll>>> dij_g;

vector<ll> dijkstra(int s, int n) {
    vector<ll> d(n, INF);
    d[s] = 0;
    using P = pair<ll, int>;
    priority_queue<P, vector<P>, greater<P>> pq;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [du, u] = pq.top();
        pq.pop();
        if (du != d[u]) continue;
        for (auto [v, w] : dij_g[u]) {
            if (d[v] > du + w) {
                d[v] = du + w;
                pq.push({d[v], v});
            }
        }
    }
    return d;
}

// Bellman-Ford：边 (u,v,w)，可判负环
bool bellman_ford(int n, const vector<tuple<int, int, ll>> &edges, int s, vector<ll> &d) {
    d.assign(n, INF);
    d[s] = 0;
    for (int i = 0; i < n - 1; ++i)
        for (auto [u, v, w] : edges)
            if (d[u] < INF && d[v] > d[u] + w) d[v] = d[u] + w;
    for (auto [u, v, w] : edges)
        if (d[u] < INF && d[v] > d[u] + w) return false; // 负环
    return true;
}

bool spfa(int n, const vector<vector<pair<int, ll>>> &g, int s, vector<ll> &d) {
    d.assign(n, INF);
    vector<int> cnt(n), inq(n);
    queue<int> q;
    d[s] = 0;
    q.push(s);
    inq[s] = 1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        inq[u] = 0;
        for (auto [v, w] : g[u]) {
            if (d[v] > d[u] + w) {
                d[v] = d[u] + w;
                if (!inq[v]) {
                    q.push(v);
                    inq[v] = 1;
                    if (++cnt[v] >= n) return false;
                }
            }
        }
    }
    return true;
}

void floyd(int n, vector<vector<ll>> &d) {
    for (int k = 0; k < n; ++k)
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (d[i][k] < INF && d[k][j] < INF)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n = 3;
    dij_g.assign(n, {});
    dij_g[0].push_back({1, 1});
    dij_g[1].push_back({2, 2});
    auto d1 = dijkstra(0, n);
    vector<tuple<int, int, ll>> edges = {{0, 1, 1}, {1, 2, 2}};
    vector<ll> d2;
    bellman_ford(n, edges, 0, d2);
    vector<vector<ll>> d3(n, vector<ll>(n, INF));
    for (int i = 0; i < n; ++i) d3[i][i] = 0;
    d3[0][1] = d3[1][0] = 1;
    floyd(n, d3);
    (void)d1;
    return 0;
}

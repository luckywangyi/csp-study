/*
 * 图：邻接表/矩阵、拓扑排序、Kruskal、Prim。
 */
#include <bits/stdc++.h>
using namespace std;

struct DSU {
    vector<int> p, r;
    DSU(int n) : p(n), r(n) { iota(p.begin(), p.end(), 0); }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (r[a] < r[b]) swap(a, b);
        p[b] = a;
        if (r[a] == r[b]) r[a]++;
        return true;
    }
};

// 邻接表
vector<vector<pair<int, int>>> build_adj_list(int n, const vector<tuple<int, int, int>> &edges) {
    vector<vector<pair<int, int>>> g(n);
    for (auto [u, v, w] : edges) {
        g[u].push_back({v, w});
        // g[v].push_back({u, w}); // 无向图
    }
    return g;
}

// 邻接矩阵（稠密图）
vector<vector<int>> build_adj_mat(int n, const vector<tuple<int, int, int>> &edges, int inf = 1e9) {
    vector<vector<int>> a(n, vector<int>(n, inf));
    for (int i = 0; i < n; ++i) a[i][i] = 0;
    for (auto [u, v, w] : edges) a[u][v] = min(a[u][v], w);
    return a;
}

// 拓扑序，有环返回空
vector<int> topo_sort(int n, const vector<vector<int>> &g) {
    vector<int> indeg(n);
    for (int u = 0; u < n; ++u)
        for (int v : g[u]) indeg[v]++;
    queue<int> q;
    for (int i = 0; i < n; ++i)
        if (!indeg[i]) q.push(i);
    vector<int> ord;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        ord.push_back(u);
        for (int v : g[u])
            if (--indeg[v] == 0) q.push(v);
    }
    return ord.size() == n ? ord : vector<int>();
}

long long kruskal(int n, vector<tuple<int, int, int>> edges) {
    sort(edges.begin(), edges.end(), [](auto &a, auto &b) { return get<2>(a) < get<2>(b); });
    DSU dsu(n);
    long long sum = 0;
    int cnt = 0;
    for (auto [u, v, w] : edges)
        if (dsu.unite(u, v)) sum += w, cnt++;
    return cnt == n - 1 ? sum : -1;
}

long long prim(int n, const vector<vector<pair<int, int>>> &g) {
    vector<char> vis(n);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, 0});
    long long sum = 0;
    int cnt = 0;
    while (!pq.empty() && cnt < n) {
        auto [w, u] = pq.top();
        pq.pop();
        if (vis[u]) continue;
        vis[u] = 1;
        sum += w;
        cnt++;
        for (auto [v, dw] : g[u])
            if (!vis[v]) pq.push({dw, v});
    }
    return cnt == n ? sum : -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n = 3;
    vector<tuple<int, int, int>> e = {{0, 1, 1}, {1, 2, 2}, {0, 2, 3}};
    (void)build_adj_list(n, e);
    vector<vector<int>> gg(n);
    gg[0].push_back(1);
    gg[1].push_back(2);
    (void)topo_sort(n, gg);
    vector<tuple<int, int, int>> und = {{0, 1, 1}, {1, 2, 2}};
    (void)kruskal(n, und);
    vector<vector<pair<int, int>>> adj(n);
    adj[0].push_back({1, 1});
    adj[1].push_back({0, 1});
    adj[1].push_back({2, 2});
    adj[2].push_back({1, 2});
    (void)prim(n, adj);
    return 0;
}

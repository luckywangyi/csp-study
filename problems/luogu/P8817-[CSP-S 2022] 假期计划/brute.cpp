// P8817 假期计划 — 参考暴力：全源 BFS，to[u][v]=最短路边数<=k+1；对每个 v 维护 to(1,i)&&to(i,v) 的权值前 4 大 i，枚举中间两景点组合
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Top4 {
    int v[4];
    Top4() { memset(v, 0, sizeof v); }
    void update(int id, const vector<ll>& w) {
        auto better = [&](int a, int b) { return w[a] >= w[b]; };
        if (!v[0] || better(id, v[0])) {
            v[3] = v[2];
            v[2] = v[1];
            v[1] = v[0];
            v[0] = id;
        } else if (!v[1] || better(id, v[1])) {
            v[3] = v[2];
            v[2] = v[1];
            v[1] = id;
        } else if (!v[2] || better(id, v[2])) {
            v[3] = v[2];
            v[2] = id;
        } else if (!v[3] || better(id, v[3])) {
            v[3] = id;
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, K;
    cin >> n >> m >> K;
    int lim = K + 1;
    vector<ll> w(n + 1, 0);
    for (int i = 2; i <= n; i++) cin >> w[i];
    w[1] = (ll)-4e18;
    vector<vector<int>> g(n + 1);
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;
        g[x].push_back(y);
        g[y].push_back(x);
    }

    vector<vector<char>> to(n + 1, vector<char>(n + 1, 0));
    vector<int> dist(n + 1);
    for (int s = 1; s <= n; s++) {
        fill(dist.begin(), dist.end(), -1);
        queue<int> q;
        dist[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            if (dist[u] > lim) continue;
            to[s][u] = 1;
            if (dist[u] == lim) continue;
            for (int v : g[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
    }

    vector<Top4> f(n + 1);
    for (int i = 2; i <= n; i++) {
        for (int j = 2; j <= n; j++) {
            if (i == j) continue;
            if (to[1][i] && to[i][j]) f[j].update(i, w);
        }
    }

    ll ans = 0;
    for (int u = 2; u <= n; u++) {
        for (int v = 2; v <= n; v++) {
            if (!to[u][v]) continue;
            Top4 &be = f[u];
            Top4 &ce = f[v];
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    int a = be.v[i];
                    int d = ce.v[j];
                    if (!a || !d) continue;
                    int pos[4] = {u, v, a, d};
                    sort(pos, pos + 4);
                    if (unique(pos, pos + 4) - pos == 4) {
                        ans = max(ans, w[u] + w[v] + w[a] + w[d]);
                    }
                }
            }
        }
    }
    cout << ans << "\n";
    return 0;
}

// P5663 加工零件 — 标程（分图奇偶最短路）：
// - L==1：Yes 当且仅当 a 与 1 相邻。
// - L>1：二分图时等价于「是否存在从 1 到 a、长度为 L-1 的行走」；
//   非二分图时用「是否存在从 a 到 1、长度为 L 的行走」（分层最短路 + 反复走一边 +2）。
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    if (!(cin >> n >> m >> q)) return 0;
    vector<vector<int>> g(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    const int INF = 1e9;
    vector<int> col(n + 1, -1);
    bool bip = true;
    for (int s = 1; s <= n; ++s) {
        if (col[s] != -1) continue;
        col[s] = 0;
        queue<int> qq;
        qq.push(s);
        while (!qq.empty()) {
            int u = qq.front();
            qq.pop();
            for (int v : g[u]) {
                if (col[v] == -1) {
                    col[v] = col[u] ^ 1;
                    qq.push(v);
                } else if (col[v] == col[u]) {
                    bip = false;
                }
            }
        }
    }

    vector<array<int, 2>> dis(n + 1, {INF, INF});
    dis[1][0] = 0;
    queue<pair<int, int>> qu;
    vector<array<char, 2>> vis(n + 1, {0, 0});
    qu.emplace(1, 0);
    vis[1][0] = 1;
    while (!qu.empty()) {
        auto [u, p] = qu.front();
        qu.pop();
        int np = p ^ 1;
        for (int v : g[u]) {
            if (vis[v][np]) continue;
            vis[v][np] = 1;
            dis[v][np] = dis[u][p] + 1;
            qu.emplace(v, np);
        }
    }

    vector<char> reach(n + 1, 0);
    {
        queue<int> qq;
        qq.push(1);
        reach[1] = 1;
        while (!qq.empty()) {
            int u = qq.front();
            qq.pop();
            for (int v : g[u])
                if (!reach[v]) {
                    reach[v] = 1;
                    qq.push(v);
                }
        }
    }
    int comp_sz = 0;
    for (int i = 1; i <= n; ++i)
        if (reach[i]) ++comp_sz;
    const bool can_pad = comp_sz >= 2;

    auto feasible_len = [&](int to, long long len) -> bool {
        if (len < 0) return false;
        if (len == 0) return to == 1;
        if (!reach[to]) return false;
        int p = int(len & 1LL);
        int d = dis[to][p];
        if (d >= INF / 2) return false;
        if (len < d) return false;
        if ((len - d) % 2 != 0) return false;
        if (len > d && !can_pad) return false;
        return true;
    };

    for (int i = 0; i < q; ++i) {
        int a;
        long long L;
        cin >> a >> L;
        bool ok = false;
        if (L == 1) {
            for (int v : g[a])
                if (v == 1) ok = true;
        } else if (bip) {
            ok = feasible_len(a, L - 1);
        } else {
            int p = int(L & 1LL);
            int d = dis[a][p];
            if (d < INF / 2 && d <= L && (L - d) % 2 == 0 && (L == d || can_pad)) ok = true;
        }
        cout << (ok ? "Yes" : "No") << '\n';
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<int64> a(n + 1), b(n + 1), c(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i] >> b[i] >> c[i];
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<int> perm;
    vector<char> in_perm(n + 1, 0);
    int64 best = numeric_limits<int64>::max();

    function<void()> dfs = [&]() {
        if ((int)perm.size() == n) {
            vector<int> pday(n + 1, 0);
            vector<int64> h(n + 1, 0);
            const int LIM = 5000000;
            for (int day = 1; day <= LIM; day++) {
                if (day <= n) {
                    int v = perm[day - 1];
                    pday[v] = day;
                }
                for (int v = 1; v <= n; v++) {
                    if (pday[v] == 0 || pday[v] > day) continue;
                    int64 add = max(b[v] + (int64)day * c[v], (int64)1);
                    h[v] += add;
                }
                bool ok = true;
                for (int v = 1; v <= n; v++) {
                    if (h[v] < a[v]) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    best = min(best, (int64)day);
                    break;
                }
            }
            return;
        }
        for (int v = 1; v <= n; v++) {
            if (in_perm[v]) continue;
            if (perm.empty()) {
                if (v != 1) continue;
            } else {
                bool touch = false;
                for (int u : adj[v])
                    if (in_perm[u]) touch = true;
                if (!touch) continue;
            }
            in_perm[v] = 1;
            perm.push_back(v);
            dfs();
            perm.pop_back();
            in_perm[v] = 0;
        }
    };

    dfs();
    cout << best << '\n';
    return 0;
}

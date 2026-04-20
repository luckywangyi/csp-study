// P5664 Emiya 家今天的饭 — 标程：差分 DP + 容斥（见洛谷题解），模 998244353
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
static const ll MOD = 998244353;

int addmod(int a, int b) {
    int s = a + b;
    if (s >= MOD) s -= MOD;
    return s;
}
int submod(int a, int b) {
    int s = a - b;
    if (s < 0) s += MOD;
    return s;
}
int mulmod(ll a, ll b) { return int((a * b) % MOD); }

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;
    vector<vector<ll>> a(n + 1, vector<ll>(m + 1));
    vector<ll> srow(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            cin >> a[i][j];
            srow[i] = (srow[i] + a[i][j]) % MOD;
        }
    }

    int ans = 1;
    for (int i = 1; i <= n; ++i) ans = mulmod(ans, addmod(int(srow[i]), 1));
    ans = submod(ans, 1);

    static int f[105][305];
    const int OFF = 105;

    for (int ing = 1; ing <= m; ++ing) {
        memset(f, 0, sizeof(f));
        f[0][OFF] = 1;
        for (int i = 1; i <= n; ++i) {
            for (int k = -102; k <= 102; ++k) {
                int idx = k + OFF;
                int v0 = f[i - 1][idx];
                int v1 = mulmod(f[i - 1][idx - 1], int(a[i][ing] % MOD));
                int rest = submod(int(srow[i]), int(a[i][ing] % MOD));
                int v2 = mulmod(f[i - 1][idx + 1], rest);
                f[i][idx] = v0;
                f[i][idx] = addmod(f[i][idx], v1);
                f[i][idx] = addmod(f[i][idx], v2);
            }
        }
        for (int k = 106; k <= 206; ++k) ans = submod(ans, f[n][k]);
    }

    cout << ans << '\n';
    return 0;
}

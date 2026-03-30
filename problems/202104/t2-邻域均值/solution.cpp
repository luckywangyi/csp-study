#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, r, t;
    [[maybe_unused]] int L;
    if (!(cin >> n >> L >> r >> t)) return 0;
    vector<vector<int>> a(n + 1, vector<int>(n + 1));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j) cin >> a[i][j];

    vector<vector<long long>> pref(n + 1, vector<long long>(n + 1));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            pref[i][j] = a[i][j] + pref[i - 1][j] + pref[i][j - 1] - pref[i - 1][j - 1];

    auto rectSum = [&](int x1, int y1, int x2, int y2) -> long long {
        return pref[x2][y2] - pref[x1 - 1][y2] - pref[x2][y1 - 1] + pref[x1 - 1][y1 - 1];
    };

    int ans = 0;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            int x1 = max(1, i - r);
            int y1 = max(1, j - r);
            int x2 = min(n, i + r);
            int y2 = min(n, j + r);
            long long s = rectSum(x1, y1, x2, y2);
            int k = (x2 - x1 + 1) * (y2 - y1 + 1);
            if (s <= 1LL * t * k) ++ans;
        }
    }
    cout << ans << '\n';
    return 0;
}

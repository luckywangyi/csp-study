// P8816 上升点列 — 暴力：排序后 O(n^2 k) DP，f[i][j] 表示以第 i 个点结尾、共使用 j 个插入点的最长序列长度
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, k;
    cin >> n >> k;
    vector<pair<int, int>> pt(n);
    for (int i = 0; i < n; i++) cin >> pt[i].first >> pt[i].second;
    sort(pt.begin(), pt.end());
    vector<vector<int>> dp(n, vector<int>(k + 1, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= k; j++) dp[i][j] = j + 1;
    for (int i = 0; i < n; i++) {
        for (int t = i - 1; t >= 0; t--) {
            if (pt[t].second > pt[i].second) continue;
            int d = (pt[i].first - pt[t].first) + (pt[i].second - pt[t].second) - 1;
            if (d < 0) continue;
            for (int p = d; p <= k; p++) dp[i][p] = max(dp[i][p], dp[t][p - d] + d + 1);
        }
    }
    int ans = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j <= k; j++) ans = max(ans, dp[i][j]);
    cout << ans << "\n";
    return 0;
}

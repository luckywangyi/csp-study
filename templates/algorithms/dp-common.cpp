/*
 * 常用 DP：01/完全背包、LCS、LIS、区间 DP。
 */
#include <bits/stdc++.h>
using namespace std;

// 01 背包：容量 W，体积 w[i] 价值 v[i]
int knapsack_01(int W, const vector<int> &w, const vector<int> &v) {
    vector<int> dp(W + 1);
    int n = (int)w.size();
    for (int i = 0; i < n; ++i)
        for (int c = W; c >= w[i]; --c)
            dp[c] = max(dp[c], dp[c - w[i]] + v[i]);
    return dp[W];
}

// 完全背包：内层正序
int knapsack_unbounded(int W, const vector<int> &w, const vector<int> &v) {
    vector<int> dp(W + 1);
    int n = (int)w.size();
    for (int i = 0; i < n; ++i)
        for (int c = w[i]; c <= W; ++c)
            dp[c] = max(dp[c], dp[c - w[i]] + v[i]);
    return dp[W];
}

int lcs(const string &a, const string &b) {
    int n = (int)a.size(), m = (int)b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
            if (a[i - 1] == b[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
    return dp[n][m];
}

// LIS 长度 O(n log n)
int lis_len(const vector<int> &a) {
    vector<int> tail;
    for (int x : a) {
        auto it = lower_bound(tail.begin(), tail.end(), x);
        if (it == tail.end()) tail.push_back(x);
        else *it = x;
    }
    return (int)tail.size();
}

// 区间 DP：示例——石子合并类，dp[l][r] 表示合并区间 [l,r] 的最小代价
int interval_dp_example(const vector<int> &sum) {
    int n = (int)sum.size() - 1;
    vector<vector<int>> dp(n, vector<int>(n, 1e9));
    for (int i = 0; i < n; ++i) dp[i][i] = 0;
    for (int len = 2; len <= n; ++len)
        for (int l = 0; l + len - 1 < n; ++l) {
            int r = l + len - 1;
            for (int k = l; k < r; ++k)
                dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r] + sum[r + 1] - sum[l]);
        }
    return n ? dp[0][n - 1] : 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<int> w = {1, 2}, v = {3, 4};
    (void)knapsack_01(3, w, v);
    (void)knapsack_unbounded(5, w, v);
    (void)lcs("abc", "axc");
    (void)lis_len({3, 1, 2, 1});
    vector<int> ps = {0, 1, 2, 3};
    for (int i = 1; i < (int)ps.size(); ++i) ps[i] += ps[i - 1];
    (void)interval_dp_example(ps);
    return 0;
}

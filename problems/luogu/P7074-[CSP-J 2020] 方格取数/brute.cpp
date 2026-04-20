// P7074 方格取数 — 暴力：DFS 枚举所有不重复路径（仅适合小网格）
#include <bits/stdc++.h>
using namespace std;

int n, m;
vector<vector<int>> a;
vector<vector<char>> vis;
long long best;

void dfs(int r, int c, long long sum) {
    if (r == n - 1 && c == m - 1) {
        best = max(best, sum);
        return;
    }
    const int dr[3] = {-1, 1, 0};
    const int dc[3] = {0, 0, 1};
    for (int k = 0; k < 3; ++k) {
        int nr = r + dr[k];
        int nc = c + dc[k];
        if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
        if (vis[nr][nc]) continue;
        vis[nr][nc] = 1;
        dfs(nr, nc, sum + a[nr][nc]);
        vis[nr][nc] = 0;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    a.assign(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) cin >> a[i][j];

    vis.assign(n, vector<char>(m, 0));
    best = LLONG_MIN;
    vis[0][0] = 1;
    dfs(0, 0, a[0][0]);
    cout << best << '\n';
    return 0;
}

// P1162 填涂颜色 — 暴力标程：从边界 0 做多源 BFS，剩余 0 视为圈内填 2
// 读入格式见 problem.md；复杂度 O(n^2)
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<vector<char>> a(n, vector<char>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            int x;
            cin >> x;
            a[i][j] = char('0' + x);
        }

    vector<vector<char>> vis(n, vector<char>(n, 0));
    queue<pair<int, int>> q;
    auto push = [&](int x, int y) {
        if (x < 0 || x >= n || y < 0 || y >= n) return;
        if (a[x][y] != '0' || vis[x][y]) return;
        vis[x][y] = 1;
        q.emplace(x, y);
    };

    for (int i = 0; i < n; ++i) {
        push(0, i);
        push(n - 1, i);
        push(i, 0);
        push(i, n - 1);
    }

    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (int k = 0; k < 4; ++k) {
            int nx = x + dx[k], ny = y + dy[k];
            push(nx, ny);
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (a[i][j] == '0' && !vis[i][j]) cout << '2';
            else cout << a[i][j];
            if (j + 1 < n) cout << ' ';
        }
        cout << '\n';
    }
    return 0;
}

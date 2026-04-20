// P1605 迷宫 — 暴力：DFS 统计简单路径条数（每格最多一次）
#include <bits/stdc++.h>
using namespace std;

int N, M, SX, SY, FX, FY;
vector<vector<int>> blocked;
int ans;

void dfs(int x, int y, vector<vector<char>>& vis) {
    if (x == FX && y == FY) {
        ++ans;
        return;
    }
    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};
    for (int k = 0; k < 4; ++k) {
        int nx = x + dx[k], ny = y + dy[k];
        if (nx < 1 || nx > N || ny < 1 || ny > M) continue;
        if (blocked[nx][ny]) continue;
        if (vis[nx][ny]) continue;
        vis[nx][ny] = 1;
        dfs(nx, ny, vis);
        vis[nx][ny] = 0;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> N >> M >> T)) return 0;
    cin >> SX >> SY >> FX >> FY;
    blocked.assign(N + 1, vector<int>(M + 1, 0));
    for (int i = 0; i < T; ++i) {
        int x, y;
        cin >> x >> y;
        blocked[x][y] = 1;
    }

    vector<vector<char>> vis(N + 1, vector<char>(M + 1, 0));
    vis[SX][SY] = 1;
    ans = 0;
    dfs(SX, SY, vis);
    cout << ans << '\n';
    return 0;
}

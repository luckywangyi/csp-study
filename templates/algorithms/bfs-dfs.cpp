/*
 * BFS/DFS：队列 BFS、层序、递归/栈式 DFS、网格、连通块计数。
 */
#include <bits/stdc++.h>
using namespace std;

int n, m;
vector<vector<int>> g;
vector<int> vis;

void bfs(int s) {
    queue<int> q;
    vis.assign(n, 0);
    vis[s] = 1;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u])
            if (!vis[v]) vis[v] = 1, q.push(v);
    }
}

void bfs_levels(int s) {
    queue<int> q;
    vis.assign(n, 0);
    vis[s] = 1;
    q.push(s);
    while (!q.empty()) {
        int sz = (int)q.size();
        while (sz--) {
            int u = q.front();
            q.pop();
            for (int v : g[u])
                if (!vis[v]) vis[v] = 1, q.push(v);
        }
        // 一层结束
    }
}

void dfs_rec(int u) {
    vis[u] = 1;
    for (int v : g[u])
        if (!vis[v]) dfs_rec(v);
}

void dfs_stack(int s) {
    stack<int> st;
    st.push(s);
    while (!st.empty()) {
        int u = st.top();
        st.pop();
        if (vis[u]) continue;
        vis[u] = 1;
        for (int v : g[u]) st.push(v);
    }
}

// 网格四连通，1 可走
int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
void grid_bfs(const vector<string> &grid, int sx, int sy) {
    n = (int)grid.size();
    m = (int)grid[0].size();
    vector<vector<int>> seen(n, vector<int>(m));
    queue<pair<int, int>> q;
    q.push({sx, sy});
    seen[sx][sy] = 1;
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (auto &d : dirs) {
            int nx = x + d[0], ny = y + d[1];
            if (nx < 0 || nx >= n || ny < 0 || ny >= m) continue;
            if (seen[nx][ny] || grid[nx][ny] != '1') continue;
            seen[nx][ny] = 1;
            q.push({nx, ny});
        }
    }
}

int count_components() {
    vis.assign(n, 0);
    int c = 0;
    for (int i = 0; i < n; ++i)
        if (!vis[i]) dfs_rec(i), c++;
    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    n = 3;
    g.assign(n, {});
    g[0].push_back(1);
    g[1].push_back(2);
    vis.assign(n, 0);
    bfs(0);
    return 0;
}

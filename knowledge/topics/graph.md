# 图论专题（CSP）

## 图的表示

### 邻接矩阵

- **适用**：稠密图、`n` 较小；需要 O(1) 判断两点是否有边。  
- **空间**：O(n²)。  
- **C++ 示例**：`vector<vector<int>> g(n, vector<int>(n, INF));`，`g[u][v] = w`。

### 邻接表

- **适用**：稀疏图，边数 `m` 与 `n` 同阶或略大。  
- **空间**：O(n + m)。  
- **C++ 示例**：

```cpp
struct Edge { int to, w; };
vector<vector<Edge>> g(n);
// g[u].push_back({v, w});
```

竞赛中 **邻接表 + `vector`** 最常见；链式前向星在极大数据时可选。

---

## 图的遍历

### BFS（广度优先）

- **用途**：无权最短路（边权为 1）、分层、拓扑排序的辅助。  
- **实现**：队列；`dist[v]` 记录层数或距离。

```cpp
queue<int> q;
vector<int> dist(n, -1);
dist[s] = 0;
q.push(s);
while (!q.empty()) {
    int u = q.front(); q.pop();
    for (int v : adj[u]) {
        if (dist[v] >= 0) continue;
        dist[v] = dist[u] + 1;
        q.push(v);
    }
}
```

### DFS（深度优先）

- **用途**：连通性、回溯、判环（配合颜色标记）、记忆化在 DAG 上。

---

## 最短路径

### Dijkstra（非负权单源）

- **思想**：每次取出当前距离最小的未确定点，松弛邻边。  
- **实现**：`priority_queue` 小顶堆（存 `pair<dist, node>`）。  
- **复杂度**：O((n + m) log n)。

### Bellman-Ford / SPFA

- **适用**：有负权边（注意负环）；SPFA 为队列优化，最坏仍可能较慢，需结合数据范围。  
- CSP 中若出现负权，先确认题意与数据范围再选算法。

### Floyd（全源最短路）

- **思想**：三重循环，`dp[k][i][j]` 或滚动为 `dist[i][j]`，允许经过前 `k` 个点。  
- **复杂度**：O(n³)。  
- **适用**：`n` 较小（通常几百以内，视时限而定）。

```cpp
for (int k = 0; k < n; ++k)
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
```

---

## 最小生成树（MST）

### Kruskal

- 边排序，并查集合并，不形成环则选边。  
- **复杂度**：O(m log m)。

### Prim

- 从一点扩展，每次连最小边；堆优化类似 Dijkstra。  
- **适用**：稠密图有时更自然；实现略繁于 Kruskal。

---

## 拓扑排序

- **对象**：有向无环图（DAG）。  
- **方法**：BFS（入度为 0 入队）或 DFS 后序。  
- **用途**：任务依赖、DP 序、判环（若无法排完所有点则有环）。

---

## 强连通分量（Tarjan / Kosaraju）

- **Tarjan**：DFS + `dfn`/`low` 栈，一次 DFS 缩点。  
- **应用**：有向图缩点成 DAG，再在缩点图上 DP 或拓扑。  
- CSP 高阶题可能出现；需掌握模板与缩点后建图。

---

## CSP 中的图论真题

图论在 **CSP-S T3/T4** 出现频率高：最短路、生成树、拓扑、二分图匹配（部分卷）、树上问题等。本仓库 `problems/` 以入门模拟为主；建议将 **CSP 历年题** 在洛谷按「最短路」「最小生成树」「拓扑排序」筛选，与本文算法一一对应练习。

---

## 复杂度对比表

| 问题 | 算法 | 典型复杂度 | 备注 |
|------|------|------------|------|
| 单源最短路（非负） | Dijkstra | O((n+m) log n) | 堆实现 |
| 单源（可有负权） | Bellman-Ford | O(nm) | 负环检测 |
| 全源最短路 | Floyd | O(n³) | n 不宜过大 |
| MST | Kruskal | O(m log m) | 并查集 |
| MST | Prim | O(m log n) | 堆优化 |
| 拓扑排序 | BFS/DFS | O(n+m) | DAG |
| SCC | Tarjan | O(n+m) | 有向图 |

---

## Dijkstra 骨架（邻接表 + pair）

```cpp
using ll = long long;
const ll INF = (1LL << 60);

vector<ll> dijkstra(int s, const vector<vector<pair<int,int>>>& g) {
    int n = (int)g.size();
    vector<ll> d(n, INF);
    d[s] = 0;
    using P = pair<ll,int>;
    priority_queue<P, vector<P>, greater<P>> pq;
    pq.push({0, s});
    while (!pq.empty()) {
        auto [du, u] = pq.top(); pq.pop();
        if (du != d[u]) continue;
        for (auto [v, w] : g[u]) {
            if (d[v] > du + w) {
                d[v] = du + w;
                pq.push({d[v], v});
            }
        }
    }
    return d;
}
```

---

## 网格图上的 BFS

将格子 `(i, j)` 映射为 id `i * m + j` 或直接二维访问；障碍格不入队。网格最短路（四连通、边权 1）是 CSP 常见建模入口。

---

## 二分图染色（判二分图）

无向图用两种颜色染色，相邻点不同色；DFS/BFS 扩展，若冲突则非二分图。可用于 **匹配**、**奇环检测** 等题的前置判断。

```cpp
vector<int> color(n, -1);
function<bool(int,int)> dfs = [&](int u, int c) -> bool {
    color[u] = c;
    for (int v : adj[u]) {
        if (color[v] == -1) { if (!dfs(v, c ^ 1)) return false; }
        else if (color[v] == c) return false;
    }
    return true;
};
```

---

## Kruskal 与并查集（骨架）

```cpp
struct Edge { int u, v, w; };
bool cmpE(const Edge& a, const Edge& b) { return a.w < b.w; }

long long kruskal(vector<Edge>& edges, int n) {
    sort(edges.begin(), edges.end(), cmpE);
    DSU dsu(n);
    long long sum = 0;
    for (auto& e : edges) {
        if (dsu.unite(e.u, e.v)) sum += e.w;
    }
    return sum; // 若边数不足 n-1 则图不连通，需额外判断
}
```

（`DSU` 定义见 `topics/data-structures.md`。）

---

## 拓扑排序 BFS 骨架

```cpp
vector<int> indeg(n, 0);
for (/* 每条边 u->v */) indeg[v]++;
queue<int> q;
for (int i = 0; i < n; ++i) if (indeg[i] == 0) q.push(i);
vector<int> order;
while (!q.empty()) {
    int u = q.front(); q.pop();
    order.push_back(u);
    for (int v : adj[u]) {
        if (--indeg[v] == 0) q.push(v);
    }
}
// 若 order.size() != n 则有环
```

---

## 小结

建图方式决定后续一切：**先数清点数、边数、有向/无向、权值符号**。BFS/DFS 是基础，最短路与 MST 是 CSP 高频工具；拓扑与 Tarjan 面向更难题。与 `topics/data-structures.md` 中的并查集、堆配合使用。

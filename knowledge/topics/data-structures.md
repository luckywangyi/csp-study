# 数据结构专题（CSP）

## STL 容器总览

| 容器 | 特点 | 典型用途 |
|------|------|----------|
| `vector` | 连续内存、随机访问 O(1) | 动态数组、邻接表存边 |
| `deque` | 双端队列 | 单调队列底层、滑动窗口 |
| `list` | 双向链表 | 较少用；需 O(1) 插入删除中间节点时 |
| `set` / `multiset` | 有序、去重/可重复 | 动态有序集合、第 k 大（配合迭代） |
| `map` / `multimap` | 键有序 | 计数、离散化后索引 |
| `unordered_set` / `unordered_map` | 哈希、均摊 O(1) | 快速查找；注意哈希冲突与是否需有序 |
| `stack` | LIFO | DFS、单调栈、表达式 |
| `queue` | FIFO | BFS、拓扑排序 |
| `priority_queue` | 堆 | 最短路、合并果子、TopK |

---

## 选择合适数据结构的指南

1. **需要按下标随机访问** → `vector`。  
2. **需要有序 + 去重/范围遍历** → `set`/`map`。  
3. **只需存在性、不关心顺序** → `unordered_set`/`unordered_map`（注意是否卡哈希）。  
4. **先处理优先级高/小** → `priority_queue`。  
5. **一层层扩展** → `queue`；**回溯/括号** → `stack`。  
6. **区间最值滑动** → 单调队列；**作为子问题** → 线段树/树状数组。

---

## 并查集（Union-Find）

### 用途

- 连通块、Kruskal、动态连通性（基础版）。  
- 扩展：路径压缩、按秩/按大小合并。

### 模板骨架

```cpp
struct DSU {
    vector<int> p, r;
    DSU(int n = 0) { init(n); }
    void init(int n) {
        p.resize(n);
        r.assign(n, 0);
        iota(p.begin(), p.end(), 0);
    }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (r[a] < r[b]) swap(a, b);
        p[b] = a;
        if (r[a] == r[b]) ++r[a];
        return true;
    }
};
```

---

## 线段树与树状数组

### 树状数组（Fenwick）

- **适用**：单点修改 + 前缀和；可扩展区间加/查（差分思想）。  
- **复杂度**：更新与查询均为 O(log n)。  
- **特点**：代码短，常数小。

### 线段树

- **适用**：区间修改、区间查询（和、最值、 gcd 等）。  
- **复杂度**：O(log n) 每次操作。  
- **特点**：可懒标记；比树状数组灵活，代码更长。

CSP 中若数据范围允许 **暴力或分块**，可先用部分分；正解常是树状数组/线段树之一。

---

## 单调栈与单调队列

### 单调栈

- **典型**：求「每个元素左/右第一个更大/更小」、柱状图最大矩形、接雨水变形。  
- **维护**：栈内元素单调递增或递减，入栈时弹出破坏单调性的顶。

### 单调队列

- **典型**：滑动窗口最值。  
- **实现**：`deque` 存下标，头部为当前窗口最值候选，过期下标从队头弹出。

```cpp
// 窗口最小值： deque 递增存下标
deque<int> dq;
for (int i = 0; i < n; ++i) {
    while (!dq.empty() && a[dq.back()] >= a[i]) dq.pop_back();
    dq.push_back(i);
    while (dq.front() <= i - k) dq.pop_front();
    if (i >= k - 1) /* a[dq.front()] 为以 i 结尾的窗口最小值 */;
}
```

---

## CSP 中的数据结构真题

- **T2/T3**：`priority_queue`、`map` 统计、堆 + 贪心。  
- **T3/T4**：并查集、线段树/树状数组、单调栈/队列与 DP/图结合。  

本仓库 `problems/201612/t1-中间数` 涉及排序与中位数思想；`problems/201312/t1-出现次数最多的数` 可用 `map` 计数。更多综合题建议在官方/洛谷按标签扩展。

---

## 与算法的搭配

| 场景 | 数据结构 |
|------|----------|
| Kruskal | 并查集 |
| Dijkstra | `priority_queue` |
| 滑动窗口最值 | 单调队列 |
| 逆序对 / 动态前缀 | 树状数组、归并排序 |
| 区间和查询 | 树状数组、线段树 |

---

## 树状数组（Fenwick）模板：单点加、前缀和

```cpp
struct BIT {
    int n;
    vector<long long> t;
    BIT(int n = 0) { init(n); }
    void init(int n_) { n = n_; t.assign(n + 1, 0); }
    void add(int i, long long x) {
        for (; i <= n; i += i & -i) t[i] += x;
    }
    long long sum(int i) const {
        long long s = 0;
        for (; i > 0; i -= i & -i) s += t[i];
        return s;
    }
    // 若下标从 0 开始，可在调用处统一 +1 映射到 BIT 内部
};
```

---

## `priority_queue` 小技巧

- 默认 **大顶堆**；要小顶堆用 `greater` 或存负权。  
- 自定义结构体需重载 `operator<` 或写比较函数（注意 **严格弱序**）。  
- Dijkstra 中堆元素建议存 `(dist, node)`，用 `greater` 按 `dist` 升序。

---

## `unordered_map` 被卡哈希时的替代

可换 `map`，或使用 **自定义哈希**（如 splitmix64）；CSP 环境若不确定，优先用 `map`/`set` 求稳（注意多一个 log 因子）。

---

## ST 表与 RMQ（选学）

静态区间最值查询可 O(n log n) 预处理、O(1) 查询；与线段树相比 **无修改** 时更省常数。适合「多次查询、数组不变」的子任务。

---

## 离散化

坐标值域大但个数少时，排序去重后用 `lower_bound` 映射到 `[0, k)`，配合树状数组/线段树。

```cpp
vector<int> xs = raw;
sort(xs.begin(), xs.end());
xs.erase(unique(xs.begin(), xs.end()), xs.end());
int id = (int)(lower_bound(xs.begin(), xs.end(), v) - xs.begin());
```

---

## `vector` 性能习惯

- `reserve(n)` 预知大小时先预留，减少 realloc。  
- 清空复用：`clear()` 后 `size` 为 0，**capacity 可能仍大**，适合多测复用同一容器。  
- 二维 `vector`：`g.assign(n, vector<int>());` 比多次 `push_back` 空行更清晰。

---

## `pair` 与 `tuple` 在排序中的使用

```cpp
vector<pair<int,int>> a;
sort(a.begin(), a.end(), [](const auto& x, const auto& y) {
    if (x.first != y.first) return x.first < y.first;
    return x.second < y.second;
});
```

CSP 中常把 **值与原下标** 绑成 `pair` 再排序，避免丢失位置信息。

---

## 栈与递归深度

深度极大的 DFS 递归可能 **栈溢出**；可改 **显式栈** 模拟递归，或 `g++` 扩栈（视考场是否允许）。BFS 一般用队列，无此问题。

---

## 可持久化与平衡树（选学）

可持久化线段树、FHQ Treap、Splay 等属于提高组以上扩展。CSP 冲刺阶段 **优先掌握** 树状数组、线段树、并查集、堆即可，避免贪多嚼不烂。

---

## 小结

**先想清楚操作序列**：单点还是区间？是否要有序？是否重复？再选容器。CSP 考场时间紧，建议 **默写并查集、堆版 Dijkstra、树状数组** 等最短模板，避免现场拼错边界。图论部分见 `topics/graph.md`。

# CSP 考前速查表（30 分钟版）

> 开考前快速过一遍：模板 → STL → 片段 → I/O → 坑 → 复杂度 → 策略。

---

## 1. 解题模板

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
#define fi first
#define se second
#define pb push_back
#define mp make_pair
#define rep(i, a, b) for (int i = (a); i < (b); ++i)
const int inf = 0x3f3f3f3f;
const ll linf = 0x3f3f3f3f3f3f3f3fLL;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // 多测时视情况: int T; cin >> T; while (T--) { ... }
    return 0;
}
```

---

## 2. 常用 STL 操作速查

### vector

```cpp
vector<int> a(n), b(n, 0);           // 长度 n / n 个 0
a.push_back(x); a.pop_back();
sort(a.begin(), a.end());            // 升序
sort(a.begin(), a.end(), greater<int>());
a.erase(unique(a.begin(), a.end()), a.end());  // 先 sort 再去重
int p = lower_bound(a.begin(), a.end(), x) - a.begin();  // 第一个 >= x
```

### string

```cpp
string s; cin >> s;                  // 无空格
getline(cin, s);                     // 整行（注意前面若有 cin>> 先 getline 吃掉换行）
s.substr(pos, len); s.find("ab");    // npos 表示未找到
int x = stoi(s); ll y = stoll(s); to_string(123);
```

### map / unordered_map

```cpp
map<string, int> mp;
mp["a"]++; mp.count("a");            // 0/1
for (auto &kv : mp) cout << kv.first << kv.second;
unordered_map<ll, int> ump;          // 键为自定义结构体需自定义 hash（一般用 map）
```

### set / multiset

```cpp
set<int> st; st.insert(x); st.erase(x); st.count(x);
auto it = st.lower_bound(x);         // 第一个 >= x
multiset<int> ms; ms.insert(x); ms.erase(ms.find(x));  // 删一个
```

### priority_queue

```cpp
priority_queue<int> pq;              // 大顶堆（默认 less）
priority_queue<int, vector<int>, greater<int>> pq2;  // 小顶堆
// 自定义：struct Cmp { bool operator()(const T&a,const T&b) const { return ...; } };
// priority_queue<T, vector<T>, Cmp> pq;
```

### stack / queue / deque

```cpp
stack<int> sk; sk.push(x); sk.top(); sk.pop(); sk.empty(); sk.size();
queue<int> q; q.push(x); q.front(); q.pop();
deque<int> dq; dq.push_front(x); dq.push_back(x); dq.pop_front(); dq.pop_back();
```

---

## 3. 常用算法代码片段

### 二分查找（手写，有序数组）

```cpp
int l = 0, r = n - 1, ans = -1;
while (l <= r) {
    int mid = (l + r) >> 1;
    if (a[mid] >= x) { ans = mid; r = mid - 1; }
    else l = mid + 1;
}
```

### BFS（网格四连通示例）

```cpp
queue<pii> q; q.push({sx, sy}); vis[sx][sy] = 1;
int dx[4] = {1,-1,0,0}, dy[4] = {0,0,1,-1};
while (!q.empty()) {
    pii cur = q.front(); q.pop();
    int x = cur.fi, y = cur.se;
    rep(k,0,4) {
        int nx = x+dx[k], ny = y+dy[k];
        if (nx<0||nx>=n||ny<0||ny>=m||vis[nx][ny]) continue;
        vis[nx][ny]=1; q.push({nx,ny});
    }
}
```

### DFS（回溯框架）

```cpp
void dfs(int u) {
    vis[u] = 1;
    for (int v : g[u]) if (!vis[v]) dfs(v);
    // 或：选/不选、标记恢复
}
```

### Dijkstra（非负权，邻接表 + 小顶堆）

```cpp
vector<pll> g[N]; vector<ll> d(N, linf); d[s] = 0;
priority_queue<pll, vector<pll>, greater<pll>> pq;
pq.push({0, s});
while (!pq.empty()) {
    pll t = pq.top(); pq.pop();
    ll du = t.fi; int u = (int)t.se;
    if (du != d[u]) continue;
    for (pll &e : g[u]) {
        ll w = e.fi; int v = (int)e.se;
        if (d[v] > du + w) { d[v] = du + w; pq.push({d[v], v}); }
    }
}
```

### 并查集

```cpp
int fa[N];
int find(int x) { return fa[x]==x ? x : fa[x]=find(fa[x]); }
void unite(int a,int b) { a=find(a); b=find(b); if(a!=b) fa[a]=b; }
// 初始化: iota(fa, fa+n+1, 0); 或 rep(i,1,n) fa[i]=i;
```

### 01 背包（一维滚动）

```cpp
vector<int> dp(W+1);
for (int i = 0; i < n; ++i)
    for (int w = W; w >= wi[i]; --w)
        dp[w] = max(dp[w], dp[w-wi[i]] + vi[i]);
```

### Kruskal（边排序 + 并查集）

```cpp
sort(es.begin(), es.end(), [](auto&a,auto&b){ return a.w < b.w; });
ll sum = 0; int cnt = 0;
for (auto &e : es) if (find(e.u) != find(e.v)) {
    unite(e.u, e.v); sum += e.w; if (++cnt == n-1) break;
}
```

### 拓扑排序（Kahn，入度）

```cpp
queue<int> q; rep(i,1,n) if (!indeg[i]) q.push(i);
vector<int> ord;
while (!q.empty()) {
    int u = q.front(); q.pop(); ord.pb(u);
    for (int v : g[u]) if (--indeg[v] == 0) q.push(v);
}
// ord.size()==n 则无环
```

### 前缀和

```cpp
// 一维: s[0]=0; rep(i,1,n) s[i]=s[i-1]+a[i];  // 区间 [l,r] : s[r]-s[l-1]
// 二维: s[i][j]=a[i][j]+s[i-1][j]+s[i][j-1]-s[i-1][j-1];
// 子矩阵 (x1,y1)-(x2,y2): s[x2][y2]-s[x1-1][y2]-s[x2][y1-1]+s[x1-1][y1-1]
```

### 快速幂

```cpp
ll qpow(ll a, ll b, ll mod) {
    ll r = 1 % mod; a %= mod;
    for (; b; b >>= 1) { if (b & 1) r = r * a % mod; a = a * a % mod; }
    return r;
}
```

### GCD / LCM

```cpp
ll gcd(ll a, ll b) { return b ? gcd(b, a % b) : a; }
ll lcm(ll a, ll b) { return a / gcd(a, b) * b; }  // 防溢出可先除
```

### 埃氏筛（素数）

```cpp
vector<bool> isp(n + 1, true); isp[0] = isp[1] = false;
for (int i = 2; i * i <= n; ++i) if (isp[i])
    for (int j = i * i; j <= n; j += i) isp[j] = false;
```

---

## 4. 输入输出技巧

### 快速 IO

```cpp
ios::sync_with_stdio(false);
cin.tie(nullptr);
// 极端大数据可配合 scanf/printf 或 fread（考场一般用上面两句足够）
```

### 读取整行 / 混合读入

```cpp
string line; getline(cin, line);
cin >> n; cin.ignore(); getline(cin, line);  // 整数后接一行时先 ignore
```

### 多组输入

```cpp
int T; cin >> T;
while (T--) { /* 每组记得清空 vector/map，重置计数器 */ }
// 或 while (cin >> n) { ... } 直到 EOF
```

### 格式化输出（`<iomanip>`）

```cpp
cout << fixed << setprecision(6) << x;
cout << setw(5) << setfill('0') << n;  // 宽度 5 前导 0
cout << hex << n << dec;               // 十六进制后记得 dec
```

---

## 5. 常见坑点提醒

| 坑 | 处理 |
|----|------|
| `int` 乘法/累加溢出 | 中间量、答案用 `long long`，乘法前强转 |
| 数组越界 | 比题意多开 5～10；下标从 0 还是从 1 统一好 |
| 无向图存边 | 通常开 `2 * m` 或 `2 * (m+5)` |
| 浮点比较 | `const double eps = 1e-9;` 用 `fabs(a-b)<eps` 或题目给定精度 |
| C 风格字符串 | 注意 `\0`、长度 `strlen`；优先 `string` |
| `sort` 比较器 | 必须**严格弱序**，禁止 `<=` 导致等价元素误判 |
| `lower_bound` 为空 / 越界 | 检查 `begin()==end()` 或下标是否 `n` |
| 多测 | 全局 `vis`/`cnt`/`vector` **每组清空**；图邻接表 `clear()` |
| `unique` | 必须先 `sort` |
| 模运算减法 | `(a - b % mod + mod) % mod` |
| 二分边界 | 明确是「第一个满足」还是「最后一个满足」，`l/r/mid` 写法配套 |

---

## 6. 复杂度速查表

| 算法 / 结构 | 时间（典型） | 空间 |
|-------------|--------------|------|
| 排序 `sort` | \(O(n \log n)\) | \(O(1)\)～\(O(n)\) |
| `unordered_map` 均摊 | \(O(1)\) | \(O(n)\) |
| `map`/`set` | 单次 \(O(\log n)\) | \(O(n)\) |
| 二分 | \(O(\log n)\) | \(O(1)\) |
| DFS/BFS | \(O(n+m)\) | \(O(n)\) |
| Dijkstra | \(O((n+m)\log n)\) | \(O(n+m)\) |
| 并查集（路径压缩） | 均摊 \(\alpha(n)\) | \(O(n)\) |
| Kruskal | \(O(m \log m)\) | \(O(m)\) |
| 拓扑排序 | \(O(n+m)\) | \(O(n+m)\) |
| 01 背包 | \(O(nW)\) | 可 \(O(W)\) |
| 快速幂 | \(O(\log b)\) | \(O(1)\) |
| 埃氏筛 | \(O(n \log\log n)\) | \(O(n)\) |

---

## 7. CSP 考试策略

### 时间分配（参考）

- **前 5～10 分钟**：通读全卷，标难度与部分分，决定顺序。
- **每题**：先写暴力拿部分分，再冲正解；别一题死磕到空。
- **留 15～20 分钟**：检查文件名、读写、边界、多测清空、样例与自造小数据。

### 做题顺序

- 先做**有思路且实现短**的，建立信心与分数。
- **T1/T2** 尽量稳；**T3/T4** 先部分分（暴力、特殊性质、小范围）。
- 同一题：**正确性 > 部分分 > 完美复杂度**（在时间内）。

### 调试技巧

- 小数据手算、对拍（暴力 vs 正解）、输出中间变量（记得提交前删掉）。
- 边界：`n=1`、全相同、最大/最小值、图不连通、空串。
- 编译警告打开；本地与评测环境差异（栈、栈溢出可考虑非递归或扩栈，视考场规则）。

### 部分分策略

- 数据范围分段：\(n \le 20\) 爆搜；\(n \le 500\) \(O(n^3)\)；\(n \le 5000\) \(O(n^2)\)；\(n \le 10^5\) \(O(n\log n)\) 等。
- 特殊图（树、链）、权值小、模数小，常对应子任务。
- 答案取模题：**全程取模**，减法加 `mod`。

---

**祝考试顺利：先稳模板与边界，再抢部分分，最后优化正解。**

/*
 * 并查集：路径压缩 + 按秩合并；带权（到父节点差分）。
 */
#include <bits/stdc++.h>
using namespace std;

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
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (r[a] < r[b]) swap(a, b);
        p[b] = a;
        if (r[a] == r[b]) r[a]++;
        return true;
    }
};

// 带权：d[x] 表示 x 与父节点关系（依题定义），合并时更新
struct WeightedDSU {
    vector<int> p;
    vector<long long> d;
    WeightedDSU(int n = 0) { init(n); }
    void init(int n) {
        p.resize(n);
        d.assign(n, 0);
        iota(p.begin(), p.end(), 0);
    }
    pair<int, long long> find(int x) {
        if (p[x] == x) return {x, 0};
        auto [r, w] = find(p[x]);
        d[x] += w;
        p[x] = r;
        return {p[x], d[x]};
    }
    // 若需约束 val(x)-val(y)=w，返回是否矛盾
    bool unite(int x, int y, long long w) {
        auto [rx, dx] = find(x);
        auto [ry, dy] = find(y);
        if (rx == ry) return (dx - dy) == w;
        p[ry] = rx;
        d[ry] = dx - dy - w; // 依具体题意调整式子
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    DSU dsu(5);
    dsu.unite(0, 1);
    (void)(dsu.find(0) == dsu.find(1));
    WeightedDSU wds(3);
    wds.unite(0, 1, 1);
    return 0;
}

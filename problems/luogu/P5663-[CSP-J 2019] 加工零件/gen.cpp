// P5663 加工零件 — 数据生成器
// 用法: gen.exe <seed> <n_max>  （n,m,q 不超过 n_max，且 n_max 截到 2000 以免过大）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = (argc >= 2) ? atoi(argv[1]) : 1;
    int n_max = (argc >= 3) ? max(1, atoi(argv[2])) : 8;
    n_max = min(n_max, 2000);
    mt19937 rng((unsigned)seed + 0x9e3779b9u);

    if (seed == 1) {
        cout << "3 2 6\n1 2\n2 3\n1 1\n2 1\n3 1\n1 2\n2 2\n3 2\n";
        return 0;
    }
    if (seed == 2) {
        cout << "5 5 5\n1 2\n2 3\n3 4\n4 5\n1 5\n1 1\n1 2\n1 3\n1 4\n1 5\n";
        return 0;
    }
    if (seed == 3) {
        // 孤立点 1：除 L=0 外奇偶均不可达（L>=1）
        cout << "3 0 4\n1 1\n1 2\n1 3\n1 4\n";
        return 0;
    }
    if (seed == 4) {
        // 完全图 K4：奇偶最短路均为 1
        cout << "4 6 4\n1 2\n1 3\n1 4\n2 3\n2 4\n3 4\n2 1\n3 1\n4 1\n4 2\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(2, n_max)(rng);
    int m = uniform_int_distribution<int>(1, min(n * (n - 1) / 2, n_max))(rng);
    int q = uniform_int_distribution<int>(1, n_max)(rng);

    set<pair<int, int>> edges;
    while ((int)edges.size() < m) {
        int u = uniform_int_distribution<int>(1, n)(rng);
        int v = uniform_int_distribution<int>(1, n)(rng);
        if (u == v) continue;
        if (u > v) swap(u, v);
        edges.emplace(u, v);
    }

    cout << n << ' ' << m << ' ' << q << '\n';
    for (auto [u, v] : edges) cout << u << ' ' << v << '\n';
    for (int i = 0; i < q; ++i) {
        int a = uniform_int_distribution<int>(1, n)(rng);
        long long L = uniform_int_distribution<long long>(1, (long long)min(1000, n_max))(rng);
        if (uniform_int_distribution<int>(0, 1)(rng)) L = uniform_int_distribution<long long>(1, 1000000000LL)(rng);
        cout << a << ' ' << L << '\n';
    }
    return 0;
}

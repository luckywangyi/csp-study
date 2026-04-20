// P8817 假期计划 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    int n_max = max(5, atoi(argv[2]));
    mt19937 rng((unsigned)seed);

    if (seed == 1) {
        cout << "8 8 1\n9 7 1 8 2 3 6\n"
                "1 2\n2 3\n3 4\n4 5\n5 6\n6 7\n7 8\n8 1\n";
        return 0;
    }
    if (seed == 2) {
        cout << "7 9 0\n1 1 1 2 3 4\n"
                "1 2\n2 3\n3 4\n1 5\n1 6\n1 7\n5 4\n6 4\n7 4\n";
        return 0;
    }
    if (seed == 3) {
        cout << "5 4 1\n1 10 100 1000\n"
                "1 2\n2 3\n3 4\n4 5\n";
        return 0;
    }
    if (seed == 4) {
        cout << "6 6 0\n5 4 3 2 1\n"
                "1 2\n2 3\n3 4\n4 5\n5 6\n6 1\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(5, min(n_max, 18))(rng);
    int m = uniform_int_distribution<int>(n - 1, min(n * (n - 1) / 2, n_max * 2))(rng);
    int k = uniform_int_distribution<int>(0, min(5, n_max))(rng);
    cout << n << " " << m << " " << k << "\n";
    for (int i = 2; i <= n; i++) {
        if (i > 2) cout << ' ';
        cout << uniform_int_distribution<int>(1, 20)(rng);
    }
    cout << "\n";
    set<pair<int, int>> edges;
    for (int i = 2; i <= n; i++) edges.insert({i - 1, i});
    edges.insert({n, 1});
    while ((int)edges.size() < m) {
        int u = uniform_int_distribution<int>(1, n)(rng);
        int v = uniform_int_distribution<int>(1, n)(rng);
        if (u == v) continue;
        if (u > v) swap(u, v);
        edges.insert({u, v});
    }
    for (auto e : edges) cout << e.first << " " << e.second << "\n";
    return 0;
}

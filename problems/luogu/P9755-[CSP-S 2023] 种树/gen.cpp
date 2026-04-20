// P9755 种树 — 数据生成器
// 用法: gen.exe <seed> <n_max>  （随机时 n 不超过 min(n_max, 7) 以便暴力）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    mt19937 rng((unsigned)seed);
    int n_max = max(1, atoi(argv[2]));

    auto print_tree = [&](int n, const vector<array<long long, 3>>& node,
                          const vector<pair<int, int>>& edges) {
        cout << n << '\n';
        for (int i = 1; i <= n; i++) {
            cout << node[i][0] << ' ' << node[i][1] << ' ' << node[i][2] << '\n';
        }
        for (auto& e : edges) cout << e.first << ' ' << e.second << '\n';
    };

    if (seed == 1) {
        int n = 4;
        vector<array<long long, 3>> node(n + 1);
        node[1] = {12, 1, 1};
        node[2] = {2, 4, -1};
        node[3] = {10, 3, 0};
        node[4] = {7, 10, -2};
        vector<pair<int, int>> edges = {{1, 2}, {1, 3}, {3, 4}};
        print_tree(n, node, edges);
        return 0;
    }
    if (seed == 2) {
        int n = 1;
        vector<array<long long, 3>> node(n + 1);
        node[1] = {5, 1, 0};
        print_tree(n, node, {});
        return 0;
    }
    if (seed == 3) {
        int n = 2;
        vector<array<long long, 3>> node(n + 1);
        node[1] = {10, 1, 0};
        node[2] = {10, 1, 0};
        vector<pair<int, int>> edges = {{1, 2}};
        print_tree(n, node, edges);
        return 0;
    }
    if (seed == 4) {
        int n = 3;
        vector<array<long long, 3>> node(n + 1);
        node[1] = {100, 2, 0};
        node[2] = {100, 2, 0};
        node[3] = {100, 2, 0};
        vector<pair<int, int>> edges = {{1, 2}, {2, 3}};
        print_tree(n, node, edges);
        return 0;
    }

    int n = uniform_int_distribution<int>(2, min(n_max, 7))(rng);
    vector<array<long long, 3>> node(n + 1);
    for (int i = 1; i <= n; i++) {
        node[i][0] = uniform_int_distribution<long long>(5, 200)(rng);
        node[i][1] = uniform_int_distribution<long long>(1, 5)(rng);
        node[i][2] = uniform_int_distribution<long long>(-2, 2)(rng);
    }
    vector<pair<int, int>> edges;
    for (int v = 2; v <= n; v++) {
        int u = uniform_int_distribution<int>(1, v - 1)(rng);
        edges.emplace_back(u, v);
    }
    print_tree(n, node, edges);
    return 0;
}

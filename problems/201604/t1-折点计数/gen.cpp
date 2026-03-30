// 201604-T1 折点计数 — 数据生成器
// 用法: gen.exe <seed> <n_max>
// 数据范围: 3 <= n <= n_max, 元素互不相同
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char* argv[]) {
    mt19937 rng((unsigned)atoi(argv[1]));
    int n_max = max(3, atoi(argv[2]));
    int n = uniform_int_distribution<int>(3, n_max)(rng);
    // 生成 n 个互不相同的整数
    set<int> used;
    vector<int> a;
    while ((int)a.size() < n) {
        int v = uniform_int_distribution<int>(-100000, 100000)(rng);
        if (used.insert(v).second) a.push_back(v);
    }
    cout << n << "\n";
    for (int i = 0; i < n; i++) {
        if (i) cout << ' ';
        cout << a[i];
    }
    cout << "\n";
}

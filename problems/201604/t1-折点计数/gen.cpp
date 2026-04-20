// 201604-T1 折点计数 — 数据生成器
// 用法: gen.exe <seed> <n_max>
// 数据范围: 3 <= n <= n_max, 元素互不相同
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    mt19937 rng((unsigned)seed);
    int n_max = max(3, atoi(argv[2]));

    // 边界用例
    if (seed == 1) {
        // 最小 n=3，单调递增，无折点
        cout << "3\n1 2 3\n";
        return 0;
    }
    if (seed == 2) {
        // n=3，有一个折点（峰）
        cout << "3\n1 3 2\n";
        return 0;
    }
    if (seed == 3) {
        // 单调递减，无折点
        int n = min(5, n_max);
        cout << n << "\n";
        for (int i = 0; i < n; i++) { if (i) cout << ' '; cout << n - i; }
        cout << "\n";
        return 0;
    }
    if (seed == 4) {
        // 交替序列，最多折点
        cout << "7\n1 100 2 99 3 98 4\n";
        return 0;
    }

    // 随机数据
    int n = uniform_int_distribution<int>(3, n_max)(rng);
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

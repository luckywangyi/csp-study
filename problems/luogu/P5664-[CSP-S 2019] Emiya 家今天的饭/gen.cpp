// P5664 Emiya 家今天的饭 — 数据生成器
// 用法: gen.exe <seed> <n_max>  （n,m 不超过题面且受 n_max 限制；高 seed 随机小系数便于暴力对拍）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = (argc >= 2) ? atoi(argv[1]) : 1;
    int n_max = (argc >= 3) ? max(1, atoi(argv[2])) : 5;
    n_max = min(n_max, 100);
    mt19937 rng((unsigned)seed + 0x9e3779b9u);

    if (seed == 1) {
        cout << "2 3\n1 0 1\n0 1 1\n";
        return 0;
    }
    if (seed == 2) {
        cout << "3 3\n1 2 3\n4 5 0\n6 0 0\n";
        return 0;
    }
    if (seed == 3) {
        cout << "5 5\n1 0 0 1 1\n0 1 0 1 0\n1 1 1 1 0\n1 0 1 0 1\n0 1 1 0 1\n";
        return 0;
    }
    if (seed == 4) {
        // n=1：退化情形
        cout << "1 3\n0 1 2\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(1, min(100, max(1, n_max)))(rng);
    int m = uniform_int_distribution<int>(1, min(2000, max(1, n_max * 20)))(rng);
    int hi = min<int>(998244353 - 1, max(3, n_max * 5));
    cout << n << ' ' << m << '\n';
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (j > 1) cout << ' ';
            cout << uniform_int_distribution<int>(0, hi)(rng);
        }
        cout << '\n';
    }
    return 0;
}

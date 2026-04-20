// P5662 纪念品 — 数据生成器
// 用法: gen.exe <seed> <n_max>  （T,N,M 及价格上界随 n_max 缩放，且满足题面范围）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = (argc >= 2) ? atoi(argv[1]) : 1;
    int n_max = (argc >= 3) ? max(1, atoi(argv[2])) : 6;
    n_max = min(n_max, 100);
    mt19937 rng((unsigned)seed + 0x9e3779b9u);

    if (seed == 1) {
        cout << "6 1 100\n50\n20\n25\n20\n25\n50\n";
        return 0;
    }
    if (seed == 2) {
        cout << "3 3 100\n10 20 15\n15 17 13\n15 25 16\n";
        return 0;
    }
    if (seed == 3) {
        // T=1：无法跨日套利
        cout << "1 2 50\n10 20\n";
        return 0;
    }
    if (seed == 4) {
        // 两天、多物品、小 M
        cout << "2 4 7\n1 2 3 4\n5 6 7 8\n";
        return 0;
    }

    int T = uniform_int_distribution<int>(1, min(100, max(2, n_max)))(rng);
    int N = uniform_int_distribution<int>(1, min(100, max(1, n_max)))(rng);
    int M = uniform_int_distribution<int>(1, min(1000, max(1, n_max * 10)))(rng);
    cout << T << ' ' << N << ' ' << M << '\n';
    for (int i = 0; i < T; ++i) {
        for (int j = 0; j < N; ++j) {
            if (j) cout << ' ';
            cout << uniform_int_distribution<int>(1, min(10000, max(2, n_max * 100)))(rng);
        }
        cout << '\n';
    }
    return 0;
}

// P7074 方格取数 — 数据生成器
// 用法: gen.exe <seed> <n_max>  （n,m 不超过 n_max，随机时控制规模以便暴力可跑）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    int n_max = max(1, atoi(argv[2]));
    mt19937 rng((unsigned)seed + 1337);

    if (seed == 1) {
        cout << "3 4\n1 -1 3 2\n2 -1 4 -1\n-2 2 -3 -1\n";
        return 0;
    }
    if (seed == 2) {
        cout << "2 5\n-1 -1 -3 -2 -7\n-2 -1 -4 -1 -2\n";
        return 0;
    }
    if (seed == 3) {
        cout << "1 1\n5\n";
        return 0;
    }
    if (seed == 4) {
        cout << "2 2\n1 -5\n-2 9\n";
        return 0;
    }

    int lim = min(n_max, 5);
    int n = uniform_int_distribution<int>(1, lim)(rng);
    int m = uniform_int_distribution<int>(1, lim)(rng);
    cout << n << ' ' << m << '\n';
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (j) cout << ' ';
            cout << uniform_int_distribution<int>(-8, 8)(rng);
        }
        cout << '\n';
    }
    return 0;
}

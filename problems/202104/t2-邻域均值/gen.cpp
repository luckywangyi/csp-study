// 202104-T2 邻域均值 — 数据生成器
// 用法: gen.exe <seed> <n_max>
// 数据范围: 1 <= n <= n_max, L=255 固定, 0 <= r <= n, 0 <= t <= L
// 矩阵元素 [0, L]
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char* argv[]) {
    mt19937 rng((unsigned)atoi(argv[1]));
    int n_max = max(1, atoi(argv[2]));
    int n = uniform_int_distribution<int>(1, n_max)(rng);
    int L = 255;
    int r = uniform_int_distribution<int>(0, min(n, 50))(rng);
    int t = uniform_int_distribution<int>(0, L)(rng);
    cout << n << " " << L << " " << r << " " << t << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j) cout << ' ';
            cout << uniform_int_distribution<int>(0, L)(rng);
        }
        cout << "\n";
    }
}

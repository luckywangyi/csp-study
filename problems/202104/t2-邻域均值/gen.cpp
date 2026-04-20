// 202104-T2 邻域均值 — 数据生成器
// 用法: gen.exe <seed> <n_max>
// 数据范围: 1 <= n <= n_max, L=255, 0 <= r <= n, 0 <= t <= L
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    mt19937 rng((unsigned)seed);
    int n_max = max(1, atoi(argv[2]));

    // 边界用例
    if (seed == 1) {
        // 1x1 矩阵，r=0
        cout << "1 255 0 100\n50\n";
        return 0;
    }
    if (seed == 2) {
        // 全零矩阵，t=0，所有格子均满足
        cout << "3 255 1 0\n";
        for (int i = 0; i < 3; i++) cout << "0 0 0\n";
        return 0;
    }
    if (seed == 3) {
        // 全 255 矩阵，t=0，没有格子满足
        cout << "2 255 1 0\n";
        for (int i = 0; i < 2; i++) cout << "255 255\n";
        return 0;
    }
    if (seed == 4) {
        // r 很大（覆盖整个矩阵）
        cout << "3 255 100 127\n";
        cout << "0 0 0\n100 200 255\n50 128 64\n";
        return 0;
    }

    // 随机数据
    int n = uniform_int_distribution<int>(1, n_max)(rng);
    int L = 255;
    int r = uniform_int_distribution<int>(0, n)(rng);
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

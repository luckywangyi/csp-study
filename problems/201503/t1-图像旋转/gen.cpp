// 201503-T1 图像旋转 — 数据生成器
// 用法: gen.exe <seed> <n_max>
// 数据范围: 1 <= n, m <= n_max, 元素 [0, 255]
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    mt19937 rng((unsigned)seed);
    int n_max = atoi(argv[2]);

    // 边界用例
    if (seed == 1) {
        // 1x1 矩阵
        cout << "1 1\n42\n";
        return 0;
    }
    if (seed == 2) {
        // 1 行多列
        cout << "1 4\n1 2 3 4\n";
        return 0;
    }
    if (seed == 3) {
        // 多行 1 列
        cout << "3 1\n10\n20\n30\n";
        return 0;
    }
    if (seed == 4) {
        // 方阵，全零
        cout << "3 3\n0 0 0\n0 0 0\n0 0 0\n";
        return 0;
    }

    // 随机数据
    int n = uniform_int_distribution<int>(1, n_max)(rng);
    int m = uniform_int_distribution<int>(1, n_max)(rng);
    cout << n << " " << m << "\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (j) cout << ' ';
            cout << uniform_int_distribution<int>(0, 255)(rng);
        }
        cout << "\n";
    }
}

// 201503-T1 图像旋转 — 数据生成器
// 用法: gen.exe <seed> <n_max>
// 数据范围: 1 <= n, m <= n_max, 元素 [0, 255]
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char* argv[]) {
    mt19937 rng((unsigned)atoi(argv[1]));
    int n_max = atoi(argv[2]);
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

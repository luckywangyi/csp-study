// P11234 擂台游戏 — 数据生成器
// 用法: gen.exe <seed> <n_max>
// 为保证 brute 可枚举补充选手，随机数据会让询问的 c 接近 n，从而自由位较少。
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    int n_max = max(2, atoi(argv[2]));
    mt19937 rng((unsigned)seed);

    if (seed == 1) {
        cout << "5 5\n0 0 0 0 0\n5 4 1 2 3\n1001\n10\n1\n4\n2 1 0 0\n1 2 1 0\n0 2 3 1\n2 2 0 1\n";
        return 0;
    }
    if (seed == 2) {
        // n=4,K=2：两轮抽签长度分别为 2 与 1
        cout << "4 4\n1 2 3 4\n4 3 2 1\n00\n0\n1\n2 0 0 0\n";
        return 0;
    }
    if (seed == 3) {
        // n=3,K=2
        cout << "3 3\n5 5 5\n3 2 1\n00\n0\n1\n0 0 0 0\n";
        return 0;
    }
    if (seed == 4) {
        // n=2,K=1
        cout << "2 2\n0 1\n2 1\n0\n1\n0 0 0 0\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(2, min(n_max, 14))(rng);
    int m = uniform_int_distribution<int>(1, min(n_max, 6))(rng);
    cout << n << " " << m << "\n";
    for (int i = 0; i < n; i++) {
        if (i) cout << ' ';
        cout << uniform_int_distribution<int>(0, 20)(rng);
    }
    cout << "\n";
    for (int i = 1; i <= m; i++) {
        if (i > 1) cout << ' ';
        int c = n;
        if (i == m && n > 2) c = uniform_int_distribution<int>(max(2, n - 2), n)(rng);
        cout << c;
    }
    cout << "\n";
    int K = 0;
    while ((1 << K) < n) K++;
    for (int r = 1; r <= K; r++) {
        int len = 1 << (K - r);
        for (int i = 0; i < len; i++) cout << (rng() % 2);
        cout << "\n";
    }
    int T = uniform_int_distribution<int>(1, min(4, n_max > 4 ? 4 : n_max))(rng);
    cout << T << "\n";
    while (T--) {
        for (int i = 0; i < 4; i++) {
            if (i) cout << ' ';
            cout << uniform_int_distribution<int>(0, 5)(rng);
        }
        cout << "\n";
    }
    return 0;
}

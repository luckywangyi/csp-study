// P1162 填涂颜色 — 数据生成器
// 用法: gen.exe <seed> <n_max>   （方阵边长不超过 min(30, n_max)）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = (argc >= 2) ? atoi(argv[1]) : 1;
    int n_max = (argc >= 3) ? max(1, atoi(argv[2])) : 6;
    n_max = min(30, n_max);
    mt19937 rng((unsigned)seed + 0x9e3779b9u);

    if (seed == 1) {
        // 题面样例（闭合圈 + 圈内 0）
        cout << "6\n";
        cout << "0 0 0 0 0 0\n";
        cout << "0 0 1 1 1 1\n";
        cout << "0 1 1 0 0 1\n";
        cout << "1 1 0 0 0 1\n";
        cout << "1 0 0 0 0 1\n";
        cout << "1 1 1 1 1 1\n";
        return 0;
    }
    if (seed == 2) {
        // n=1，无圈内 0
        cout << "1\n0\n";
        return 0;
    }
    if (seed == 3) {
        // 最小圈：中心为 0，周围一圈 1
        int n = min(5, n_max);
        if (n < 3) n = 3;
        vector<vector<int>> b(n, vector<int>(n, 0));
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                if (i == 0 || j == 0 || i == n - 1 || j == n - 1) b[i][j] = 0;
                else if (i == 1 || j == 1 || i == n - 2 || j == n - 2) b[i][j] = 1;
                else b[i][j] = 0;
        cout << n << '\n';
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (j) cout << ' ';
                cout << b[i][j];
            }
            cout << '\n';
        }
        return 0;
    }
    if (seed == 4) {
        // 全 0：无 1 圈（边界全通）
        int n = min(4, n_max);
        cout << n << '\n';
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (j) cout << ' ';
                cout << 0;
            }
            cout << '\n';
        }
        return 0;
    }

    int n = uniform_int_distribution<int>(max(3, min(6, n_max)), n_max)(rng);
    // 随机生成外边界 0，内部随机 0/1，再用一圈 1 把内部围起来（保证有圈）
    vector<vector<int>> b(n, vector<int>(n, 0));
    for (int i = 1; i < n - 1; ++i)
        for (int j = 1; j < n - 1; ++j) b[i][j] = uniform_int_distribution<int>(0, 1)(rng);
    for (int i = 0; i < n; ++i) {
        b[0][i] = b[n - 1][i] = 0;
        b[i][0] = b[i][n - 1] = 0;
    }
    for (int i = 1; i < n - 1; ++i)
        for (int j = 1; j < n - 1; ++j)
            if (i == 1 || j == 1 || i == n - 2 || j == n - 2) b[i][j] = 1;

    cout << n << '\n';
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j) cout << ' ';
            cout << b[i][j];
        }
        cout << '\n';
    }
    return 0;
}

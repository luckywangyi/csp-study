// P5658 括号树 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    int n_max = max(2, atoi(argv[2]));
    mt19937 rng((unsigned)seed + 911382323);

    if (seed == 1) {
        cout << "5\n(()()\n1 1 2 2\n";
        return 0;
    }
    if (seed == 2) {
        cout << "2\n()\n1\n";
        return 0;
    }
    if (seed == 3) {
        int n = min(8, n_max);
        cout << n << '\n';
        for (int i = 1; i <= n; ++i) cout << '(';
        cout << '\n';
        for (int i = 2; i <= n; ++i) {
            if (i > 2) cout << ' ';
            cout << i - 1;
        }
        cout << '\n';
        return 0;
    }
    if (seed == 4) {
        int n = min(9, n_max);
        cout << n << '\n';
        for (int i = 1; i <= n; ++i) cout << (i % 2 ? '(' : ')');
        cout << '\n';
        for (int i = 2; i <= n; ++i) {
            if (i > 2) cout << ' ';
            cout << 1;
        }
        cout << '\n';
        return 0;
    }

    int n = uniform_int_distribution<int>(2, min(n_max, 28))(rng);
    cout << n << '\n';
    for (int i = 1; i <= n; ++i) cout << (uniform_int_distribution<int>(0, 1)(rng) ? '(' : ')');
    cout << '\n';
    for (int i = 2; i <= n; ++i) {
        if (i > 2) cout << ' ';
        int hi = i - 1;
        cout << uniform_int_distribution<int>(1, hi)(rng);
    }
    cout << '\n';
    return 0;
}

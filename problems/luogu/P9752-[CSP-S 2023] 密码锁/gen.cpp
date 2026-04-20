// P9752 密码锁 — 数据生成器
// 用法: gen.exe <seed> <n_max>  （n 为状态数，题目 n<=8）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    mt19937 rng((unsigned)seed);
    int n_max = max(1, min(8, atoi(argv[2])));

    auto line = [](array<int, 5> a) {
        for (int i = 0; i < 5; i++) {
            if (i) cout << ' ';
            cout << a[i];
        }
        cout << '\n';
    };

    if (seed == 1) {
        cout << "1\n";
        line({0, 0, 1, 1, 5});
        return 0;
    }
    if (seed == 2) {
        cout << "1\n";
        line({1, 2, 3, 4, 5});
        return 0;
    }
    if (seed == 3) {
        cout << "2\n";
        line({0, 0, 0, 0, 1});
        line({0, 0, 0, 0, 9});
        return 0;
    }
    if (seed == 4) {
        cout << "8\n";
        for (int i = 0; i < 8; i++) {
            line({i, i, i, i, i});
        }
        return 0;
    }

    int n = uniform_int_distribution<int>(1, n_max)(rng);
    cout << n << '\n';
    uniform_int_distribution<int> d(0, 9);
    for (int i = 0; i < n; i++) {
        line({d(rng), d(rng), d(rng), d(rng), d(rng)});
    }
    return 0;
}

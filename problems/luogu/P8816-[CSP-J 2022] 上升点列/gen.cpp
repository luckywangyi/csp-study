// P8816 上升点列 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    int n_max = max(1, atoi(argv[2]));
    mt19937 rng((unsigned)seed);

    if (seed == 1) {
        cout << "8 2\n"
                "3 1\n3 2\n3 3\n3 6\n1 2\n2 2\n5 5\n5 3\n";
        return 0;
    }
    if (seed == 2) {
        cout << "4 100\n10 10\n15 25\n20 20\n30 30\n";
        return 0;
    }
    if (seed == 3) {
        int n = min(5, n_max);
        cout << n << " 0\n";
        for (int i = 1; i <= n; i++) cout << i << " " << i << "\n";
        return 0;
    }
    if (seed == 4) {
        cout << "3 5\n1 1\n3 1\n5 1\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(1, min(n_max, 40))(rng);
    int k = uniform_int_distribution<int>(0, min(100, n_max))(rng);
    cout << n << " " << k << "\n";
    set<pair<int, int>> used;
    for (int i = 0; i < n; i++) {
        int x, y;
        do {
            x = uniform_int_distribution<int>(1, min(50, n_max))(rng);
            y = uniform_int_distribution<int>(1, min(50, n_max))(rng);
        } while (!used.insert({x, y}).second);
        cout << x << " " << y << "\n";
    }
    return 0;
}

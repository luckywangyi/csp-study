// P11229 小木棍 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    int n_max = max(2, atoi(argv[2]));
    mt19937 rng((unsigned)seed);

    if (seed == 1) {
        cout << "5\n1\n2\n3\n6\n18\n";
        return 0;
    }
    if (seed == 2) {
        cout << "1\n2\n";
        return 0;
    }
    if (seed == 3) {
        cout << "1\n7\n";
        return 0;
    }
    if (seed == 4) {
        cout << "3\n100\n101\n102\n";
        return 0;
    }

    int T = uniform_int_distribution<int>(1, min(50, n_max > 50 ? 50 : n_max))(rng);
    cout << T << "\n";
    while (T--) {
        int n = uniform_int_distribution<int>(2, min(n_max, 5000))(rng);
        cout << n << "\n";
    }
    return 0;
}

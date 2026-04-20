// P11233 染色 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    int n_max = max(2, atoi(argv[2]));
    mt19937 rng((unsigned)seed);

    if (seed == 1) {
        cout << "3\n3\n1 2 1\n4\n1 2 3 4\n8\n3 5 2 5 1 2 1 4\n";
        return 0;
    }
    if (seed == 2) {
        cout << "1\n2\n7 7\n";
        return 0;
    }
    if (seed == 3) {
        cout << "1\n5\n1 1 1 1 1\n";
        return 0;
    }
    if (seed == 4) {
        cout << "1\n6\n1 2 1 2 1 2\n";
        return 0;
    }

    int T = uniform_int_distribution<int>(1, min(10, n_max > 10 ? 10 : n_max))(rng);
    cout << T << "\n";
    while (T--) {
        int n = uniform_int_distribution<int>(2, min(n_max, 2000))(rng);
        cout << n << "\n";
        for (int i = 0; i < n; i++) {
            if (i) cout << ' ';
            cout << uniform_int_distribution<int>(1, min(1000000, max(2, n_max)))(rng);
        }
        cout << "\n";
    }
    return 0;
}

// P11231 决斗 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    int n_max = max(1, atoi(argv[2]));
    mt19937 rng((unsigned)seed);

    if (seed == 1) {
        cout << "5\n1 2 3 1 2\n";
        return 0;
    }
    if (seed == 2) {
        cout << "10\n136 136 136 2417 136 136 2417 136 136 136\n";
        return 0;
    }
    if (seed == 3) {
        cout << "1\n42\n";
        return 0;
    }
    if (seed == 4) {
        cout << "4\n1 1 1 1\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(1, min(n_max, 100000))(rng);
    cout << n << "\n";
    for (int i = 0; i < n; i++) {
        if (i) cout << ' ';
        cout << uniform_int_distribution<int>(1, min(100000, max(1, n_max)))(rng);
    }
    cout << "\n";
    return 0;
}

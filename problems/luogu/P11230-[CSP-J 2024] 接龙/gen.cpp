// P11230 接龙 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    int n_max = max(2, atoi(argv[2]));
    mt19937 rng((unsigned)seed);

    if (seed == 1) {
        cout << "1\n3 3 7\n"
                "5 1 2 3 4 1\n"
                "3 1 2 5\n"
                "3 5 1 6\n"
                "1 2\n"
                "1 4\n"
                "2 4\n"
                "3 4\n"
                "6 6\n"
                "1 1\n"
                "7 7\n";
        return 0;
    }
    if (seed == 2) {
        cout << "1\n2 3 1\n4 1 2 3 1\n4 1 2 3 1\n1 3\n";
        return 0;
    }
    if (seed == 3) {
        cout << "1\n1 2 2\n3 1 2 1\n1 1\n1 1\n";
        return 0;
    }
    if (seed == 4) {
        cout << "1\n2 2 3\n4 1 1 1 1\n4 1 1 1 1\n2 1\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(2, min(n_max, 8))(rng);
    int k = uniform_int_distribution<int>(2, min(n_max, 8))(rng);
    int q = uniform_int_distribution<int>(1, min(n_max, 10))(rng);
    cout << "1\n" << n << " " << k << " " << q << "\n";
    int suml = 0;
    int maxl = min(15, n_max);
    for (int i = 0; i < n; i++) {
        int l = uniform_int_distribution<int>(2, maxl)(rng);
        suml += l;
        cout << l;
        for (int j = 0; j < l; j++) cout << " " << uniform_int_distribution<int>(1, min(20, n_max))(rng);
        cout << "\n";
    }
    for (int i = 0; i < q; i++) {
        int rr = uniform_int_distribution<int>(1, min(5, n_max))(rng);
        int cc = uniform_int_distribution<int>(1, 20)(rng);
        cout << rr << " " << cc << "\n";
    }
    return 0;
}

// P7910 插入排序 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    int n_max = max(1, atoi(argv[2]));
    mt19937 rng((unsigned)seed);

    if (seed == 1) {
        cout << "3 4\n3 2 1\n2 3\n1 3 2\n2 2\n2 3\n";
        return 0;
    }
    if (seed == 2) {
        cout << "1 3\n1000000000\n2 1\n1 1 1\n2 1\n";
        return 0;
    }
    if (seed == 3) {
        cout << "5 5\n1 1 1 1 1\n2 1\n2 2\n2 3\n2 4\n2 5\n";
        return 0;
    }
    if (seed == 4) {
        cout << "4 6\n4 3 2 1\n2 1\n2 4\n1 2 9\n2 2\n1 3 9\n2 3\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(1, min(n_max, 50))(rng);
    int q = uniform_int_distribution<int>(1, min(n_max, 200))(rng);
    cout << n << " " << q << "\n";
    for (int i = 1; i <= n; i++) {
        if (i > 1) cout << ' ';
        cout << uniform_int_distribution<int>(1, 20)(rng);
    }
    cout << "\n";
    int type1 = 0;
    const int cap1 = min(5000, max(1, q / 2));
    for (int i = 0; i < q; i++) {
        bool can1 = type1 < cap1 && n > 0;
        int op = (!can1 || uniform_int_distribution<int>(0, 2)(rng) > 0) ? 2 : 1;
        if (op == 1) {
            int x = uniform_int_distribution<int>(1, n)(rng);
            int v = uniform_int_distribution<int>(1, 100)(rng);
            cout << "1 " << x << " " << v << "\n";
            type1++;
        } else {
            int x = uniform_int_distribution<int>(1, n)(rng);
            cout << "2 " << x << "\n";
        }
    }
    return 0;
}

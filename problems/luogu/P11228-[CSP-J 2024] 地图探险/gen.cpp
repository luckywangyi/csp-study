// P11228 地图探险 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    int n_max = max(1, atoi(argv[2]));
    mt19937 rng((unsigned)seed);

    if (seed == 1) {
        cout << "1\n1 5 4\n1 1 2\n....x\n";
        return 0;
    }
    if (seed == 2) {
        cout << "1\n1 1 10\n1 1 0\n.\n";
        return 0;
    }
    if (seed == 3) {
        cout << "1\n2 2 1\n1 1 0\n..\n..\n";
        return 0;
    }
    if (seed == 4) {
        cout << "2\n3 3 5\n2 2 1\n...\n.x.\n...\n1 1 3\n1 1 0\n.\n";
        return 0;
    }

    int T = uniform_int_distribution<int>(1, min(5, n_max > 1 ? 5 : 1))(rng);
    cout << T << "\n";
    while (T--) {
        int n = uniform_int_distribution<int>(1, min(n_max, 50))(rng);
        int m = uniform_int_distribution<int>(1, min(n_max, 50))(rng);
        int k = uniform_int_distribution<int>(1, min(n_max * 100, 5000))(rng);
        int x0 = uniform_int_distribution<int>(1, n)(rng);
        int y0 = uniform_int_distribution<int>(1, m)(rng);
        int d0 = uniform_int_distribution<int>(0, 3)(rng);
        vector<string> g(n + 1, string(m + 1, 'x'));
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++) g[i][j] = (rng() % 5 == 0 ? 'x' : '.');
        g[x0][y0] = '.';
        cout << n << " " << m << " " << k << "\n";
        cout << x0 << " " << y0 << " " << d0 << "\n";
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) cout << g[i][j];
            cout << "\n";
        }
    }
    return 0;
}

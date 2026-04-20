// P11232 超速检测 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    int n_max = max(1, atoi(argv[2]));
    mt19937 rng((unsigned)seed);

    if (seed == 1) {
        cout << "1\n5 5 15 3\n0 3 0\n12 4 0\n1 1 4\n5 5 -2\n6 4 -4\n2 5 8 9 15\n";
        return 0;
    }
    if (seed == 2) {
        cout << "1\n1 3 100 10\n0 5 0\n0 20 0\n0 50 0\n10 40 90\n";
        return 0;
    }
    if (seed == 3) {
        cout << "1\n2 2 10 5\n0 10 0\n5 3 0\n9 10\n";
        return 0;
    }
    if (seed == 4) {
        cout << "1\n1 1 8 3\n0 2 3\n8\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(1, min(n_max, 30))(rng);
    int m = uniform_int_distribution<int>(1, min(n_max, 30))(rng);
    int L = uniform_int_distribution<int>(n_max, min(1000000, max(n_max, 20)))(rng);
    int V = uniform_int_distribution<int>(1, min(1000, max(1, n_max)))(rng);
    cout << "1\n" << n << " " << m << " " << L << " " << V << "\n";
    for (int i = 0; i < n; i++) {
        long long di = uniform_int_distribution<int>(0, max(0, L - 1))(rng);
        int vi = uniform_int_distribution<int>(1, 1000)(rng);
        int ai = uniform_int_distribution<int>(-1000, 1000)(rng);
        cout << di << " " << vi << " " << ai << "\n";
    }
    set<long long> ps;
    while ((int)ps.size() < m) ps.insert(uniform_int_distribution<int>(0, L)(rng));
    bool first = true;
    for (long long x : ps) {
        if (!first) cout << ' ';
        first = false;
        cout << x;
    }
    cout << "\n";
    return 0;
}

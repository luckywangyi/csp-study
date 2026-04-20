// P5657 格雷码 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    int n_max = max(1, atoi(argv[2]));
    mt19937_64 rng((uint64_t)seed + 0x9e3779b97f4a7c15ULL);

    if (seed == 1) {
        cout << "1 0\n";
        return 0;
    }
    if (seed == 2) {
        cout << "2 3\n";
        return 0;
    }
    if (seed == 3) {
        cout << "3 5\n";
        return 0;
    }
    if (seed == 4) {
        int n = min(64, max(1, n_max));
        unsigned long long k = 0;
        cout << n << ' ' << k << '\n';
        return 0;
    }

    int n = uniform_int_distribution<int>(1, min(64, n_max))(rng);
    unsigned long long mask = (n == 64) ? ~0ULL : ((1ULL << n) - 1ULL);
    unsigned long long k = uniform_int_distribution<unsigned long long>(0, mask)(rng);
    cout << n << ' ' << k << '\n';
    return 0;
}

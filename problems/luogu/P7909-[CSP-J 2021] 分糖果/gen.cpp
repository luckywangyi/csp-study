// P7909 分糖果 — 数据生成器
// 用法: gen.exe <seed> <n_max>  （n 不超过 n_max，R 随规模放大）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    long long n_max = max(2LL, atoll(argv[2]));
    mt19937_64 rng((uint64_t)seed + 19260817u);

    if (seed == 1) {
        cout << "7 16 23\n";
        return 0;
    }
    if (seed == 2) {
        cout << "10 14 18\n";
        return 0;
    }
    if (seed == 3) {
        cout << "2 2 5\n";
        return 0;
    }
    if (seed == 4) {
        long long n = min(n_max, 1000LL);
        cout << n << ' ' << n << ' ' << n << '\n';
        return 0;
    }

    long long n = uniform_int_distribution<long long>(2, min(n_max, 5000LL))(rng);
    long long span = uniform_int_distribution<long long>(0, min<long long>(100000, n * 50))(rng);
    long long L = n + uniform_int_distribution<long long>(0, min<long long>(10000, n_max))(rng);
    long long R = L + span;
    cout << n << ' ' << L << ' ' << R << '\n';
    return 0;
}

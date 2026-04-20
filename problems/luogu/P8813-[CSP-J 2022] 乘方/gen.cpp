// P8813 乘方 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    long long n_max = max(1LL, atoll(argv[2]));
    mt19937_64 rng((unsigned long long)seed);

    if (seed == 1) {
        cout << "10 9\n";
        return 0;
    }
    if (seed == 2) {
        cout << "23333 66666\n";
        return 0;
    }
    if (seed == 3) {
        cout << "1 1000000000\n";
        return 0;
    }
    if (seed == 4) {
        cout << "2 30\n";
        return 0;
    }

    long long hi = min<long long>(1000000000LL, n_max);
    long long a = uniform_int_distribution<long long>(1, hi)(rng);
    long long b = uniform_int_distribution<long long>(1, hi)(rng);
    cout << a << " " << b << "\n";
    return 0;
}

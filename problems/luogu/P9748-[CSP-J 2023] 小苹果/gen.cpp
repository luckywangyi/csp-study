// P9748 小苹果 — 数据生成器
// 用法: gen.exe <seed> <n_max>  （随机时 n 不超过 min(n_max, 20000) 以便暴力可跑）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    mt19937 rng((unsigned)seed);
    long long n_max = max(1LL, atoll(argv[2]));

    if (seed == 1) {
        cout << "1\n";
        return 0;
    }
    if (seed == 2) {
        cout << "8\n";
        return 0;
    }
    if (seed == 3) {
        cout << "2\n";
        return 0;
    }
    if (seed == 4) {
        cout << "3\n";
        return 0;
    }

    long long hi = min<long long>(n_max, 20000LL);
    uniform_int_distribution<long long> dist(1, hi);
    cout << dist(rng) << '\n';
    return 0;
}

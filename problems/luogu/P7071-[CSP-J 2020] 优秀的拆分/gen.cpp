// P7071 优秀的拆分 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    long long n_max = max(2LL, atoll(argv[2]));
    mt19937 rng((unsigned)seed + 2463534242u);

    if (seed == 1) {
        cout << "6\n";
        return 0;
    }
    if (seed == 2) {
        cout << "7\n";
        return 0;
    }
    if (seed == 3) {
        cout << "126\n";
        return 0;
    }
    if (seed == 4) {
        cout << "2\n";
        return 0;
    }

    long long n;
    if (uniform_int_distribution<int>(0, 1)(rng)) {
        n = uniform_int_distribution<long long>(2, min(n_max, 2000000LL))(rng);
        if (n % 2) ++n;
        if (n > n_max) n -= 2;
        if (n < 2) n = 2;
    } else {
        int max_e = 1;
        while (max_e < 60 && (1LL << (max_e + 1)) <= n_max) ++max_e;
        int e = uniform_int_distribution<int>(1, max_e)(rng);
        n = 1LL << e;
    }
    cout << n << '\n';
    return 0;
}

// P8814 解密 — 数据生成器
// 用法: gen.exe <seed> <n_max>   （n_max 控制询问数 k 的上界）
#include <bits/stdc++.h>
using namespace std;

using u64 = unsigned long long;
using u128 = __uint128_t;

mt19937_64 rng;

u64 rnd(u64 lo, u64 hi) {
    return uniform_int_distribution<u64>(lo, hi)(rng);
}

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    int n_max = max(1, atoi(argv[2]));
    rng.seed((u64)seed + 1469598103934665603ULL);

    if (seed == 1) {
        cout << "10\n"
                "770 77 5\n"
                "633 1 211\n"
                "545 1 499\n"
                "683 3 227\n"
                "858 3 257\n"
                "723 37 13\n"
                "572 26 11\n"
                "867 17 17\n"
                "829 3 263\n"
                "528 4 109\n";
        return 0;
    }
    if (seed == 2) {
        cout << "3\n"
                "15 2 4\n"
                "221 4 50\n"
                "143 1 120\n";
        return 0;
    }
    if (seed == 3) {
        cout << "2\n"
                "999999937 1 999999937\n"
                "1000000007 1 1000000007\n";
        return 0;
    }
    if (seed == 4) {
        cout << "4\n"
                "100 5 19\n"
                "100 3 33\n"
                "77 1 5\n"
                "8 1 1\n";
        return 0;
    }

    int k = uniform_int_distribution<int>(1, min(n_max, 20))(rng);
    cout << k << "\n";
    for (int i = 0; i < k; i++) {
        int mode = uniform_int_distribution<int>(0, 2)(rng);
        if (mode == 0) {
            u64 p = rnd(2, 5000);
            u64 q = rnd(2, 5000);
            if (p > q) swap(p, q);
            u128 n = (u128)p * q;
            u128 phi = (u128)(p - 1) * (q - 1);
            u64 e = 1;
            u64 d = (u64)(phi + 1);
            cout << (u64)n << " " << d << " " << e << "\n";
        } else if (mode == 1) {
            u64 n = rnd(4, 5000);
            u64 d = rnd(1, 1000);
            u64 e = rnd(1, 1000);
            cout << n << " " << d << " " << e << "\n";
        } else {
            u64 p = rnd(2, 500);
            u64 q = p;
            u128 n = (u128)p * q;
            u128 phi = (u128)(p - 1) * (q - 1);
            u64 e = 1;
            u64 d = (u64)(phi + 1);
            cout << (u64)n << " " << d << " " << e << "\n";
        }
    }
    return 0;
}

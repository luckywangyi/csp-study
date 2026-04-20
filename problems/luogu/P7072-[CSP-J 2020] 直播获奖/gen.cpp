// P7072 直播获奖 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    int n_max = max(1, atoi(argv[2]));
    mt19937 rng((unsigned)seed + 288252163u);

    if (seed == 1) {
        cout << "10 60\n200 300 400 500 600 600 0 300 200 100\n";
        return 0;
    }
    if (seed == 2) {
        cout << "10 30\n100 100 600 100 100 100 100 100 100 100\n";
        return 0;
    }
    if (seed == 3) {
        cout << "1 50\n0\n";
        return 0;
    }
    if (seed == 4) {
        cout << "5 99\n600 600 600 600 600\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(1, min(n_max, 2000))(rng);
    int w = uniform_int_distribution<int>(1, 99)(rng);
    cout << n << ' ' << w << '\n';
    for (int i = 0; i < n; ++i) {
        if (i) cout << ' ';
        cout << uniform_int_distribution<int>(0, 600)(rng);
    }
    cout << '\n';
    return 0;
}

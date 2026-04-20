// P5661 公交换乘 — 数据生成器
// 用法: gen.exe <seed> <n_max>  （n 不超过 n_max，且不超过 5000 以免暴力过慢）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = (argc >= 2) ? atoi(argv[1]) : 1;
    int n_max = (argc >= 3) ? max(1, atoi(argv[2])) : 20;
    n_max = min(n_max, 5000);
    mt19937 rng((unsigned)seed + 0x9e3779b9u);

    if (seed == 1) {
        cout << "6\n0 10 3\n1 5 46\n0 12 50\n1 3 96\n0 5 110\n1 6 135\n";
        return 0;
    }
    if (seed == 2) {
        cout << "6\n0 5 1\n0 20 16\n0 7 23\n1 18 31\n1 4 38\n1 7 68\n";
        return 0;
    }
    if (seed == 3) {
        // 边界：公交恰好在第 45 分钟仍可用票
        cout << "2\n0 10 0\n1 10 45\n";
        return 0;
    }
    if (seed == 4) {
        // 边界：公交晚 1 分钟，票失效
        cout << "2\n0 10 0\n1 10 46\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(1, n_max)(rng);
    int cur_t = 0;
    vector<array<int, 3>> rec;
    for (int i = 0; i < n; ++i) {
        int type = uniform_int_distribution<int>(0, 1)(rng);
        int price = uniform_int_distribution<int>(1, 1000)(rng);
        cur_t += uniform_int_distribution<int>(1, 50)(rng);
        rec.push_back({type, price, cur_t});
    }
    cout << n << '\n';
    for (auto& a : rec) cout << a[0] << ' ' << a[1] << ' ' << a[2] << '\n';
    return 0;
}

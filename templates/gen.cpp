/*
 * CSP 数据生成器模板
 * 用法: gen.exe <seed> <n_max> [其他参数...]
 *   seed   — 随机种子
 *   n_max  — 控制数据规模上界
 *
 * 编译: g++ -std=c++17 -O2 -o gen gen.cpp
 */
#include <bits/stdc++.h>
using namespace std;

mt19937 rng;

int randint(int lo, int hi) {
    return uniform_int_distribution<int>(lo, hi)(rng);
}

long long randll(long long lo, long long hi) {
    return uniform_int_distribution<long long>(lo, hi)(rng);
}

double randdbl(double lo, double hi) {
    return uniform_real_distribution<double>(lo, hi)(rng);
}

// 生成 cnt 个在 [lo, hi] 范围内互不相同的整数
vector<int> rand_distinct(int cnt, int lo, int hi) {
    assert(hi - lo + 1 >= cnt);
    set<int> used;
    vector<int> res;
    while ((int)res.size() < cnt) {
        int v = randint(lo, hi);
        if (used.insert(v).second) res.push_back(v);
    }
    return res;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: gen <seed> <n_max> [args...]\n";
        return 1;
    }
    rng.seed((unsigned)atoi(argv[1]));
    int n_max = atoi(argv[2]);

    // === 在此编写生成逻辑 ===
    int n = randint(1, n_max);
    cout << n << "\n";
    for (int i = 0; i < n; i++) {
        if (i) cout << " ";
        cout << randint(1, 10000);
    }
    cout << "\n";

    return 0;
}

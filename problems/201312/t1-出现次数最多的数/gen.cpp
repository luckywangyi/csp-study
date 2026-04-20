// 201312-T1 出现次数最多的数 — 数据生成器
// 用法: gen.exe <seed> <n_max>
// 数据范围: 1 <= n <= 1000, 每个数在 [1, 10000]
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    mt19937 rng((unsigned)seed);
    int n_max = atoi(argv[2]);

    // 边界用例：低 seed 时生成特殊数据
    if (seed == 1) {
        // n=1：只有一个数
        cout << "1\n42\n";
        return 0;
    }
    if (seed == 2) {
        // 全相同元素
        int n = min(10, n_max);
        cout << n << "\n";
        for (int i = 0; i < n; i++) { if (i) cout << ' '; cout << 7; }
        cout << "\n";
        return 0;
    }
    if (seed == 3) {
        // 多个并列最多，取最小
        cout << "6\n5 3 5 3 1 2\n";
        return 0;
    }
    if (seed == 4) {
        // 所有数都只出现一次，取最小
        int n = min(5, n_max);
        cout << n << "\n";
        for (int i = 0; i < n; i++) { if (i) cout << ' '; cout << n - i; }
        cout << "\n";
        return 0;
    }

    // 随机数据
    int n = uniform_int_distribution<int>(1, n_max)(rng);
    int hotVal = uniform_int_distribution<int>(1, 10000)(rng);
    int hotCount = uniform_int_distribution<int>(1, max(1, n / 3))(rng);
    cout << n << "\n";
    vector<int> a;
    for (int i = 0; i < hotCount && i < n; i++) a.push_back(hotVal);
    for (int i = (int)a.size(); i < n; i++)
        a.push_back(uniform_int_distribution<int>(1, 10000)(rng));
    shuffle(a.begin(), a.end(), rng);
    for (int i = 0; i < n; i++) {
        if (i) cout << ' ';
        cout << a[i];
    }
    cout << "\n";
}

// 201312-T1 出现次数最多的数 — 数据生成器
// 用法: gen.exe <seed> <n_max>
// 数据范围: 1 <= n <= 1000, 每个数在 [1, 10000]
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char* argv[]) {
    mt19937 rng((unsigned)atoi(argv[1]));
    int n_max = atoi(argv[2]);
    int n = uniform_int_distribution<int>(1, n_max)(rng);
    // 为确保有明确的"最多"，有时让某个数重复出现多次
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

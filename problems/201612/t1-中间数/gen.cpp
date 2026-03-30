// 201612-T1 中间数 — 数据生成器
// 用法: gen.exe <seed> <n_max>
// 数据范围: 1 <= n <= n_max
// 注意: 生成的数据需要覆盖"有中间数"和"无中间数"两种情况
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char* argv[]) {
    mt19937 rng((unsigned)atoi(argv[1]));
    int n_max = max(1, atoi(argv[2]));
    int n = uniform_int_distribution<int>(1, n_max)(rng);
    // seed 为偶数时尝试构造有中间数的情况
    bool forceMiddle = (atoi(argv[1]) % 2 == 0) && n >= 3;
    vector<int> a;
    if (forceMiddle) {
        int mid = uniform_int_distribution<int>(1, 1000)(rng);
        int half = (n - 1) / 2;
        a.push_back(mid);
        for (int i = 0; i < half; i++)
            a.push_back(uniform_int_distribution<int>(mid + 1, mid + 1000)(rng));
        for (int i = 0; i < half; i++)
            a.push_back(uniform_int_distribution<int>(max(1, mid - 1000), mid - 1)(rng));
        while ((int)a.size() < n)
            a.push_back(mid);
    } else {
        for (int i = 0; i < n; i++)
            a.push_back(uniform_int_distribution<int>(1, 1000)(rng));
    }
    shuffle(a.begin(), a.end(), rng);
    cout << n << "\n";
    for (int i = 0; i < n; i++) {
        if (i) cout << ' ';
        cout << a[i];
    }
    cout << "\n";
}

// 201612-T1 中间数 — 数据生成器
// 用法: gen.exe <seed> <n_max>
// 数据范围: 1 <= n <= n_max
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    mt19937 rng((unsigned)seed);
    int n_max = max(1, atoi(argv[2]));

    // 边界用例
    if (seed == 1) {
        // n=1：唯一元素，左右各 0 个 → 是中间数
        cout << "1\n5\n";
        return 0;
    }
    if (seed == 2) {
        // n=2：无论如何没有中间数（各自作为候选时 L≠R）
        cout << "2\n1 2\n";
        return 0;
    }
    if (seed == 3) {
        // n=3，明确有中间数
        cout << "3\n3 1 2\n";
        return 0;
    }
    if (seed == 4) {
        // 全相同元素：L=0, R=0 → 是中间数
        cout << "5\n3 3 3 3 3\n";
        return 0;
    }
    if (seed == 5) {
        // 偶数个不同元素，无中间数
        cout << "4\n1 2 3 4\n";
        return 0;
    }

    // 随机数据
    int n = uniform_int_distribution<int>(1, n_max)(rng);
    bool forceMiddle = (seed % 2 == 0) && n >= 3;
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

// P9753 消消乐 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    mt19937 rng((unsigned)seed);
    int n_max = max(1, atoi(argv[2]));

    if (seed == 1) {
        cout << "8\naccabccb\n";
        return 0;
    }
    if (seed == 2) {
        cout << "2\naa\n";
        return 0;
    }
    if (seed == 3) {
        cout << "1\na\n";
        return 0;
    }
    if (seed == 4) {
        cout << "4\nabab\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(1, min(n_max, 400))(rng);
    string alpha = "abcdefghijklmnopqrstuvwxyz";
    cout << n << '\n';
    for (int i = 0; i < n; i++) cout << alpha[uniform_int_distribution<int>(0, 25)(rng)];
    cout << '\n';
    return 0;
}

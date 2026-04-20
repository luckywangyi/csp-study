// P5660 数字游戏 — 数据生成器
// 用法: gen.exe <seed> <n_max>  （n_max 未使用；固定输出长度 8）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = (argc >= 2) ? atoi(argv[1]) : 1;
    (void)((argc >= 3) ? atoi(argv[2]) : 0);
    mt19937 rng((unsigned)seed + 0x9e3779b9u);

    if (seed == 1) {
        cout << "00000000\n";
        return 0;
    }
    if (seed == 2) {
        cout << "11111111\n";
        return 0;
    }
    if (seed == 3) {
        cout << "00010100\n";
        return 0;
    }
    if (seed == 4) {
        cout << "10101010\n";
        return 0;
    }

    string s;
    for (int i = 0; i < 8; ++i) s.push_back('0' + uniform_int_distribution<int>(0, 1)(rng));
    cout << s << '\n';
    return 0;
}

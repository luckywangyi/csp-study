// P7073 表达式 — 数据生成器
// 用法: gen.exe <seed> <n_max>   （n_max 控制变量个数上界）
#include <bits/stdc++.h>
using namespace std;

static string chain_postfix(int n, mt19937& rng) {
    string s = "x1";
    for (int i = 2; i <= n; ++i) {
        s.push_back(' ');
        s.push_back('x');
        s += to_string(i);
        s.push_back(' ');
        s.push_back(uniform_int_distribution<int>(0, 1)(rng) ? '&' : '|');
    }
    return s;
}

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    int n_max = max(2, atoi(argv[2]));
    mt19937 rng((unsigned)seed + 1000003);

    if (seed == 1) {
        cout << "x1 x2 & x3 |\n3\n1 0 1\n3\n1\n2\n3\n";
        return 0;
    }
    if (seed == 2) {
        cout << "x1 ! x2 x4 | x3 x5 ! & & ! &\n5\n0 1 0 1 1\n3\n1\n3\n5\n";
        return 0;
    }
    if (seed == 3) {
        cout << "x1 x2 &\n2\n0 0\n1\n1\n";
        return 0;
    }
    if (seed == 4) {
        cout << "x1 x2 |\n2\n0 0\n2\n1\n2\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(2, min(n_max, 40))(rng);
    string expr = chain_postfix(n, rng);
    int q = uniform_int_distribution<int>(1, min(50, n * 3))(rng);

    cout << expr << '\n';
    cout << n << '\n';
    for (int i = 1; i <= n; ++i) {
        if (i > 1) cout << ' ';
        cout << uniform_int_distribution<int>(0, 1)(rng);
    }
    cout << '\n' << q << '\n';
    for (int i = 0; i < q; ++i) {
        cout << uniform_int_distribution<int>(1, n)(rng) << '\n';
    }
    return 0;
}

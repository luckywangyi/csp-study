// P7913 廊桥分配 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    int n_max = max(1, atoi(argv[2]));
    mt19937 rng((unsigned)seed);

    if (seed == 1) {
        cout << "3 5 4\n"
                "1 5\n3 8\n6 10\n9 14\n13 18\n"
                "2 11\n4 15\n7 17\n12 16\n";
        return 0;
    }
    if (seed == 2) {
        cout << "2 4 6\n"
                "20 30\n40 50\n21 22\n41 42\n"
                "1 19\n2 18\n3 4\n5 6\n7 8\n9 10\n";
        return 0;
    }
    if (seed == 3) {
        cout << "1 1 1\n1 2\n2 3\n";
        return 0;
    }
    if (seed == 4) {
        cout << "5 3 3\n"
                "1 100\n2 99\n3 98\n"
                "50 60\n51 61\n52 62\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(1, min(n_max, 20))(rng);
    int m1 = uniform_int_distribution<int>(1, min(n_max, 15))(rng);
    int m2 = uniform_int_distribution<int>(1, min(n_max, 15))(rng);
    cout << n << " " << m1 << " " << m2 << "\n";
    set<pair<int, int>> used;
    auto genFlight = [&](mt19937& r) {
        while (true) {
            int a = uniform_int_distribution<int>(1, 500)(r);
            int len = uniform_int_distribution<int>(1, 50)(r);
            int b = a + len;
            if (!used.count({a, b})) {
                used.insert({a, b});
                return pair<int, int>{a, b};
            }
        }
    };
    for (int i = 0; i < m1; i++) {
        auto p = genFlight(rng);
        cout << p.first << " " << p.second << "\n";
    }
    for (int i = 0; i < m2; i++) {
        auto p = genFlight(rng);
        cout << p.first << " " << p.second << "\n";
    }
    return 0;
}

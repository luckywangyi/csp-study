// P9754 结构体 — 数据生成器
// 用法: gen.exe <seed> <n_max>  （n 为操作数）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    mt19937 rng((unsigned)seed);
    int n_max = max(1, atoi(argv[2]));

    if (seed == 1) {
        cout << "5\n";
        cout << "1 a 2\nshort aa\nint ab\n";
        cout << "1 b 2\na ba\nlong bb\n";
        cout << "2 b x\n";
        cout << "3 x.ba.ab\n";
        cout << "4 10\n";
        return 0;
    }
    if (seed == 2) {
        cout << "3\n";
        cout << "1 s1 1\nlong a\n";
        cout << "2 long e1\n";
        cout << "3 e1.a\n";
        return 0;
    }
    if (seed == 3) {
        cout << "4\n";
        cout << "1 t1 2\nbyte b1\nbyte b2\n";
        cout << "2 t1 u\n";
        cout << "4 0\n";
        cout << "3 u.b1\n";
        return 0;
    }
    if (seed == 4) {
        cout << "6\n";
        cout << "1 inner 2\nint x\nshort y\n";
        cout << "1 outer 2\ninner i\nbyte pad\n";
        cout << "2 outer root\n";
        cout << "3 root.i.x\n";
        cout << "4 0\n";
        cout << "4 100\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(1, min(n_max, 20))(rng);
    cout << n << '\n';
    int name_ctr = 0;
    auto new_id = [&](const char* pfx) {
        return string(pfx) + to_string(++name_ctr);
    };

    vector<string> structs;

    for (int i = 0; i < n; i++) {
        bool can_struct = (int)structs.size() < 6;
        bool can_elem = !structs.empty();
        int op;
        if (!can_elem)
            op = 1;
        else if (!can_struct)
            op = 2;
        else
            op = uniform_int_distribution<int>(1, 2)(rng);

        if (op == 1) {
            string s = new_id("t");
            int k = uniform_int_distribution<int>(1, 3)(rng);
            cout << "1 " << s << ' ' << k << '\n';
            for (int j = 0; j < k; j++) {
                string ty;
                int r = uniform_int_distribution<int>(0, 5)(rng);
                if (r < 4 || structs.empty())
                    ty = vector<string>{"byte", "short", "int", "long"}[r < 4 ? r : r % 4];
                else
                    ty = structs[uniform_int_distribution<int>(
                        0, (int)structs.size() - 1)(rng)];
                string mn = new_id("m");
                cout << ty << ' ' << mn << '\n';
            }
            structs.push_back(s);
        } else {
            string ty = structs[uniform_int_distribution<int>(
                0, (int)structs.size() - 1)(rng)];
            string en = new_id("e");
            cout << "2 " << ty << ' ' << en << '\n';
        }
    }
    return 0;
}

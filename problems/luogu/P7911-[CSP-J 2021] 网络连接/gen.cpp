// P7911 网络连接 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = atoi(argv[1]);
    int n_max = max(1, atoi(argv[2]));
    mt19937 rng((unsigned)seed);

    if (seed == 1) {
        cout << "5\n"
                "Server 192.168.1.1:8080\n"
                "Server 192.168.1.1:8080\n"
                "Client 192.168.1.1:8080\n"
                "Client 192.168.1.1:80\n"
                "Client 192.168.1.1:99999\n";
        return 0;
    }
    if (seed == 2) {
        cout << "4\n"
                "Server 0.0.0.0:0\n"
                "Client 0.0.0.0:0\n"
                "Server 255.255.255.255:65535\n"
                "Client 255.255.255.255:65535\n";
        return 0;
    }
    if (seed == 3) {
        cout << "6\n"
                "Server 192.168.00.1:80\n"
                "Client 192.168.0.1:80\n"
                "Server 192.168.0.1:088\n"
                "Server 192:168:0:1.233\n"
                "Client 192.168.0.1\n"
                "Client 192.168.0.1:80\n";
        return 0;
    }
    if (seed == 4) {
        cout << "3\n"
                "Server 10.0.0.1:1\n"
                "Server 10.0.0.2:2\n"
                "Client 10.0.0.1:1\n";
        return 0;
    }

    int n = uniform_int_distribution<int>(1, min(n_max, 30))(rng);
    cout << n << "\n";
    for (int i = 0; i < n; i++) {
        string op = uniform_int_distribution<int>(0, 1)(rng) ? "Server" : "Client";
        int a = uniform_int_distribution<int>(0, 255)(rng);
        int b = uniform_int_distribution<int>(0, 255)(rng);
        int c = uniform_int_distribution<int>(0, 255)(rng);
        int d = uniform_int_distribution<int>(0, 255)(rng);
        int e = uniform_int_distribution<int>(0, 65535)(rng);
        auto fmt = [](int x) {
            return to_string(x);
        };
        string ad = fmt(a) + "." + fmt(b) + "." + fmt(c) + "." + fmt(d) + ":" + fmt(e);
        int t = uniform_int_distribution<int>(0, 7)(rng);
        if (t == 0 && a != 0) {
            ad = string(1, '0') + to_string(a) + "." + fmt(b) + "." + fmt(c) + "." + fmt(d) + ":" + fmt(e);
        } else if (t == 1 && e != 0) {
            ad = fmt(a) + "." + fmt(b) + "." + fmt(c) + "." + fmt(d) + ":" + string("0") + fmt(e);
        } else if (t == 2) {
            ad = fmt(a) + "." + fmt(b) + "." + fmt(c) + "." + fmt(d) + fmt(d) + ":" + fmt(e);
        }
        cout << op << " " << ad << "\n";
    }
    return 0;
}

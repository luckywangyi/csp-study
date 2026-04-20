// P8818 策略游戏 — 数据生成器
// 用法: gen.exe <seed> <n_max>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    mt19937 rng((unsigned)seed);
    int n_max = max(1, atoi(argv[2]));

    auto print_case = [&](int n, int m, int q, const vector<long long>& A,
                          const vector<long long>& B,
                          const vector<array<int, 4>>& qs) {
        cout << n << ' ' << m << ' ' << q << '\n';
        for (int i = 1; i <= n; i++) {
            if (i > 1) cout << ' ';
            cout << A[i];
        }
        cout << '\n';
        for (int j = 1; j <= m; j++) {
            if (j > 1) cout << ' ';
            cout << B[j];
        }
        cout << '\n';
        for (auto& t : qs) {
            cout << t[0] << ' ' << t[1] << ' ' << t[2] << ' ' << t[3] << '\n';
        }
    };

    if (seed == 1) {
        vector<long long> A(4), B(3);
        A[1] = 0;
        A[2] = 1;
        A[3] = -2;
        B[1] = -3;
        B[2] = 4;
        vector<array<int, 4>> qs = {{1, 3, 1, 2}, {2, 3, 2, 2}};
        print_case(3, 2, 2, A, B, qs);
        return 0;
    }
    if (seed == 2) {
        int n = 1, m = 1, q = 1;
        vector<long long> A(n + 1), B(m + 1);
        A[1] = 5;
        B[1] = -7;
        vector<array<int, 4>> qs = {{1, 1, 1, 1}};
        print_case(n, m, q, A, B, qs);
        return 0;
    }
    if (seed == 3) {
        int n = 2, m = 2, q = 2;
        vector<long long> A(n + 1), B(m + 1);
        A[1] = -1000000000LL;
        A[2] = 1000000000LL;
        B[1] = 1000000000LL;
        B[2] = -1000000000LL;
        vector<array<int, 4>> qs = {{1, 2, 1, 2}, {2, 2, 1, 1}};
        print_case(n, m, q, A, B, qs);
        return 0;
    }
    if (seed == 4) {
        int n = 3, m = 3, q = 3;
        vector<long long> A(n + 1), B(m + 1);
        A[1] = 0;
        A[2] = 0;
        A[3] = 1;
        B[1] = -1;
        B[2] = 0;
        B[3] = 2;
        vector<array<int, 4>> qs = {{1, 3, 1, 3}, {1, 1, 1, 3}, {3, 3, 2, 2}};
        print_case(n, m, q, A, B, qs);
        return 0;
    }

    int n = uniform_int_distribution<int>(1, n_max)(rng);
    int m = uniform_int_distribution<int>(1, n_max)(rng);
    int q = uniform_int_distribution<int>(1, min(n_max, 50))(rng);
    uniform_int_distribution<long long> dv(-(long long)1e6, (long long)1e6);
    vector<long long> A(n + 1), B(m + 1);
    for (int i = 1; i <= n; i++) A[i] = dv(rng);
    for (int j = 1; j <= m; j++) B[j] = dv(rng);
    vector<array<int, 4>> qs;
    for (int i = 0; i < q; i++) {
        int l1 = uniform_int_distribution<int>(1, n)(rng);
        int r1 = uniform_int_distribution<int>(l1, n)(rng);
        int l2 = uniform_int_distribution<int>(1, m)(rng);
        int r2 = uniform_int_distribution<int>(l2, m)(rng);
        qs.push_back({l1, r1, l2, r2});
    }
    print_case(n, m, q, A, B, qs);
    return 0;
}

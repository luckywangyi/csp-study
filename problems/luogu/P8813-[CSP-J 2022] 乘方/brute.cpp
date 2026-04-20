// P8813 乘方 — 暴力：二进制快速幂配合溢出判断
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long a, b;
    cin >> a >> b;
    const long long LIM = 1000000000LL;
    if (a == 1) {
        cout << 1 << "\n";
        return 0;
    }
    __int128 res = 1;
    __int128 base = a;
    long long e = b;
    while (e > 0) {
        if (e & 1) {
            res *= base;
            if (res > (__int128)LIM) {
                cout << -1 << "\n";
                return 0;
            }
        }
        e >>= 1;
        if (e) {
            if (base > (__int128)LIM) {
                base = (__int128)LIM + 1;
            } else {
                base = base * base;
            }
        }
    }
    if (res > (__int128)LIM)
        cout << -1 << "\n";
    else
        cout << (long long)res << "\n";
    return 0;
}

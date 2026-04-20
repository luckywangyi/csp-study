// P8814 解密 — 暴力：m = n - d*e + 2 = p+q，整数根判别式并验证 p*q=n
#include <bits/stdc++.h>
using namespace std;

using i128 = __int128_t;

bool isqrt(i128 x, i128& r) {
    if (x < 0) return false;
    i128 lo = 0, hi = (i128)4e18;
    r = 0;
    while (lo <= hi) {
        i128 mid = (lo + hi) / 2;
        i128 sq = mid * mid;
        if (sq <= x) {
            r = mid;
            lo = mid + 1;
        } else
            hi = mid - 1;
    }
    return r * r == x;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int k;
    cin >> k;
    while (k--) {
        unsigned long long n0, d0, e0;
        cin >> n0 >> d0 >> e0;
        i128 n = n0;
        i128 d = d0;
        i128 e = e0;
        i128 m = n - d * e + 2;
        i128 disc = m * m - 4 * n;
        i128 s;
        if (!isqrt(disc, s)) {
            cout << "NO\n";
            continue;
        }
        if ((m - s) % 2 != 0 || (m + s) % 2 != 0) {
            cout << "NO\n";
            continue;
        }
        i128 p = (m - s) / 2;
        i128 q = (m + s) / 2;
        if (p <= 0 || q <= 0 || p * q != n) {
            cout << "NO\n";
            continue;
        }
        if (p > q) swap(p, q);
        cout << (unsigned long long)p << " " << (unsigned long long)q << "\n";
    }
    return 0;
}

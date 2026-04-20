// P7909 分糖果 — 暴力：max_{L<=k<=R} (k mod n)
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n, L, R;
    cin >> n >> L >> R;
    long long ans;
    if (R - L + 1 >= n) {
        ans = n - 1;
    } else {
        long long t = L + (n - 1 - (L % n));
        if (t <= R)
            ans = n - 1;
        else
            ans = max(L % n, R % n);
    }
    cout << ans << '\n';
    return 0;
}

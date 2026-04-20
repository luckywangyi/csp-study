#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m, q;
    cin >> n >> m >> q;
    vector<long long> A(n + 1), B(m + 1);
    for (int i = 1; i <= n; i++) cin >> A[i];
    for (int i = 1; i <= m; i++) cin >> B[i];
    while (q--) {
        int l1, r1, l2, r2;
        cin >> l1 >> r1 >> l2 >> r2;
        __int128 best;
        bool first = true;
        for (int x = l1; x <= r1; x++) {
            __int128 mn = (__int128)1 << 120;
            for (int y = l2; y <= r2; y++) {
                __int128 p = (__int128)A[x] * B[y];
                mn = min(mn, p);
            }
            if (first) {
                best = mn;
                first = false;
            } else {
                best = max(best, mn);
            }
        }
        long long out = (long long)best;
        cout << out << '\n';
    }
    return 0;
}

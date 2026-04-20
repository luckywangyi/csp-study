#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        vector<long long> A(n + 1);
        for (int i = 1; i <= n; i++) cin >> A[i];
        vector<long long> s(n + 1, 0);
        for (int i = 2; i <= n; i++) {
            s[i] = s[i - 1];
            if (A[i] == A[i - 1]) s[i] += A[i];
        }
        unordered_map<long long, int> prev;
        vector<int> last(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            auto it = prev.find(A[i]);
            last[i] = (it == prev.end() ? 0 : it->second);
            prev[A[i]] = i;
        }
        vector<long long> dp0(n + 1, 0), dp1(n + 1, 0);
        for (int i = 1; i <= n; i++) {
            long long base = max(dp0[i - 1], dp1[i - 1]);
            dp0[i] = dp1[i] = base;
            if (i >= 2 && A[i] == A[i - 1]) {
                dp0[i] = max(dp0[i], dp0[i - 1] + A[i]);
                dp1[i] = max(dp1[i], dp1[i - 1] + A[i]);
            }
            if (i >= 2 && A[i] != A[i - 1]) {
                int l = last[i];
                if (l > 0) {
                    dp0[i] = max(dp0[i], A[i] + dp1[l + 1] + s[i - 1] - s[l + 1]);
                    dp1[i] = max(dp1[i], A[i] + dp0[l + 1] + s[i - 1] - s[l + 1]);
                }
            }
        }
        cout << max(dp0[n], dp1[n]) << "\n";
    }
    return 0;
}

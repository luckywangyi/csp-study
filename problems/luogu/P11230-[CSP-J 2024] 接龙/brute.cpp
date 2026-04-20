#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        int n, k, q;
        cin >> n >> k >> q;
        vector<vector<int>> S(n);
        for (int i = 0; i < n; i++) {
            int l;
            cin >> l;
            S[i].resize(l);
            for (int j = 0; j < l; j++) cin >> S[i][j];
        }
        while (q--) {
            int r, c;
            cin >> r >> c;
            map<tuple<int, int, int>, bool> memo;
            function<bool(int, int, int)> dfs = [&](int lastPl, int lastVal, int t) -> bool {
                if (t == r) return lastVal == c;
                auto key = make_tuple(lastPl, lastVal, t);
                auto it = memo.find(key);
                if (it != memo.end()) return it->second;
                for (int p = 0; p < n; p++) {
                    if (p == lastPl) continue;
                    const auto& seq = S[p];
                    for (int st = 0; st < (int)seq.size(); st++) {
                        if (seq[st] != lastVal) continue;
                        for (int en = st + 1; en < (int)seq.size(); en++) {
                            int len = en - st + 1;
                            if (len < 2 || len > k) continue;
                            int nv = seq[en];
                            if (dfs(p, nv, t + 1)) return memo[key] = true;
                        }
                    }
                }
                return memo[key] = false;
            };

            bool ok = false;
            for (int p = 0; p < n; p++) {
                const auto& seq = S[p];
                for (int st = 0; st < (int)seq.size(); st++) {
                    if (seq[st] != 1) continue;
                    for (int en = st + 1; en < (int)seq.size(); en++) {
                        int len = en - st + 1;
                        if (len < 2 || len > k) continue;
                        int nv = seq[en];
                        if (dfs(p, nv, 1)) ok = true;
                        if (ok) break;
                    }
                    if (ok) break;
                }
                if (ok) break;
            }
            cout << (ok ? 1 : 0) << "\n";
        }
    }
    return 0;
}

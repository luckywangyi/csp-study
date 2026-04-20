#include <bits/stdc++.h>
using namespace std;

using ll = long long;

int n, m, K;
vector<int> aprime;
vector<int> cq;
vector<vector<int>> dd;

static int simk(const vector<int>& a, int k) {
    vector<int> cur(1 << k);
    iota(cur.begin(), cur.end(), 1);
    for (int r = 1; r <= k; r++) {
        vector<int> nxt;
        int half = (int)cur.size() / 2;
        for (int g = 0; g < half; g++) {
            int u = cur[2 * g], v = cur[2 * g + 1];
            int bit = dd[r][g];
            int host = bit == 0 ? min(u, v) : max(u, v);
            int guest = u + v - host;
            int win = ((ll)a[host - 1] >= r) ? host : guest;
            nxt.push_back(win);
        }
        cur.swap(nxt);
    }
    return cur[0];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> m;
    aprime.resize(n);
    for (int i = 0; i < n; i++) cin >> aprime[i];
    cq.assign(m + 1, 0);
    for (int i = 1; i <= m; i++) cin >> cq[i];
    K = 0;
    while ((1 << K) < n) K++;
    dd.assign(K + 1, vector<int>());
    for (int r = 1; r <= K; r++) {
        string s;
        cin >> s;
        dd[r].resize((int)s.size());
        for (int i = 0; i < (int)s.size(); i++) dd[r][i] = s[i] - '0';
    }
    int T;
    cin >> T;

    while (T--) {
        array<int, 4> X{};
        for (int i = 0; i < 4; i++) cin >> X[i];
        vector<int> a0(n);
        for (int i = 0; i < n; i++) a0[i] = aprime[i] ^ X[(i + 1) % 4];

        ll out = 0;
        for (int qi = 1; qi <= m; qi++) {
            int c = cq[qi];
            int k = 0;
            while ((1 << k) < c) k++;
            int M = 1 << k;
            vector<int> freepos;
            for (int i = c; i < M; i++) freepos.push_back(i);
            int f = (int)freepos.size();
            // 胜负只与「a 擂主 >= 轮次 R」有关，R∈[1,k]。对整数 a，与所有 R 比较的结果由 a 落在 {0..k-1} 或 ≥k 决定，枚举 0..k 即可代表全部等价类。
            vector<int> vals;
            for (int v = 0; v <= k; v++) vals.push_back(v);
            int Cn = (int)vals.size();

            long long est = 1;
            for (int z = 0; z < f; z++) {
                est *= Cn;
                if (est > 8000000LL) break;
            }
            if (est > 8000000LL) {
                cerr << "arena brute: (k+2)^free too large (c=" << c << ", k=" << k << ", free=" << f
                     << "). Reduce n_max / keep asks near n in gen.\n";
                return 1;
            }

            set<int> wins;
            vector<int> cur(M);
            for (int i = 0; i < c; i++) cur[i] = a0[i];
            function<void(int)> dfs = [&](int pos) {
                if (pos == f) {
                    wins.insert(simk(cur, k));
                    return;
                }
                for (int t = 0; t < Cn; t++) {
                    cur[freepos[pos]] = vals[t];
                    dfs(pos + 1);
                }
            };
            dfs(0);

            ll sum = 0;
            for (int id : wins) sum += id;
            out ^= (ll)qi * sum;
        }
        cout << out << "\n";
    }
    return 0;
}

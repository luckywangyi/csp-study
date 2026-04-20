#include <bits/stdc++.h>
using namespace std;

static long double end_pos(long long L, long long d, long long v0, long long a) {
    if (a == 0) return (long double)L;
    if (a < 0) {
        long double smax = -(long double)v0 * v0 / (2.0L * (long double)a);
        long double pos = (long double)d + smax;
        if (pos < (long double)L) return pos;
    }
    return (long double)L;
}

static long double v_at(long long v0, long long a, long long d, long long p) {
    long double s = (long double)p - (long double)d;
    return sqrtl(max((long double)0, (long double)v0 * v0 + 2.0L * (long double)a * s));
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    const long double EPS = 1e-12L;
    while (T--) {
        int n, m;
        long long L;
        int V;
        cin >> n >> m >> L >> V;
        vector<long long> d(n), v(n), a(n);
        for (int i = 0; i < n; i++) cin >> d[i] >> v[i] >> a[i];
        vector<long long> p(m);
        for (int j = 0; j < m; j++) cin >> p[j];

        vector<char> speed_car(n, 0);
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            long double e = end_pos(L, d[i], v[i], a[i]);
            bool sp = false;
            for (int j = 0; j < m; j++) {
                if ((long double)p[j] + EPS < (long double)d[i]) continue;
                if ((long double)p[j] > e + EPS) continue;
                long double vv = v_at(v[i], a[i], d[i], p[j]);
                if (vv > (long double)V + EPS) sp = true;
            }
            if (sp) {
                speed_car[i] = 1;
                cnt++;
            }
        }

        vector<pair<long long, long long>> iv;
        for (int i = 0; i < n; i++) {
            if (!speed_car[i]) continue;
            long double e = end_pos(L, d[i], v[i], a[i]);
            long long Lp = (long long)4e18, Rp = -(long long)4e18;
            bool any = false;
            for (int j = 0; j < m; j++) {
                if ((long double)p[j] + EPS < (long double)d[i]) continue;
                if ((long double)p[j] > e + EPS) continue;
                long double vv = v_at(v[i], a[i], d[i], p[j]);
                if (vv > (long double)V + EPS) {
                    any = true;
                    Lp = min(Lp, p[j]);
                    Rp = max(Rp, p[j]);
                }
            }
            if (any) iv.push_back({Lp, Rp});
        }

        sort(iv.begin(), iv.end(), [](const auto& x, const auto& y) {
            if (x.second != y.second) return x.second < y.second;
            return x.first < y.first;
        });
        int keep = 0;
        long long last = -(long long)4e18;
        for (auto& e : iv) {
            if (last < e.first) {
                keep++;
                last = e.second;
            }
        }
        cout << cnt << " " << (m - keep) << "\n";
    }
    return 0;
}

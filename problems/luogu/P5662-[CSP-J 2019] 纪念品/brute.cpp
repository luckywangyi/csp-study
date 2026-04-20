// P5662 纪念品 — 暴力/标程：相邻两天完全背包赚差价（与常见 AC 一致）
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T, N, M;
    if (!(cin >> T >> N >> M)) return 0;
    vector<vector<int>> P(T, vector<int>(N));
    for (int i = 0; i < T; ++i)
        for (int j = 0; j < N; ++j) cin >> P[i][j];

    if (T == 1) {
        cout << M << '\n';
        return 0;
    }

    vector<int> f(10005, 0);
    for (int day = 0; day < T - 1; ++day) {
        fill(f.begin(), f.end(), 0);
        for (int j = 0; j < N; ++j) {
            if (P[day + 1][j] > P[day][j]) {
                int c = P[day][j];
                int g = P[day + 1][j] - P[day][j];
                for (int k = c; k <= M; ++k) f[k] = max(f[k], f[k - c] + g);
            }
        }
        M += f[M];
    }

    cout << M << '\n';
    return 0;
}

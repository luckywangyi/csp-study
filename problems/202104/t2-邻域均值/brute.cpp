// 202104-T2 邻域均值 — 暴力标程
// O(n^2 * r^2) 暴力：对每个格子遍历其邻域求和
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, L, r, t;
    cin >> n >> L >> r >> t;

    vector<vector<int>> mat(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> mat[i][j];

    int ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            long long sum = 0;
            int cnt = 0;
            int x1 = max(0, i - r), x2 = min(n - 1, i + r);
            int y1 = max(0, j - r), y2 = min(n - 1, j + r);
            for (int x = x1; x <= x2; x++) {
                for (int y = y1; y <= y2; y++) {
                    sum += mat[x][y];
                    cnt++;
                }
            }
            if (sum <= (long long)t * cnt) ans++;
        }
    }
    cout << ans << "\n";
    return 0;
}

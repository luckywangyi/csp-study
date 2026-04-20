// 201503-T1 图像旋转 — 暴力标程
// 顺时针 90°：新矩阵 res[j][n-1-i] = mat[i][j]
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> mat(n, vector<int>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> mat[i][j];

    // 旋转后为 m 行 n 列
    vector<vector<int>> res(m, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            res[j][n - 1 - i] = mat[i][j];

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (j) cout << ' ';
            cout << res[i][j];
        }
        cout << "\n";
    }
    return 0;
}

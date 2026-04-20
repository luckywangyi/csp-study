#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        int n, m, k;
        cin >> n >> m >> k;
        int x, y, d;
        cin >> x >> y >> d;
        vector<string> g(n + 1);
        for (int i = 1; i <= n; i++) {
            cin >> g[i];
            g[i] = " " + g[i];
        }
        set<pair<int, int>> vis;
        vis.insert({x, y});
        for (int step = 0; step < k; step++) {
            int nx = x, ny = y;
            if (d == 0) ny = y + 1;
            else if (d == 1) nx = x + 1;
            else if (d == 2) ny = y - 1;
            else nx = x - 1;
            if (nx >= 1 && nx <= n && ny >= 1 && ny <= m && g[nx][ny] == '.') {
                x = nx;
                y = ny;
                vis.insert({x, y});
            } else {
                d = (d + 1) % 4;
            }
        }
        cout << (int)vis.size() << "\n";
    }
    return 0;
}

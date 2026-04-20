// P7913 廊桥分配 — 暴力：枚举国内廊桥数，按全局到达时间模拟先到先得
#include <bits/stdc++.h>
using namespace std;

struct Flight {
    int a, b, ty;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m1, m2;
    cin >> n >> m1 >> m2;
    vector<Flight> fs;
    fs.reserve(m1 + m2);
    for (int i = 0; i < m1; i++) {
        int a, b;
        cin >> a >> b;
        fs.push_back({a, b, 0});
    }
    for (int i = 0; i < m2; i++) {
        int a, b;
        cin >> a >> b;
        fs.push_back({a, b, 1});
    }
    sort(fs.begin(), fs.end(), [](const Flight& x, const Flight& y) { return x.a < y.a; });

    int best = 0;
    for (int nd = 0; nd <= n; nd++) {
        int ni = n - nd;
        multiset<int> dom, inter;
        int cnt = 0;
        for (const auto& f : fs) {
            multiset<int>& ms = f.ty == 0 ? dom : inter;
            int cap = f.ty == 0 ? nd : ni;
            while (!ms.empty() && *ms.begin() < f.a) ms.erase(ms.begin());
            if ((int)ms.size() < cap) {
                ms.insert(f.b);
                cnt++;
            }
        }
        best = max(best, cnt);
    }
    cout << best << "\n";
    return 0;
}

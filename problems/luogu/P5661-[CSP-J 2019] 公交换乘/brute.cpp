// P5661 公交换乘 — 暴力：按时间顺序模拟，公交时 O(k) 扫描最早可用优惠票
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    long long cost = 0;
    // 地铁票：获得时间、地铁票价（决定可抵扣的公交票价上限）
    vector<pair<int, int>> tickets;  // (time, max_bus_price)

    for (int i = 0; i < n; ++i) {
        int type, price, t;
        cin >> type >> price >> t;
        if (type == 0) {
            cost += price;
            tickets.emplace_back(t, price);
        } else {
            int used = -1;
            for (int j = 0; j < (int)tickets.size(); ++j) {
                int ts = tickets[j].first, mx = tickets[j].second;
                if (t - ts <= 45 && price <= mx) {
                    used = j;
                    break;
                }
            }
            if (used >= 0) {
                tickets.erase(tickets.begin() + used);
            } else {
                cost += price;
            }
        }
    }

    cout << cost << '\n';
    return 0;
}

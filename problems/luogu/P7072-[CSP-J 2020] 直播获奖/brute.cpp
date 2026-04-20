// P7072 直播获奖 — 暴力：每次重排成绩，按题意算分数线
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, w;
    cin >> n >> w;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    vector<int> cur;
    cur.reserve(n);
    for (int p = 1; p <= n; ++p) {
        cur.push_back(a[p - 1]);
        vector<int> desc = cur;
        sort(desc.begin(), desc.end(), greater<int>());
        long long plan = max(1LL, (1LL * p * w) / 100);
        int thr = desc[(int)plan - 1];
        cout << thr << (p == n ? '\n' : ' ');
    }
    return 0;
}

// P7910 插入排序 — 暴力：每次询问复制数组，按题面伪代码做插入排序，用 (值, 原下标) 区分相等元素
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, Q;
    cin >> n >> Q;
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    while (Q--) {
        int op;
        cin >> op;
        if (op == 1) {
            int x;
            long long v;
            cin >> x >> v;
            a[x] = v;
        } else {
            int x;
            cin >> x;
            vector<pair<long long, int>> b(n);
            for (int i = 0; i < n; i++) b[i] = {a[i + 1], i + 1};
            for (int i = 0; i < n; i++)
                for (int j = i; j >= 1; j--)
                    if (b[j].first < b[j - 1].first) swap(b[j], b[j - 1]);
            int pos = 1;
            for (int i = 0; i < n; i++)
                if (b[i].second == x) {
                    pos = i + 1;
                    break;
                }
            cout << pos << "\n";
        }
    }
    return 0;
}

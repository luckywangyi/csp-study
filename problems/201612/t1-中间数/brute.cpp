// 201612-T1 中间数 — 暴力标程
// 排序后逐个检查每个不同值，统计严格小于和严格大于的个数
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    sort(a.begin(), a.end());

    for (int i = 0; i < n; ) {
        int val = a[i];
        int j = i;
        while (j < n && a[j] == val) j++;
        int less = i;
        int greater = n - j;
        if (less == greater) {
            cout << val << "\n";
            return 0;
        }
        i = j;
    }
    cout << -1 << "\n";
    return 0;
}

// 201604-T1 折点计数 — 暴力标程
// 遍历中间元素，判断是否为峰或谷
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    int count = 0;
    for (int i = 1; i < n - 1; i++) {
        if ((a[i] > a[i - 1] && a[i] > a[i + 1]) ||
            (a[i] < a[i - 1] && a[i] < a[i + 1])) {
            count++;
        }
    }
    cout << count << "\n";
    return 0;
}

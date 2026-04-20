// P5657 格雷码 — 暴力/标程（按题意算法等价于 k ^ (k>>1)）
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    unsigned long long k;
    cin >> n >> k;
    unsigned long long g = k ^ (k >> 1);
    for (int i = n - 1; i >= 0; --i) {
        cout << char('0' + ((g >> i) & 1ULL));
    }
    cout << '\n';
    return 0;
}

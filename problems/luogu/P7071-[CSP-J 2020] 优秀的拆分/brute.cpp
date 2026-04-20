// P7071 优秀的拆分 — 暴力：检查二进制位（等价于枚举 2 的正整数次幂子集）
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n;
    cin >> n;
    if (n % 2 == 1) {
        cout << -1 << '\n';
        return 0;
    }
    vector<long long> parts;
    for (int b = 60; b >= 1; --b) {
        long long pw = 1LL << b;
        if (n & pw) parts.push_back(pw);
    }
    for (size_t i = 0; i < parts.size(); ++i) {
        if (i) cout << ' ';
        cout << parts[i];
    }
    cout << '\n';
    return 0;
}

/*
 * CCF CSP 标准解题模板：头文件、IO 优化、常用别名与常量。
 * 编译：g++ -std=c++17 -O2 solution.cpp
 */
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

constexpr ll INF = 4e18;
constexpr int MOD = 1e9 + 7;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 多测时取消下一行注释
    // int T; cin >> T; while (T--) {

    // 解题代码

    // }
    int n, L, R;
    cin >> n >> L >> R;
    int left = L % n;
    int left2 = R % n;
    if ((R - L) >=n) {
        cout << (n - 1);
        return 0;
    } else {
        cout << max(left, left2);
    }

    return 0;
}

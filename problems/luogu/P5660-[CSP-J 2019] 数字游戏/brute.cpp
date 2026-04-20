// P5660 数字游戏 — 暴力：统计长度为 8 的 01 串中字符 1 的个数
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string s;
    if (!(cin >> s)) return 0;
    int c = 0;
    for (char ch : s)
        if (ch == '1') ++c;
    cout << c << '\n';
    return 0;
}

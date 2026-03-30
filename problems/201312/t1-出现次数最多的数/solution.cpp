#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    map<int, int> cnt;
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        ++cnt[x];
    }
    int bestVal = 0, bestC = -1;
    for (const auto& [v, c] : cnt) {
        if (c > bestC) {
            bestC = c;
            bestVal = v;
        }
    }
    cout << bestVal << '\n';
    return 0;
}

// 201312-T1 出现次数最多的数 — 暴力标程
// 用 map 统计频次，遍历找最大频次中最小值
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    map<int, int> freq;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        freq[x]++;
    }
    int maxFreq = 0, ans = 0;
    for (auto& [val, cnt] : freq) {
        if (cnt > maxFreq) {
            maxFreq = cnt;
            ans = val;
        }
    }
    cout << ans << "\n";
    return 0;
}

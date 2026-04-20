#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    map<string, int> cnt;
    for (int i = 0; i < n; i++) {
        string s;
        cin >> s;
        cnt[s]++;
    }
    const vector<string> suits = {"D", "C", "H", "S"};
    const vector<string> ranks = {"A", "2", "3", "4", "5", "6", "7",
                                    "8", "9", "T", "J", "Q", "K"};
    int have = 0;
    for (const string& su : suits) {
        for (const string& rk : ranks) {
            string card = su + rk;
            if (cnt[card] >= 1) have++;
        }
    }
    cout << 52 - have << '\n';
    return 0;
}

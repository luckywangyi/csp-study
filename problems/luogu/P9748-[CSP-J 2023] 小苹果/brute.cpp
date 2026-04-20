#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n;
    cin >> n;
    vector<int> cur(n);
    iota(cur.begin(), cur.end(), 1);
    int day = 0;
    int day_n = -1;
    while (!cur.empty()) {
        day++;
        vector<int> nxt;
        for (size_t i = 0; i < cur.size(); i++) {
            if (i % 3 == 0) {
                if (cur[i] == (int)n) day_n = day;
            } else {
                nxt.push_back(cur[i]);
            }
        }
        cur.swap(nxt);
    }
    cout << day << ' ' << day_n << '\n';
    return 0;
}

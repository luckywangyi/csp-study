#include <bits/stdc++.h>
using namespace std;

static const int cost[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};

static bool can_finish(int rem_sticks, int rem_digits) {
    if (rem_digits < 0) return false;
    if (rem_digits == 0) return rem_sticks == 0;
    return rem_sticks >= 2 * rem_digits && rem_sticks <= 7 * rem_digits;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        if (n < 2) {
            cout << "-1\n";
            continue;
        }
        string ans = "-1";
        for (int d = 1; d <= n; d++) {
            if (n < 2 * d || n > 7 * d) continue;
            string cur;
            int rem = n;
            bool ok = true;
            for (int pos = 0; pos < d; pos++) {
                bool placed = false;
                for (int dig = (pos == 0 ? 1 : 0); dig <= 9; dig++) {
                    int w = cost[dig];
                    if (rem < w) continue;
                    if (can_finish(rem - w, d - pos - 1)) {
                        cur.push_back(char('0' + dig));
                        rem -= w;
                        placed = true;
                        break;
                    }
                }
                if (!placed) {
                    ok = false;
                    break;
                }
            }
            if (ok && rem == 0 && (int)cur.size() == d) {
                ans = cur;
                break;
            }
        }
        cout << ans << "\n";
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    int ans = 0;
    for (int i = 1; i + 1 < n; ++i) {
        if ((a[i] > a[i - 1] && a[i] > a[i + 1]) ||
            (a[i] < a[i - 1] && a[i] < a[i + 1]))
            ++ans;
    }
    cout << ans << '\n';
    return 0;
}

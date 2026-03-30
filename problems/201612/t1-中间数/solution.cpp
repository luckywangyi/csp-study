#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    sort(a.begin(), a.end());
    for (int i = 0; i < n;) {
        int j = i;
        while (j < n && a[j] == a[i]) ++j;
        int left = i;
        int right = n - j;
        if (left == right) {
            cout << a[i] << '\n';
            return 0;
        }
        i = j;
    }
    cout << -1 << '\n';
    return 0;
}

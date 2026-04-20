#include <bits/stdc++.h>
using namespace std;

static bool reducible(const string& s) {
    string st;
    for (char c : s) {
        if (!st.empty() && st.back() == c)
            st.pop_back();
        else
            st.push_back(c);
    }
    return st.empty();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    string str;
    cin >> n >> str;
    long long ans = 0;
    for (int l = 0; l < n; l++) {
        for (int r = l; r < n; r++) {
            if (reducible(str.substr(l, r - l + 1))) ans++;
        }
    }
    cout << ans << '\n';
    return 0;
}

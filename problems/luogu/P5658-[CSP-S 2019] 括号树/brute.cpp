// P5658 括号树 — 暴力：对每个结点枚举所有子串，用 set 去重
#include <bits/stdc++.h>
using namespace std;

static bool is_balanced(const string& t) {
    int bal = 0;
    for (char c : t) {
        if (c == '(')
            ++bal;
        else
            --bal;
        if (bal < 0) return false;
    }
    return bal == 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    string s;
    cin >> s;
    s = " " + s;
    vector<int> parent(n + 1);
    parent[1] = 0;
    for (int i = 2; i <= n; ++i) cin >> parent[i];

    vector<int> path_nodes;
    unsigned long long ans_xor = 0;

    for (int i = 1; i <= n; ++i) {
        path_nodes.clear();
        for (int u = i; u != 0; u = parent[u]) path_nodes.push_back(u);
        reverse(path_nodes.begin(), path_nodes.end());
        string pstr;
        pstr.reserve(path_nodes.size());
        for (int u : path_nodes) pstr.push_back(s[u]);

        set<string> good;
        int L = (int)pstr.size();
        for (int a = 0; a < L; ++a) {
            for (int b = a; b < L; ++b) {
                string sub = pstr.substr(a, b - a + 1);
                if (is_balanced(sub)) good.insert(sub);
            }
        }
        unsigned long long k = good.size();
        ans_xor ^= (unsigned long long)i * k;
    }

    cout << ans_xor << '\n';
    return 0;
}

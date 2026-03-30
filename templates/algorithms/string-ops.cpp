/*
 * 字符串：KMP、分割、stringstream。
 */
#include <bits/stdc++.h>
using namespace std;

vector<int> kmp_pi(const string &p) {
    int m = (int)p.size();
    vector<int> pi(m);
    for (int i = 1; i < m; ++i) {
        int j = pi[i - 1];
        while (j && p[i] != p[j]) j = pi[j - 1];
        if (p[i] == p[j]) ++j;
        pi[i] = j;
    }
    return pi;
}

// 返回所有匹配起始下标
vector<int> kmp_find(const string &s, const string &p) {
    vector<int> pi = kmp_pi(p), res;
    int n = (int)s.size(), m = (int)p.size(), j = 0;
    for (int i = 0; i < n; ++i) {
        while (j && s[i] != p[j]) j = pi[j - 1];
        if (s[i] == p[j]) ++j;
        if (j == m) res.push_back(i - m + 1), j = pi[j - 1];
    }
    return res;
}

vector<string> split(const string &s, char delim) {
    vector<string> out;
    string cur;
    for (char ch : s) {
        if (ch == delim) {
            if (!cur.empty()) out.push_back(cur), cur.clear();
        } else cur += ch;
    }
    if (!cur.empty()) out.push_back(cur);
    return out;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    (void)kmp_find("abababa", "aba");

    string line = "1 2  3";
    stringstream ss(line);
    int x;
    while (ss >> x) { (void)x; }

    string csv = "a,b,c";
    auto parts = split(csv, ',');
    (void)parts.size();
    return 0;
}

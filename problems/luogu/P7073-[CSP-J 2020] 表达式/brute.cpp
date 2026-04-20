// P7073 表达式 — 暴力：每次询问复制初值并在后缀表达式上完整求值
#include <bits/stdc++.h>
using namespace std;

static int var_index(const string& tok) {
    int x = 0;
    for (size_t i = 1; i < tok.size(); ++i) x = x * 10 + (tok[i] - '0');
    return x;
}

static int eval_postfix(const vector<string>& tok, const vector<int>& val) {
    vector<int> st;
    st.reserve(tok.size());
    for (const string& t : tok) {
        if (t == "&") {
            int b = st.back();
            st.pop_back();
            int a = st.back();
            st.pop_back();
            st.push_back(a & b);
        } else if (t == "|") {
            int b = st.back();
            st.pop_back();
            int a = st.back();
            st.pop_back();
            st.push_back(a | b);
        } else if (t == "!") {
            int a = st.back();
            st.pop_back();
            st.push_back(a ? 0 : 1);
        } else {
            int id = var_index(t);
            st.push_back(val[id]);
        }
    }
    return st.back();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string line;
    getline(cin, line);
    stringstream ss(line);
    vector<string> tok;
    string w;
    while (ss >> w) tok.push_back(w);

    int n;
    cin >> n;
    vector<int> init(n + 1);
    for (int i = 1; i <= n; ++i) cin >> init[i];
    int q;
    cin >> q;
    while (q--) {
        int idx;
        cin >> idx;
        vector<int> val = init;
        val[idx] ^= 1;
        cout << eval_postfix(tok, val) << '\n';
    }
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

struct State {
    array<int, 5> d{};
    bool operator==(const State& o) const { return d == o.d; }
    bool operator<(const State& o) const { return d < o.d; }
};

static State read_state(istream& in) {
    State s;
    for (int i = 0; i < 5; i++) in >> s.d[i];
    return s;
}

static vector<State> neighbors(const State& x) {
    set<State> st;
    for (int i = 0; i < 5; i++) {
        for (int k = 1; k <= 9; k++) {
            State s = x;
            s.d[i] = (s.d[i] + k) % 10;
            st.insert(s);
            s = x;
            s.d[i] = (s.d[i] - k + 100) % 10;
            st.insert(s);
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int k = 1; k <= 9; k++) {
            State s = x;
            s.d[i] = (s.d[i] + k) % 10;
            s.d[i + 1] = (s.d[i + 1] + k) % 10;
            st.insert(s);
            s = x;
            s.d[i] = (s.d[i] - k + 100) % 10;
            s.d[i + 1] = (s.d[i + 1] - k + 100) % 10;
            st.insert(s);
        }
    }
    return vector<State>(st.begin(), st.end());
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<State> obs(n);
    for (int i = 0; i < n; i++) obs[i] = read_state(cin);

    long long ans = 0;
    for (int code = 0; code < 100000; code++) {
        State x;
        int t = code;
        for (int i = 4; i >= 0; i--) {
            x.d[i] = t % 10;
            t /= 10;
        }
        bool bad = false;
        for (auto& o : obs)
            if (x == o) bad = true;
        if (bad) continue;
        auto nb = neighbors(x);
        set<State> nbset(nb.begin(), nb.end());
        bool ok = true;
        for (auto& o : obs) {
            if (!nbset.count(o)) {
                ok = false;
                break;
            }
        }
        if (ok) ans++;
    }
    cout << ans << '\n';
    return 0;
}

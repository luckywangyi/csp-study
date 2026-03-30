/*
 * 线段树：单点改区间和、懒标记区间加；树状数组区间和。
 */
#include <bits/stdc++.h>
using namespace std;

struct SegTree {
    int n;
    vector<long long> t;
    SegTree(const vector<long long> &a) {
        n = 1;
        while (n < (int)a.size()) n <<= 1;
        t.assign(2 * n, 0);
        for (int i = 0; i < (int)a.size(); ++i) t[n + i] = a[i];
        for (int i = n - 1; i >= 1; --i) t[i] = t[i << 1] + t[i << 1 | 1];
    }
    void add(int i, long long v) {
        for (i += n; i; i >>= 1) t[i] += v;
    }
    long long qsum(int l, int r) { // [l,r)
        long long s = 0;
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) s += t[l++];
            if (r & 1) s += t[--r];
        }
        return s;
    }
};

struct SegLazy {
    int n;
    vector<long long> t, lz;
    SegLazy(int _n) {
        n = 1;
        while (n < _n) n <<= 1;
        t.assign(2 * n, 0);
        lz.assign(2 * n, 0);
    }
    void push(int o, int len) {
        if (!lz[o]) return;
        t[o << 1] += lz[o] * (len >> 1);
        t[o << 1 | 1] += lz[o] * (len >> 1);
        lz[o << 1] += lz[o];
        lz[o << 1 | 1] += lz[o];
        lz[o] = 0;
    }
    void pull(int o) { t[o] = t[o << 1] + t[o << 1 | 1]; }
    void range_add(int o, int l, int r, int ql, int qr, long long v) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) {
            t[o] += v * (r - l);
            lz[o] += v;
            return;
        }
        push(o, r - l);
        int m = (l + r) >> 1;
        range_add(o << 1, l, m, ql, qr, v);
        range_add(o << 1 | 1, m, r, ql, qr, v);
        pull(o);
    }
    long long range_sum(int o, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) return 0;
        if (ql <= l && r <= qr) return t[o];
        push(o, r - l);
        int m = (l + r) >> 1;
        return range_sum(o << 1, l, m, ql, qr) + range_sum(o << 1 | 1, m, r, ql, qr);
    }
};

struct BIT {
    int n;
    vector<long long> c;
    BIT(int n = 0) : n(n), c(n + 1) {}
    void add(int i, long long v) {
        for (; i <= n; i += i & -i) c[i] += v;
    }
    long long sum(int i) {
        long long s = 0;
        for (; i; i -= i & -i) s += c[i];
        return s;
    }
    long long range_sum(int l, int r) { return sum(r) - sum(l - 1); }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<long long> a = {1, 2, 3};
    SegTree st(a);
    st.add(1, 10);
    (void)st.qsum(0, 3);
    SegLazy sl(3);
    sl.range_add(1, 0, sl.n, 0, 3, 1);
    (void)sl.range_sum(1, 0, sl.n, 0, 2);
    BIT bit(5);
    bit.add(2, 5);
    (void)bit.range_sum(1, 3);
    return 0;
}

/*
 * 二分：有序数组查找、手写 lower/upper_bound、二分答案。
 */
#include <bits/stdc++.h>
using namespace std;

// 返回最大下标 i 使 a[i] <= x，无则 -1（a 升序）
int bs_last_leq(const vector<int> &a, int x) {
    int l = 0, r = (int)a.size() - 1, ans = -1;
    while (l <= r) {
        int m = (l + r) / 2;
        if (a[m] <= x) ans = m, l = m + 1;
        else r = m - 1;
    }
    return ans;
}

// 手写 lower_bound：第一个 >= x
int my_lower(const vector<int> &a, int x) {
    int l = 0, r = (int)a.size();
    while (l < r) {
        int m = (l + r) / 2;
        if (a[m] < x) l = m + 1;
        else r = m;
    }
    return l;
}

// 手写 upper_bound：第一个 > x
int my_upper(const vector<int> &a, int x) {
    int l = 0, r = (int)a.size();
    while (l < r) {
        int m = (l + r) / 2;
        if (a[m] <= x) l = m + 1;
        else r = m;
    }
    return l;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<int> a = {1, 3, 3, 5, 7};
    auto it = lower_bound(a.begin(), a.end(), 3);
    auto jt = upper_bound(a.begin(), a.end(), 3);
    (void)(it != a.end());
    (void)(jt != a.end());

    // 二分答案：最小化，检查 mid 是否可行
    int lo = 0, hi = 1e9;
    auto ok = [](int mid) { return mid >= 42; };
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (ok(mid)) hi = mid;
        else lo = mid + 1;
    }
    (void)lo;

    (void)bs_last_leq(a, 3);
    (void)my_lower(a, 3);
    (void)my_upper(a, 3);
    return 0;
}

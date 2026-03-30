/*
 * 排序：快排、归并、计数排序；自定义比较与 sort + lambda。
 */
#include <bits/stdc++.h>
using namespace std;

// 快速排序 [l, r)
void quick_sort(vector<int> &a, int l, int r) {
    if (r - l <= 1) return;
    int p = a[l + (r - l) / 2], i = l, j = r - 1;
    while (i <= j) {
        while (a[i] < p) ++i;
        while (a[j] > p) --j;
        if (i <= j) swap(a[i++], a[j--]);
    }
    quick_sort(a, l, j + 1);
    quick_sort(a, i, r);
}

void merge_sort(vector<int> &a, int l, int r, vector<int> &tmp) {
    if (r - l <= 1) return;
    int m = (l + r) / 2;
    merge_sort(a, l, m, tmp);
    merge_sort(a, m, r, tmp);
    int i = l, j = m, k = l;
    while (i < m && j < r)
        tmp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i < m) tmp[k++] = a[i++];
    while (j < r) tmp[k++] = a[j++];
    copy(tmp.begin() + l, tmp.begin() + r, a.begin() + l);
}

// 值域较小时用，maxv 为最大元素
void counting_sort(vector<int> &a, int maxv) {
    vector<int> cnt(maxv + 1);
    for (int x : a) cnt[x]++;
    int p = 0;
    for (int v = 0; v <= maxv; ++v)
        while (cnt[v]--) a[p++] = v;
}

struct Node {
    int x, y;
};
bool cmp_node(const Node &a, const Node &b) {
    return a.x != b.x ? a.x < b.x : a.y > b.y;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<int> a = {3, 1, 4, 1, 5};
    quick_sort(a, 0, (int)a.size());
    vector<int> b = {9, 2, 6}, tmp(b.size());
    merge_sort(b, 0, (int)b.size(), tmp);
    vector<int> c = {2, 0, 2, 1};
    counting_sort(c, 2);

    sort(a.begin(), a.end(), [](int u, int v) { return u > v; });

    vector<Node> ns = {{1, 2}, {2, 1}, {1, 3}};
    sort(ns.begin(), ns.end(), cmp_node);
    (void)b;
    return 0;
}

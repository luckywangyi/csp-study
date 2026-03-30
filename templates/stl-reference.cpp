/*
 * STL 容器与算法速查，考试时对照复制。
 */
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // ----- vector -----
    vector<int> a = {3, 1, 4};
    a.push_back(2);
    a.pop_back();
    a.size();
    a.empty();
    a.front();
    a.back();
    a.begin();
    a.end();
    a.insert(a.begin() + 1, 9);
    a.erase(a.begin());
    a.clear();
    a.resize(10);

    // ----- string -----
    string s = "abc";
    s += "d";
    s.substr(1, 2);
    s.find("b");
    stoi("42");
    to_string(42);

    // ----- map / unordered_map -----
    map<string, int> mp;
    mp["x"] = 1;
    mp.count("x");
    mp.erase("x");
    for (auto &[k, v] : mp) { (void)k; (void)v; }

    // ----- set / multiset -----
    set<int> st;
    st.insert(1);
    st.erase(1);
    auto it = st.lower_bound(2);
    multiset<int> ms;
    ms.insert(3);
    ms.erase(ms.find(3));

    // ----- priority_queue -----
    priority_queue<int> pq_max;
    priority_queue<int, vector<int>, greater<int>> pq_min;

    // ----- stack / queue / deque -----
    stack<int> sk;
    queue<int> q;
    deque<int> dq;
    dq.push_front(1);
    dq.push_back(2);

    // ----- 算法 -----
    sort(a.begin(), a.end());
    sort(a.begin(), a.end(), greater<int>());
    reverse(a.begin(), a.end());
    unique(a.begin(), a.end()); // 先 sort 再 unique，长度用 erase 配合
    int *p = lower_bound(a.begin(), a.end(), 2);
    upper_bound(a.begin(), a.end(), 2);
    next_permutation(a.begin(), a.end());
    prev_permutation(a.begin(), a.end());
    int sum = accumulate(a.begin(), a.end(), 0);
    int mx = *max_element(a.begin(), a.end());
    int mn = *min_element(a.begin(), a.end());
    (void)pq_max;
    (void)pq_min;
    (void)sk;
    (void)q;
    (void)dq;
    (void)it;
    (void)sum;
    (void)mx;
    (void)mn;
    (void)p;
    return 0;
}

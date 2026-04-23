/*
 * CCF CSP 标准解题模板：头文件、IO 优化、常用别名与常量。
 * 编译：g++ -std=c++17 -O2 solution.cpp
 */
#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using vi = vector<int>;
using vll = vector<ll>;

constexpr ll INF = 4e18;
constexpr int MOD = 1e9 + 7;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 多测时取消下一行注释
    // int T; cin >> T; while (T--) {

    // 解题代码

    // }
    int n;
    cin >> n;
    int count = 0;
    int day = 0;
    while (n != 0)
    {
        int temp = n / 3;
        int temp2 = n % 3;
        if (temp2 == 1 && day == 0) day = count + 1;
        if (temp < 1)
        {
            temp = 1;
            n -= 1;
        }
        else if (temp2 == 0)
        {
            n -= temp;
        }
        else
        {
            n -= temp + 1;
        }
        count++;
    }
    
    cout << count << " " << day;
    return 0;
}

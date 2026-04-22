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
    int n, L, r, t;
    cin >> n >> L >> r >> t;
    vector<vector<int>> arr(n, vector<int>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> arr[i][j];
        }
    }

    int count = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int sum = 0;
            int k = 0;
            for (int a = 0; a <= 2 * r; a++)
            {
                for (int b = 0; b <= 2 * r; b++)
                {
                    if ((i - r + a) < 0 || (i - r + a) >= n || (j - r + b) < 0 || (j - r + b) >= n)
                    {
                        continue;
                    }
                    sum += arr[i - r + a][j - r + b];
                    k++;
                }
            }
            if (sum <= t * k)
            {
                count++;
            }
        }
    }
    cout << count;
    return 0;
}

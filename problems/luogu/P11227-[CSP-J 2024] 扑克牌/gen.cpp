// P11227 扑克牌 — 数据生成器
// 用法: gen.exe <seed> <n_max>  （n 为牌数，不超过 52）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    mt19937 rng((unsigned)seed);
    int n_max = max(1, min(52, atoi(argv[2])));

    vector<string> deck;
    const string suits = "DCHS";
    const string ranks = "A23456789TJQK";
    for (char su : suits) {
        for (char rk : ranks) {
            string s;
            s.push_back(su);
            s.push_back(rk);
            deck.push_back(s);
        }
    }

    if (seed == 1) {
        cout << "1\nSA\n";
        return 0;
    }
    if (seed == 2) {
        cout << "4\nDQ\nH3\nDQ\nDT\n";
        return 0;
    }
    if (seed == 3) {
        cout << "52\n";
        for (auto& s : deck) cout << s << '\n';
        return 0;
    }
    if (seed == 4) {
        cout << "13\n";
        for (int i = 0; i < 13; i++) cout << deck[i] << '\n';
        return 0;
    }

    int n = uniform_int_distribution<int>(1, n_max)(rng);
    cout << n << '\n';
    uniform_int_distribution<int> pick(0, 51);
    for (int i = 0; i < n; i++) cout << deck[pick(rng)] << '\n';
    return 0;
}

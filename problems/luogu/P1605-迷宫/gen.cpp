// P1605 迷宫 — 数据生成器
// 用法: gen.exe <seed> <n_max>   （N,M 不超过 min(5, n_max)，与题面一致）
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
    int seed = (argc >= 2) ? atoi(argv[1]) : 1;
    int n_max = (argc >= 3) ? max(1, atoi(argv[2])) : 5;
    n_max = min(5, n_max);
    mt19937 rng((unsigned)seed + 0x9e3779b9u);

    if (seed == 1) {
        cout << "2 2 1\n1 1 2 2\n1 2\n";
        return 0;
    }
    if (seed == 2) {
        // 起点即终点
        cout << "2 2 0\n1 1 1 1\n";
        return 0;
    }
    if (seed == 3) {
        // 障碍堵死
        cout << "2 2 2\n1 1 2 2\n1 2\n2 1\n";
        return 0;
    }
    if (seed == 4) {
        // 3x3 无障碍，(1,1)->(3,3) 多条路径
        cout << "3 3 0\n1 1 3 3\n";
        return 0;
    }

    int N = uniform_int_distribution<int>(1, n_max)(rng);
    int M = uniform_int_distribution<int>(1, n_max)(rng);
    int SX, SY, FX, FY;
    for (int tries = 0; tries < 1000; ++tries) {
        SX = uniform_int_distribution<int>(1, N)(rng);
        SY = uniform_int_distribution<int>(1, M)(rng);
        FX = uniform_int_distribution<int>(1, N)(rng);
        FY = uniform_int_distribution<int>(1, M)(rng);
        if (SX != FX || SY != FY) break;
    }

    int maxT = min(10, N * M - 1);
    int T = uniform_int_distribution<int>(0, max(0, maxT))(rng);
    vector<vector<char>> occ(N + 1, vector<char>(M + 1, 0));
    occ[SX][SY] = 1;
    vector<pair<int, int>> obs;
    while ((int)obs.size() < T) {
        int x = uniform_int_distribution<int>(1, N)(rng);
        int y = uniform_int_distribution<int>(1, M)(rng);
        if (occ[x][y]) continue;
        occ[x][y] = 1;
        obs.emplace_back(x, y);
    }

    cout << N << ' ' << M << ' ' << T << '\n';
    cout << SX << ' ' << SY << ' ' << FX << ' ' << FY << '\n';
    for (auto [x, y] : obs) cout << x << ' ' << y << '\n';
    return 0;
}

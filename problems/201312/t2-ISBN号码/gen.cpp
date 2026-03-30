// 201312-T2 ISBN号码 — 数据生成器
// 用法: gen.exe <seed> <n_max>
// 输出格式: x-xxx-xxxxx-c (其中 c 可能正确也可能错误)
// n_max 控制是否生成正确校验位: n_max 为偶数时生成正确的，奇数时随机
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char* argv[]) {
    mt19937 rng((unsigned)atoi(argv[1]));
    int n_max = atoi(argv[2]);
    int digits[9];
    for (int i = 0; i < 9; i++)
        digits[i] = uniform_int_distribution<int>(0, 9)(rng);
    int sum = 0;
    for (int i = 0; i < 9; i++) sum += (i + 1) * digits[i];
    int r = sum % 11;
    char correct = (r == 10) ? 'X' : (char)('0' + r);
    char check;
    if (n_max % 2 == 0) {
        check = correct;
    } else {
        // 随机生成一个可能错误的校验位
        string pool = "0123456789X";
        check = pool[uniform_int_distribution<int>(0, 10)(rng)];
    }
    // 格式: d-ddd-ddddd-c
    cout << digits[0] << '-'
         << digits[1] << digits[2] << digits[3] << '-'
         << digits[4] << digits[5] << digits[6] << digits[7] << digits[8] << '-'
         << check << "\n";
}

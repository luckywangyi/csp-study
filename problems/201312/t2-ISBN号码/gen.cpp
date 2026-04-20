// 201312-T2 ISBN号码 — 数据生成器
// 用法: gen.exe <seed> <n_max>
// 输出格式: x-xxx-xxxxx-c (其中 c 可能正确也可能错误)
#include <bits/stdc++.h>
using namespace std;
int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    mt19937 rng((unsigned)seed);
    int n_max = atoi(argv[2]);

    // 边界用例
    if (seed == 1) {
        // 校验码为 X 的情况：1*1+9*1=10, 10%11=10 → X
        cout << "1-000-00001-X\n";
        return 0;
    }
    if (seed == 2) {
        // 全零 -> sum=0, 0%11=0, check='0', correct
        cout << "0-000-00000-0\n";
        return 0;
    }
    if (seed == 3) {
        // 明显错误的校验码
        cout << "0-670-82162-0\n";
        return 0;
    }
    if (seed == 4) {
        // 正确的校验码
        cout << "0-670-82162-4\n";
        return 0;
    }

    // 随机数据
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
        string pool = "0123456789X";
        check = pool[uniform_int_distribution<int>(0, 10)(rng)];
    }
    cout << digits[0] << '-'
         << digits[1] << digits[2] << digits[3] << '-'
         << digits[4] << digits[5] << digits[6] << digits[7] << digits[8] << '-'
         << check << "\n";
}

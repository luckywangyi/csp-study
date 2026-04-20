// 201312-T2 ISBN号码 — 暴力标程
// 逐字符提取数字，计算校验码，比较后输出
#include <bits/stdc++.h>
using namespace std;

int main() {
    string s;
    cin >> s;
    // s 格式: x-xxx-xxxxx-c，提取前 9 个数字
    vector<int> digits;
    for (char c : s) {
        if (c >= '0' && c <= '9') digits.push_back(c - '0');
        else if (c == 'X') digits.push_back(10);
    }
    // digits 有 10 个元素，前 9 个是数据位，第 10 个是校验位
    int sum = 0;
    for (int i = 0; i < 9; i++) {
        sum += (i + 1) * digits[i];
    }
    int r = sum % 11;
    char correct = (r == 10) ? 'X' : (char)('0' + r);

    char given = s.back();
    if (given == correct) {
        cout << "Right" << "\n";
    } else {
        s.back() = correct;
        cout << s << "\n";
    }
    return 0;
}

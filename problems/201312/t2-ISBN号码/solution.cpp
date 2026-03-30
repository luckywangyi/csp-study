#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    if (!getline(cin, s)) return 0;
    while (!s.empty() && (s.back() == '\r' || s.back() == '\n')) s.pop_back();

    string compact;
    compact.reserve(s.size());
    for (char c : s)
        if (c != '-') compact += c;

    int sum = 0;
    for (int i = 0; i < 9; ++i) sum += (i + 1) * (compact[i] - '0');
    int r = sum % 11;
    char correct = (r == 10) ? 'X' : static_cast<char>('0' + r);
    char given = compact[9];
    if (given == correct) {
        cout << "Right\n";
    } else {
        s.back() = correct;
        cout << s << '\n';
    }
    return 0;
}

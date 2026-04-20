// P7911 网络连接 — 暴力：按题意解析地址、维护已注册服务机地址
#include <bits/stdc++.h>
using namespace std;

bool isValid(const string& ad) {
    int colons = 0, dots = 0;
    for (char c : ad) {
        if (c == ':') colons++;
        else if (c == '.') dots++;
    }
    if (colons != 1) return false;
    size_t p = ad.find(':');
    string ip = ad.substr(0, p);
    string port = ad.substr(p + 1);
    if (ip.empty() || port.empty()) return false;
    if (count(ip.begin(), ip.end(), '.') != 3) return false;
    stringstream ss(ip);
    string part;
    vector<long long> nums;
    while (getline(ss, part, '.')) {
        if (part.empty()) return false;
        if (part.size() > 1 && part[0] == '0') return false;
        for (char c : part)
            if (!isdigit((unsigned char)c)) return false;
        long long v = stoll(part);
        if (v < 0 || v > 255) return false;
        nums.push_back(v);
    }
    if (nums.size() != 4) return false;
    if (port.empty()) return false;
    if (port.size() > 1 && port[0] == '0') return false;
    for (char c : port)
        if (!isdigit((unsigned char)c)) return false;
    long long pe = stoll(port);
    if (pe < 0 || pe > 65535) return false;
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    cin.ignore();
    unordered_map<string, int> serverAddr;
    for (int i = 1; i <= n; i++) {
        string line;
        getline(cin, line);
        stringstream ls(line);
        string op, ad;
        ls >> op >> ad;
        if (!isValid(ad)) {
            cout << "ERR\n";
            continue;
        }
        if (op == "Server") {
            if (serverAddr.count(ad)) {
                cout << "FAIL\n";
            } else {
                serverAddr[ad] = i;
                cout << "OK\n";
            }
        } else {
            auto it = serverAddr.find(ad);
            if (it == serverAddr.end())
                cout << "FAIL\n";
            else
                cout << it->second << "\n";
        }
    }
    return 0;
}

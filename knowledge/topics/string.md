# 字符串处理专题（CSP）

## C++ 字符串基础

### `std::string`

- 动态长度，支持 `+`、`+=`、`substr`、`find`、`compare`。  
- 下标从 `0` 开始；`s.size()` 与 `s.length()` 等价。  
- 与 `cin` 读入时注意：**`cin >> s` 不能读空格**；整行用 `getline(cin, s)`。

### 字符数组 `char[]`

- 部分老题或固定缓冲区场景仍可能出现。  
- 注意 `\0` 结尾与 `strlen` 的 O(n) 成本。

### 常用转换

```cpp
#include <string>
int x = stoi(s);
long long y = stoll(s);
string t = to_string(x);
```

`stoi`/`stoll` 在非法格式时会抛异常；竞赛中若输入保证合法可直接用，否则手写解析更安全。

---

## 常见字符串操作

### 分割（手写按分隔符）

```cpp
vector<string> split(const string& s, char delim) {
    vector<string> out;
    string cur;
    for (char c : s) {
        if (c == delim) {
            if (!cur.empty()) out.push_back(cur);
            cur.clear();
        } else cur += c;
    }
    if (!cur.empty()) out.push_back(cur);
    return out;
}
```

连续分隔符是否产生空串需按题意处理：有的题目要跳过空 token，有的要保留。

### 查找与替换

- `s.find(sub, pos)` 从 `pos` 起找子串，返回下标或 `string::npos`。  
- 循环替换时注意 **自增位置**，避免死循环（替换后的串又匹配同一模式）。

### 大小写与分类

```cpp
#include <cctype>
if (isdigit(static_cast<unsigned char>(c))) { /* ... */ }
if (isalpha(static_cast<unsigned char>(c))) { /* ... */ }
```

对 `char` 调用 `cctype` 函数前转为 `unsigned char`，避免负值未定义行为。

---

## 解析技巧：`stringstream` 与状态机

### `stringstream`

适合按空格切分或一行内混合读入：

```cpp
#include <sstream>
string line;
getline(cin, line);
stringstream ss(line);
string w;
while (ss >> w) {
    // 处理每个 token
}
```

### 状态机

对「括号嵌套」「多种 token」「引号内原样保留」类题，显式定义状态枚举，逐字符转移，分支清晰、易对拍。

---

## 模式匹配：KMP 算法

### 思想

- **next/pi 数组**：对每个前缀，求 **最长相等真前后缀** 长度。  
- 失配时模式串指针跳到 `next[j-1]`，文本指针不回退，总时间 O(n+m)。

### 适用场景

多次子串匹配、最小周期、与 DP/字符串结合。CSP 中若 `n,m` 较大且暴力会 TLE，应想到 KMP 或哈希（双哈希防碰撞）。

### next 数组构建（示意）

```cpp
vector<int> build_next(const string& p) {
    int m = (int)p.size();
    vector<int> nxt(m, 0);
    for (int i = 1, j = 0; i < m; ++i) {
        while (j > 0 && p[i] != p[j]) j = nxt[j - 1];
        if (p[i] == p[j]) ++j;
        nxt[i] = j;
    }
    return nxt;
}
```

### 匹配主循环（示意）

```cpp
void kmp_search(const string& s, const string& p, const vector<int>& nxt) {
    int n = (int)s.size(), m = (int)p.size();
    for (int i = 0, j = 0; i < n; ++i) {
        while (j > 0 && s[i] != p[j]) j = nxt[j - 1];
        if (s[i] == p[j]) ++j;
        if (j == m) {
            // 匹配位置 i - m + 1
            j = nxt[j - 1];
        }
    }
}
```

---

## 字符串哈希（竞赛常用）

将子串映射为基数哈希值，O(1) 比较（概率意义下）。需注意 **模数、冲突、卡哈希**；重要场合可用 **双模数** 或 KMP。

### 前缀哈希 + 幂次（示意）

```cpp
using ull = unsigned long long;
// 或使用两个 long long 模大质数做双哈希
vector<ull> pref, pw;
void build_hash(const string& s, ull base = 131) {
    int n = (int)s.size();
    pref.assign(n + 1, 0);
    pw.assign(n + 1, 1);
    for (int i = 0; i < n; ++i) {
        pref[i + 1] = pref[i] * base + (unsigned char)s[i];
        pw[i + 1] = pw[i] * base;
    }
}
// 子串 [l, r) 哈希：pref[r] - pref[l] * pw[r-l]
```

实际提交时建议选用 **明确取模** 的双哈希方案，避免 `ull` 自然溢出被针对性卡掉。

---

## CSP 中的字符串真题

**很多 T3 是字符串 + 模拟**：校验规则、格式解析、大整数用字符串存储等。

本仓库可参考：

| 路径 | 说明 |
|------|------|
| `problems/201312/t2-ISBN号码` | 校验位、字符与数字混合处理 |

建议在洛谷/CCF OJ 将 CSP 历年题按「字符串」筛选，与 `topics/simulation.md` 对照阅读。

---

## 「正则思维」在解题中的应用

竞赛实现通常不用 `std::regex`（编译器差异与性能），但 **正则的抽象**很有用：

- 把合法串描述为：**若干段**（数字段、字母段、固定字符）。  
- 实现时用 **下标扫描 + 分支**，等价于手写 NFA。  

示例：「非空数字串 + '-' + 固定长度字母」→ 先 `while (isdigit)`，再期望 `'-'`，再读满 `k` 个字母，否则 `invalid`。

---

## 大整数（思想）

超出 `long long` 的整数可用 **十进制字符串** 存，按位模拟加减乘除；CSP 部分题只要求加/乘 small number，可逐位处理。

---

## 常见坑点

| 坑点 | 说明 |
|------|------|
| `getline` 前残留换行 | 若前面有 `cin >> n`，需 `cin.ignore(numeric_limits<streamsize>::max(), '\n');` |
| `size_t` 与 `int` 混用 | 循环 `i < s.size()` 时若 `i` 为 `int`，注意比较统一或强转 |
| 子串越界 | `substr(pos, len)` 要求 `pos <= s.size()`，`len` 过长时取到末尾 |
| 多测未清空 | `string`、`stringstream` 每次测试重新构造或 `clear()` |

### `getline` 与 `cin` 混用示例

```cpp
#include <limits>
// ...
int n;
cin >> n;
cin.ignore(numeric_limits<streamsize>::max(), '\n');
string line;
getline(cin, line);
```

---

## 与 `vector<char>`、`string` 的选择

- 需要频繁 `push_back`/`pop_back` 且最终当串输出：两者皆可，`string` 更惯用。  
- 需要与 C 风格 API 交互：注意 `.c_str()` 与生命周期。

---

## Z 函数（扩展，了解）

`z[i]` 表示 `s` 与 `s[i..]` 的最长公共前缀长度，可在 O(n) 求出，用于子串分析与匹配；掌握 KMP 后再学 Z 函数可互为补充。

---

## 小结

字符串题得分点往往在 **细节与鲁棒性**：读入方式、边界、字符分类。建议熟练 `string` + `stringstream` + KMP/哈希之一，并与模拟专题联动。图论/DP 中与字符串结合的题，先 **抽象成序列或自动机** 再选算法。

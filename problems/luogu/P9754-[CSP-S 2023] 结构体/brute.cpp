#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;

static ull align_up(ull x, ull a) {
    if (a == 0) return x;
    return (x + a - 1) / a * a;
}

struct TypeDesc {
    bool basic = false;
    string basic_kind;
    ull size = 0, alignv = 0;
    vector<tuple<string, string, ull, ull, ull>> members;
};

static bool is_basic(const string& t) {
    return t == "byte" || t == "short" || t == "int" || t == "long";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    map<string, pair<ull, ull>> prim = {
        {"byte", {1, 1}}, {"short", {2, 2}}, {"int", {4, 4}}, {"long", {8, 8}}};

    int nq;
    cin >> nq;
    map<string, TypeDesc> types;
    for (auto& [k, v] : prim) {
        TypeDesc td;
        td.basic = true;
        td.basic_kind = k;
        td.size = v.first;
        td.alignv = v.second;
        types[k] = td;
    }

    vector<string> elem_name;
    vector<string> elem_type;
    vector<ull> elem_base;
    vector<ull> elem_size;
    vector<ull> elem_align;

    vector<tuple<string, ull, string>> prim_cells;

    function<void(const string&, ull, const string&)> collect =
        [&](const string& ty, ull base, const string& path) {
            const TypeDesc& td = types.at(ty);
            if (td.basic) {
                prim_cells.emplace_back(path, base, ty);
                return;
            }
            for (auto& [mt, mn, off, msz, mal] : td.members) {
                (void)msz;
                (void)mal;
                string npath = path + "." + mn;
                if (is_basic(mt))
                    prim_cells.emplace_back(npath, base + off, mt);
                else
                    collect(mt, base + off, npath);
            }
        };

    for (int qi = 0; qi < nq; qi++) {
        int op;
        cin >> op;
        if (op == 1) {
            string s;
            int k;
            cin >> s >> k;
            ull cur = 0;
            ull amax = 1;
            vector<tuple<string, string, ull, ull, ull>> mems;
            for (int i = 0; i < k; i++) {
                string ti, ni;
                cin >> ti >> ni;
                const TypeDesc& inner = types.at(ti);
                ull al = inner.alignv;
                ull sz = inner.size;
                amax = max(amax, al);
                cur = align_up(cur, al);
                ull off = cur;
                cur += sz;
                mems.emplace_back(ti, ni, off, sz, al);
            }
            ull st_align = amax;
            ull tot = align_up(cur, st_align);
            TypeDesc td;
            td.basic = false;
            td.size = tot;
            td.alignv = st_align;
            td.members = mems;
            types[s] = td;
            cout << tot << ' ' << st_align << '\n';
        } else if (op == 2) {
            string t, ename;
            cin >> t >> ename;
            const TypeDesc& td = types.at(t);
            ull base = 0;
            if (!elem_base.empty()) {
                ull prev_end = elem_base.back() + elem_size.back();
                base = align_up(prev_end, td.alignv);
            }
            elem_name.push_back(ename);
            elem_type.push_back(t);
            elem_base.push_back(base);
            elem_size.push_back(td.size);
            elem_align.push_back(td.alignv);
            cout << base << '\n';
        } else if (op == 3) {
            string path;
            cin >> path;
            size_t pos = path.find('.');
            string root_name = path.substr(0, pos);
            string rest = pos == string::npos ? string() : path.substr(pos + 1);
            int ei = -1;
            for (int i = 0; i < (int)elem_name.size(); i++) {
                if (elem_name[i] == root_name) {
                    ei = i;
                    break;
                }
            }
            ull addr = elem_base[ei];
            string cur_type = elem_type[ei];
            string tail = rest;
            while (!tail.empty()) {
                size_t p = tail.find('.');
                string seg = p == string::npos ? tail : tail.substr(0, p);
                tail = p == string::npos ? string() : tail.substr(p + 1);
                const TypeDesc& td = types.at(cur_type);
                ull off = 0;
                string next_type;
                for (auto& [mt, mn, o, sz, al] : td.members) {
                    (void)sz;
                    (void)al;
                    if (mn == seg) {
                        off = o;
                        next_type = mt;
                        break;
                    }
                }
                addr += off;
                cur_type = next_type;
            }
            cout << addr << '\n';
        } else if (op == 4) {
            ull addr;
            cin >> addr;
            prim_cells.clear();
            for (size_t i = 0; i < elem_base.size(); i++) {
                collect(elem_type[i], elem_base[i], elem_name[i]);
            }
            string ans_path;
            bool found = false;
            for (auto& [p, a, t] : prim_cells) {
                const TypeDesc& td = types.at(t);
                if (a <= addr && addr < a + td.size) {
                    if (!found) {
                        found = true;
                        ans_path = p;
                    }
                }
            }
            if (!found)
                cout << "ERR\n";
            else
                cout << ans_path << '\n';
        }
    }
    return 0;
}

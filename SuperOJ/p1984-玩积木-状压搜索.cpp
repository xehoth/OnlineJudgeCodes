/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 1984」玩积木 15-09-2017
 * 状压搜索
 * @author xehoth
 */
#include <bits/stdc++.h>
#include <tr1/unordered_map>

namespace {

typedef unsigned long long ulong;

inline int getPos(int x, int y) {
    switch (x) {
        case 0:
            return 0;
        case 1:
            return 1 + y;
        case 2:
            return 3 + y;
        case 3:
            return 6 + y;
        case 4:
            return 10 + y;
        case 5:
            return 15 + y;
    }
}

inline int show(ulong s, const int index) { return (s >> (index << 2)) & 15ll; }

inline int get(const ulong &s, const int index) {
    return (s >> (index << 2)) & 15ll;
}
std::bitset<64> tmp;

inline void set(ulong &s, const int index, const int val) {
    s &= ~(15ll << (index << 2));
    s |= (val & 15ll) << (index << 2);
}

std::tr1::unordered_map<ulong, int> vis;

ulong O1, O2;

int ans;

void dfs(const int x, const int y, ulong d1, ulong d2, int step) {
    if (step > ans) return;
    if (step > 14) return;
    if (d1 == O1 && d2 == O2) {
        ans = std::min(ans, step);
        return;
    }
    register ulong h;
    if (x > 0 && y > 0) {
        register ulong tmp1 = d1, tmp2 = d2;
        register int pos1 = getPos(x, y), pos2 = getPos(x - 1, y - 1);
        register int val1 = (pos1 <= 10 ? get(d1, pos1) : get(d2, pos1 - 11));
        register int val2 = (pos2 <= 10 ? get(d1, pos2) : get(d2, pos2 - 11));
        if (pos1 <= 10)
            set(tmp1, pos1, val2);
        else
            set(tmp2, pos1 - 11, val2);
        if (pos2 <= 10)
            set(tmp1, pos2, val1);
        else
            set(tmp2, pos2 - 11, val1);
        if (!vis[h = tmp1 * tmp2]) {
            vis[h]++;
            dfs(x - 1, y - 1, tmp1, tmp2, step + 1);
            vis[h]--;
        }
    }
    if (x > 0 && y <= x - 1) {
        register ulong tmp1 = d1, tmp2 = d2;
        register int pos1 = getPos(x, y), pos2 = getPos(x - 1, y);
        register int val1 = (pos1 <= 10 ? get(d1, pos1) : get(d2, pos1 - 11));
        register int val2 = (pos2 <= 10 ? get(d1, pos2) : get(d2, pos2 - 11));
        if (pos1 <= 10)
            set(tmp1, pos1, val2);
        else
            set(tmp2, pos1 - 11, val2);
        if (pos2 <= 10)
            set(tmp1, pos2, val1);
        else
            set(tmp2, pos2 - 11, val1);
        if (!vis[h = tmp1 * tmp2]) {
            vis[h]++;
            dfs(x - 1, y, tmp1, tmp2, step + 1);
            vis[h]--;
        }
    }
    if (x < 5) {
        register ulong tmp1 = d1, tmp2 = d2;
        register int pos1 = getPos(x, y), pos2 = getPos(x + 1, y);
        register int val1 = (pos1 <= 10 ? get(d1, pos1) : get(d2, pos1 - 11));
        register int val2 = (pos2 <= 10 ? get(d1, pos2) : get(d2, pos2 - 11));
        if (pos1 <= 10)
            set(tmp1, pos1, val2);
        else
            set(tmp2, pos1 - 11, val2);
        if (pos2 <= 10)
            set(tmp1, pos2, val1);
        else
            set(tmp2, pos2 - 11, val1);
        if (!vis[h = tmp1 * tmp2]) {
            vis[h]++;
            dfs(x + 1, y, tmp1, tmp2, step + 1);
            vis[h]--;
        }
    }
    if (x < 5 && y < 5) {
        register ulong tmp1 = d1, tmp2 = d2;
        register int pos1 = getPos(x, y), pos2 = getPos(x + 1, y + 1);
        register int val1 = (pos1 <= 10 ? get(d1, pos1) : get(d2, pos1 - 11));
        register int val2 = (pos2 <= 10 ? get(d1, pos2) : get(d2, pos2 - 11));
        if (pos1 <= 10)
            set(tmp1, pos1, val2);
        else
            set(tmp2, pos1 - 11, val2);
        if (pos2 <= 10)
            set(tmp1, pos2, val1);
        else
            set(tmp2, pos2 - 11, val1);
        if (!vis[h = tmp1 * tmp2]) {
            vis[h]++;
            dfs(x + 1, y + 1, tmp1, tmp2, step + 1);
            vis[h]--;
        }
    }
}

ulong d1, d2;

inline void solveCase() {
    vis.clear();
    for (register int i = 0, pos; i < 6; i++) {
        for (register int j = 0; j <= i; j++) {
            pos = getPos(i, j);
            if (pos <= 10)
                set(O1, pos, i);
            else
                set(O2, pos - 11, i);
        }
    }
    register int initX, initY;
    for (register int i = 0, x, pos; i < 6; i++) {
        for (register int j = 0; j <= i; j++) {
            std::cin >> x;
            pos = getPos(i, j);
            if (x == 0) initX = i, initY = j;
            if (pos <= 10)
                set(d1, pos, x);
            else
                set(d2, pos - 11, x);
        }
    }
    ans = INT_MAX;
    show(12, 2);
    dfs(initX, initY, d1, d2, 0);
    if (ans == INT_MAX) {
        std::cout << "too difficult\n";
    } else {
        std::cout << ans << '\n';
    }
}

inline void solve() {
    register int T;
    std::cin >> T;
    while (T--) solveCase();
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    // freopen("blocks.out", "w", stdout);
    solve();
    return 0;
}
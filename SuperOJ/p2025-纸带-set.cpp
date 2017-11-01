/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2025」纸带 1-11-2017
 * set
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        for (c = read(); !isdigit(c); c = read())
            if (c == -1) return *this;
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        return *this;
    }
} io;

const int MAXN = 1000000;

struct Node {
    int l, r;

    inline bool operator<(const Node &p) const { return r + 1 < p.l; }
} d[MAXN + 1];

Node pool[MAXN + 1], *cur = pool;

struct Allocator : public std::allocator<Node> {
    inline pointer allocate(size_type n, const void * = 0) { return cur++; }
    inline void deallocate(pointer, size_type) {}
};

std::set<Node, std::less<Node>, Allocator> set;

int n, cnt;

inline void solve() {
    io >> n;
    for (register int i = 1; i <= n; i++) io >> d[i].l >> d[i].r, d[i].l++;
    for (register int i = n; i >= 1; i--) {
        register std::set<Node>::iterator t = set.lower_bound(d[i]);
        if (t == set.end()) {
            cnt++, set.insert(set.end(), d[i]);
            continue;
        }
        if (t->l <= d[i].l && t->r >= d[i].r) continue;
        for (cnt++; t != set.end() && (t->l <= d[i].r + 1);
             t = set.lower_bound(d[i])) {
            d[i].l = std::min(d[i].l, t->l), d[i].r = std::max(d[i].r, t->r);
            set.erase(t);
        }
        set.insert(t, d[i]);
    }
    std::cout << cnt;
}
}

int main() {
    solve();
    return 0;
}
/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2288」生日礼物 23-11-2017
 * 堆 + 链表
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 100000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
}

struct InputStream {
    template <typename T>
    inline InputStream &operator>>(T &x) {
        return read(x), *this;
    }
} io;
}  // namespace IO

namespace {

using IO::io;

const int MAXN = 100000 + 9;

std::list<int> list;

struct Comparator {
    inline bool operator()(const std::pair<int, int> &a,
                           const std::pair<int, int> &b) const {
        return std::abs(a.first) > std::abs(b.first);
    }
};

std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int> >,
                    Comparator>
    q;

std::list<int>::iterator it[MAXN];

int a[MAXN];

inline void solve() {
    register int n, m, cnt = 1, sum = 0, ans = 0;
    io >> n >> m;
    for (register int i = 1, x; i <= n; i++)
        io >> x, ((long long)a[cnt] * x >= 0) ? a[cnt] += x : a[++cnt] = x;
    n = cnt;
    for (register int i = 1; i <= n; i++)
        if (a[i] > 0) sum++, ans += a[i];
    for (register int i = 1; i <= n; i++)
        it[i] = list.insert(list.end(), i), q.push(std::make_pair(a[i], i));
    for (register int x, tId; sum > m;) {
        sum--;
        while (it[q.top().second] == list.end()) q.pop();
        x = q.top().second, q.pop();
        if (it[x] != list.begin() && it[x] != --list.end()) {
            ans -= std::abs(a[x]);
        } else if (a[x] > 0) {
            ans -= a[x];
        } else {
            sum++;
            continue;
        }
        std::list<int>::iterator tmp = it[x];
        if (tmp != list.begin())
            a[x] += a[tId = *--tmp], list.erase(tmp), it[tId] = list.end();
        if (++(tmp = it[x]) != list.end())
            a[x] += a[tId = *tmp], list.erase(tmp), it[tId] = list.end();
        q.push(std::make_pair(a[x], x));
    }
    std::cout << ans;
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
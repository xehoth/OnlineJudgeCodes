/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3578」GTY的人类基因组计划2 07-10-2017
 * Hash
 * @author xehoth
 */
#include <bits/stdc++.h>
#include <tr1/unordered_set>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
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

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
}

const int OUT_LEN = 100000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    ~InputOutputStream() { flush(); }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }
} io;
}

namespace {

typedef unsigned long long ulong;

inline ulong xorShift128Plus() {
    static ulong seed1 = 495;
    static ulong seed2 = 233;
    register ulong x = seed1;
    register const ulong y = seed2;
    seed1 = y, x ^= x << 23;
    seed2 = x ^ y ^ (x >> 17) ^ (y >> 26);
    return seed2 + y;
}

using IO::io;

std::set<int> s;
std::tr1::unordered_set<ulong> exist;
const int MAXN = 100010;

ulong num[MAXN], size[MAXN], pos[MAXN], h[MAXN];
int n, m, q;

inline void solve() {
    io >> n >> m >> q;
    for (register int i = 1; i <= n; i++) num[i] = xorShift128Plus();
    s.insert(1);
    for (register int i = 1; i <= n; i++) h[1] ^= num[i], size[1]++, pos[i] = 1;
    for (register int i = 1, x, y; i <= q; i++) {
        register char c;
        io >> c >> x >> y;
        switch (c) {
            case 'C': {
                if (pos[x] == y) continue;
                s.erase(pos[x]), s.erase(y);
                h[pos[x]] ^= num[x], size[pos[x]]--;
                if (exist.find(h[pos[x]]) == exist.end()) s.insert(pos[x]);
                h[y] ^= num[x], size[y]++;
                if (exist.find(h[y]) == exist.end()) s.insert(y);
                pos[x] = y;
                break;
            }
            case 'W': {
                register int ans = 0;
                for (register std::set<int>::iterator it = s.lower_bound(x);
                     it != s.end() && *it <= y; it = s.lower_bound(x))
                    exist.insert(h[*it]), ans += size[*it], s.erase(it);
                io << ans << '\n';
                break;
            }
        }
    }
}
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    solve();
    return 0;
}
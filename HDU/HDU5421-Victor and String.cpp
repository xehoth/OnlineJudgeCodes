/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「HDU 5421」Victor and String 14-09-2017
 * 回文自动机
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
}

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = 0;
    return s;
}

const int OUT_LEN = 1000000;

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

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

    ~InputOutputStream() { flush(); }
} io;
}

namespace {

#define long long long

using IO::io;

const int MAX_SIGMA = 26;
const int MAXN = 200000;
const int SIGMA = 26;

struct Node {
    int len, endPos, fail, c[MAX_SIGMA];
};

struct PalindromicTree {
    static const int even = 0;
    static const int odd = 1;

    Node d[MAXN + 5];
    int cur, last[2], l, r;
    long endPosSize;
    char s[MAXN + 1];

    inline void init() {
        memset(s, -1, sizeof(s));
        memset(d, 0, sizeof(d));
        d[odd].len = -1, d[even].fail = d[odd].fail = odd;
        cur = 2;
        endPosSize = 0;
        l = MAXN / 2, r = l - 1, last[0] = last[1] = even;
    }

    inline int getFailLeft(int p) {
        while (s[l + d[p].len + 1] != s[l]) p = d[p].fail;
        return p;
    }

    inline int getFailRight(int p) {
        while (s[r - d[p].len - 1] != s[r]) p = d[p].fail;
        return p;
    }

    inline void pushBack(char c) {
        s[++r] = c;
        register int p = getFailRight(last[1]);
        if (!d[p].c[c]) {
            register int np = cur++;
            d[np].len = d[p].len + 2;
            d[np].fail = d[getFailRight(d[p].fail)].c[c];
            d[np].endPos = d[d[np].fail].endPos + 1;
            d[p].c[c] = np;
        }
        p = d[p].c[c], last[1] = p, endPosSize += d[p].endPos;
        if (r - l + 1 == d[p].len) last[0] = p;
    }

    inline void pushFront(char c) {
        s[--l] = c;
        register int p = getFailLeft(last[0]);
        if (!d[p].c[c]) {
            register int np = cur++;
            d[np].len = d[p].len + 2;
            d[np].fail = d[getFailLeft(d[p].fail)].c[c];
            d[np].endPos = d[d[np].fail].endPos + 1;
            d[p].c[c] = np;
        }
        p = d[p].c[c], last[0] = p, endPosSize += d[p].endPos;
        if (r - l + 1 == d[p].len) last[1] = p;
    }

    inline void solve() {
        register int n;
        while (IO::read(n)) {
            register int cmd;
            static char ch;
            init();
            while (n--) {
                io >> cmd;
                if (cmd < 3) {
                    io >> ch;
                    if (cmd == 1) {
                        pushFront(ch - 'a');
                    } else {
                        pushBack(ch - 'a');
                    }
                } else if (cmd == 3) {
                    io << cur - 2 << '\n';
                } else {
                    io << endPosSize << '\n';
                }
            }
        }
    }

} task;

#undef long
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    task.solve();
    return 0;
}

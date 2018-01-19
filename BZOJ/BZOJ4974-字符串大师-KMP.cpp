/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 4974」字符串大师 03-01-2018
 * KMP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 1 << 18 | 1;
    static char buf[IN_LEN], *s, *t;
    return (s == t) && (t = (s = buf) + fread(buf, 1, IN_LEN, stdin)),
           s == t ? -1 : *s++;
}

template <typename T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        iosig |= c == '-';
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig && (x = -x);
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

const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

struct InputOutputStream {
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

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
} io;

const int MAXN = 100000 + 9;

int a[MAXN];

char s[MAXN];

std::bitset<29> vis;

inline void solve() {
    register int n;
    io >> n;
    for (register int i = 1; i <= n; i++) io >> a[i];
    s[1] = 'a';
    for (register int i = 2; i <= n; i++) {
        if (i != a[i]) {
            s[i] = s[(i - 1) % a[i] + 1];
        } else {
            vis.reset();
            register int tmp = i - 1;
            while (tmp != a[tmp]) {
                tmp = (tmp - 1) % a[tmp] + 1;
                vis.set(s[tmp + 1] - 'a');
            }
            for (register int j = 1; j < 26; j++) {
                if (!vis.test(j)) {
                    s[i] = 'a' + j;
                    break;
                }
            }
        }
    }
    io << (s + 1);
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
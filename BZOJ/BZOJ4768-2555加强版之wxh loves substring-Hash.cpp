/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4768」2555加强版之wxh loves substring 13-12-2017
 * Hash
 * @author xehoth
 */
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/hash_policy.hpp>

namespace {

inline char read() {
    static const int IN_LEN = 1 << 18 | 1;
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
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[21], cnt;
    if (x == 0) {
        print('0');
    } else {
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    ~InputOutputStream() { flush(); }

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

typedef unsigned long long ulong;

const int MAXN = 600000 + 9;
const int MAX_LEN = 3000000 + 9;
const int MAXQ = 13;

const int HASH_BASE = 31;

ulong powVal[MAXN];
char s[MAXN];
ulong h[MAXN];
int lenS, q, mask, maxLen;
int pos[MAX_LEN], posCnt;

std::vector<int> lenType;

typedef __gnu_pbds::gp_hash_table<ulong, int> HashMap;

HashMap map[MAXQ];

inline void decode(char *s, const int len, register int mask) {
    register char t;
    for (register int i = 0; i < len; i++) {
        mask = (mask * 131 + i) % len;
        t = s[i], s[i] = s[mask], s[mask] = t;
    }
}

inline void init(const int n) {
    powVal[0] = 1;
    for (register int i = 1; i <= n; i++) powVal[i] = powVal[i - 1] * HASH_BASE;
}

inline void solve() {
    io >> q;
    lenS = read(s + 1);
    init(lenS);
    maxLen = lenS;
    for (register int i = 1; i <= lenS; i++) h[i] = h[i - 1] * HASH_BASE + s[i];
    char cmd[7];
    for (; q--;) {
        read(cmd);
        switch (cmd[0]) {
            case 'A': {
                register int len = read(s);
                decode(s, len, mask);
                for (register int i = 0; i < len; i++) {
                    lenS++;
                    h[lenS] = h[lenS - 1] * HASH_BASE + s[i];
                    for (register int j = 0, l; j < lenType.size(); j++) {
                        if ((l = lenType[j]) > lenS) continue;
                        map[pos[l]][h[lenS] - h[lenS - l] * powVal[l]]++;
                    }
                }
                if (maxLen < lenS) {
                    for (register int i = maxLen + 1; i <= lenS; i++)
                        powVal[i] = powVal[i - 1] * HASH_BASE;
                    maxLen = lenS;
                }
                break;
            }
            case 'D': {
                register int len;
                io >> len;
                for (register int i = 1; i <= len; i++) {
                    for (register int j = 0, l; j < lenType.size(); j++) {
                        if ((l = lenType[j]) > lenS) continue;
                        map[pos[l]][h[lenS] - h[lenS - l] * powVal[l]]--;
                    }
                    lenS--;
                }
                break;
            }
            case 'Q': {
                register int len = read(s);
                decode(s, len, mask);
                register ulong hashVal = 0;
                for (register int i = 0; i < len; i++)
                    hashVal = hashVal * HASH_BASE + s[i];
                if (pos[len]) {
                    register int ans = map[pos[len]][hashVal];
                    mask ^= ans;
                    io << ans << '\n';
                } else {
                    pos[len] = ++posCnt;
                    lenType.push_back(len);
                    for (register int i = len; i <= lenS; i++)
                        map[posCnt][h[i] - h[i - len] * powVal[len]]++;
                    register int ans = map[posCnt][hashVal];
                    mask ^= ans;
                    io << ans << '\n';
                }
                break;
            }
        }
    }
}
}  // namespace

int main() {
    solve();
    return 0;
}
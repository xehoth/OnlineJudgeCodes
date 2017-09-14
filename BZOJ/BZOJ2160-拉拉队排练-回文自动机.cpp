/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2160」拉拉队排练 14-09-2017
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

using IO::io;
#define long long long

const int MAXN = 1000005;
const int MAX_SIGMA = 26;
const int MOD = 19930726;

struct Node {
    int c[MAX_SIGMA], len, fail;
    long cnt;
};

inline long modPow(long a, long b) {
    register long ret = 1;
    for (; b; b >>= 1, a = a * a % MOD) (b & 1) ? ret = ret * a % MOD : 0;
    return ret;
}

struct PalindromicTree {
    static const int odd = 1;
    static const int even = 0;

    Node d[MAXN + 1];
    char s[MAXN + 1];
    int size, cur, last;

    PalindromicTree() {
        s[size = 0] = -1, cur = 2;
        d[even].fail = d[odd].fail = odd, d[odd].len = -1;
    }

    inline int getFail(int p) {
        while (s[size - d[p].len - 1] != s[size]) p = d[p].fail;
        return p;
    }

    inline void extend(char c) {
        s[++size] = c;
        register int p = getFail(last);
        if (!d[p].c[c]) {
            register int np = cur++;
            d[np].len = d[p].len + 2;
            d[np].fail = d[getFail(d[p].fail)].c[c];
            d[p].c[c] = np;
        }
        last = d[p].c[c], d[last].cnt++;
    }

    inline void count() {
        register int p = cur - 1;
        for (; p; p--) d[d[p].fail].cnt += d[p].cnt;
        d[d[p].fail].cnt += d[p].cnt;
    }

    inline void solve() {
        register int n;
        register long k;
        static char buf[MAXN + 1];
        io >> n >> k >> buf;
        for (register int i = 0; i < n; i++) extend(buf[i] - 'a');
        count();
        static std::vector<std::pair<int, long> > vec;
        vec.reserve(cur);
        register long sumCnt = 0;
        for (register int p = 0; p != cur; p++) {
            if (d[p].len == 0 || d[p].len == -1) continue;
            if (d[p].len % 2 == 0) continue;
            vec.push_back(std::make_pair(d[p].len, d[p].cnt));
            sumCnt += d[p].cnt;
        }
        if (k > sumCnt) {
            io << "-1";
            return;
        }
        std::sort(vec.begin(), vec.end());
        register long ans = 1;
        for (register int i = vec.size() - 1; ~i; i--) {
            if (k > vec[i].second) {
                ans = ans * modPow(vec[i].first, vec[i].second) % MOD;
                k -= vec[i].second;
            } else {
                ans = ans * modPow(vec[i].first, k) % MOD;
                k = 0;
                break;
            }
        }
        io << ans;
    }
} task;
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    task.solve();
    return 0;
}
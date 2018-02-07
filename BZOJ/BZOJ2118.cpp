/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2118」墨墨的等式 12-10-2017
 * 最短路
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
}  // namespace IO

namespace {

using IO::io;

#define long long long
const int MAXN = 12, MAX_NODE = 500000 + 3;
const long INF = 0x3f3f3f3f3f3f3f3fll;
typedef std::pair<long, int> Pair;
int n, a[MAXN], mn;
long L, R;

inline bool tension(long &a, const long &b) {
    return b < a ? a = b, true : false;
}

long dis[MAX_NODE];
inline void dijkstra() {
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair> > pq;
    memset(dis, 0x3f, sizeof dis);
    dis[0] = 0;
    pq.push(Pair(0, 0));
    while (!pq.empty()) {
        Pair now = pq.top();
        pq.pop();
        int u = now.second;
        if (now.first > dis[u]) continue;
        for (int i = 0; i < n; ++i)
            if (tension(dis[(u + a[i]) % mn], dis[u] + a[i]))
                pq.push(Pair(dis[(u + a[i]) % mn], (u + a[i]) % mn));
    }
}

inline long F(const long m) {
    long ans = 0;
    for (int i = 0; i < mn; ++i)
        if (dis[i] <= m) ans += (m - dis[i]) / mn + 1;
    return ans;
}

inline void solve() {
    io >> n >> L >> R;
    for (register int i = 0; i < n; i++) {
        io >> a[i];
        if (a[i] == 0) n--, i--;
    }
    mn = *std::min_element(a, a + n);
    dijkstra();
    io << F(R) - F(L - 1);
}
}  // namespace

int main() {
    solve();
    return 0;
}
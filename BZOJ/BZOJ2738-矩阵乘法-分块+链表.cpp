/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2738」矩阵乘法 28-12-2017
 * 分块 + 链表
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

const int OUT_LEN = 1 << 18 | 1;
char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static char buf[21], cnt;
    if (x != 0) {
        (x < 0) && (print('-'), x = -x);
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print(buf[cnt--]);
    } else {
        print('0');
    }
}

struct InputOutputStream {
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 500 + 9;
const int MAXQ = 60000;

int n, m;

struct Node {
    int x, y, v;
    inline bool operator<(const Node &p) const { return v < p.v; }
} d[500 * 500 + 9];

struct Query {
    int x1, y1, x2, y2, k, ans;
} q[MAXQ];

int cnt[MAXN][MAXN], sum[MAXN][MAXN], tot;

std::list<int> list;

inline void solve() {
    io >> n >> m;
    for (register int i = 1; i <= n; i++) {
        for (register int j = 1; j <= n; j++) {
            tot++;
            io >> d[tot].v;
            d[tot].x = i;
            d[tot].y = j;
        }
    }
    std::sort(d + 1, d + tot + 1);
    for (register int i = 1; i <= m; i++) {
        io >> q[i].x1 >> q[i].y1 >> q[i].x2 >> q[i].y2 >> q[i].k;
    }
    list.resize(m);
    {
        register int i = 1;
        for (register std::list<int>::iterator it = list.begin();
             it != list.end(); ++it) {
            *it = i;
            i++;
        }
    }
    for (register int k = 1; k <= n; k++) {
        for (register int i = 1, t; i <= n; i++) {
            t = (k - 1) * n + i;
            cnt[d[t].x][d[t].y]++;
        }
        for (register int i = 1; i <= n; i++) {
            for (register int j = 1; j <= n; j++) {
                sum[i][j] = cnt[i][j] + sum[i - 1][j] + sum[i][j - 1] -
                            sum[i - 1][j - 1];
            }
        }
        for (register std::list<int>::iterator it = list.begin();
             it != list.end();) {
            register int x1 = q[*it].x1 - 1, x2 = q[*it].x2;
            register int y1 = q[*it].y1 - 1, y2 = q[*it].y2;
            register int tmp =
                sum[x2][y2] - sum[x1][y2] - sum[x2][y1] + sum[x1][y1];
            if ((tmp -= q[*it].k) < 0) {
                ++it;
                continue;
            }
            for (register int x = k * n; x; x--) {
                if (d[x].x > x1 && d[x].x <= x2 && d[x].y > y1 &&
                    d[x].y <= y2 && !tmp--) {
                    q[*it].ans = d[x].v;
                    break;
                }
            }
            it = list.erase(it);
        }
    }
    for (register int i = 1; i <= m; i++) io << q[i].ans << '\n';
}
}  // namespace

int main() {
    solve();
    return 0;
}
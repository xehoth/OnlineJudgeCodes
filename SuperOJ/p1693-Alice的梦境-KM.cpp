/**
 * Copyright (c) 2016-2018, xehoth
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * 「SuperOJ 1693」Alice的梦境 08-02-2018
 * KM
 * @author xehoth
 */
#include <bits/stdc++.h>

struct InputOutputStream {
    enum { SIZE = 1 << 18 | 1 };
    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
               s == t ? -1 : *s++;
    }

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

    inline void print(char c) {
        (oh == obuf + SIZE) && (fwrite(obuf, 1, SIZE, stdout), oh = obuf);
        *oh++ = c;
    }

    template <typename T>
    inline void print(T x) {
        static int buf[21], cnt;
        if (x != 0) {
            (x < 0) && (print('-'), x = -x);
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
            while (cnt) print((char)buf[cnt--]);
        } else {
            print('0');
        }
    }

    inline void print(const char *s) {
        for (; *s; s++) print(*s);
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 505;
const int MAX_NL = 505;
const int MAX_NR = 505;
const long long INF = 0x3f3f3f3f3f3f3f3fll;
const long long SHIFT = 1000000ll;

template <class T>
inline bool relax(T &a, const T &b) {
    return b > a ? (a = b, true) : false;
}

template <class T>
inline bool tense(T &a, const T &b) {
    return b < a ? (a = b, true) : false;
}

struct KuhnMunkres {
    long long map[MAX_NL][MAX_NR], labL[MAX_NL], labR[MAX_NR], slackR[MAX_NR];
    int n;
    int mateL[MAX_NL], mateR[MAX_NR], faR[MAX_NR], qSize, q[MAX_NL];
    bool bookL[MAX_NL], bookR[MAX_NR];

    inline void augment(int v) {
        for (register int u = faR[v]; v > 0;) {
            mateR[v] = (u = faR[v]);
            std::swap(v, mateL[u]);
        }
    }

    inline bool isOnFoundEdge(int v) {
        if (mateR[v]) {
            q[++qSize] = mateR[v];
            bookR[v] = true;
            bookL[mateR[v]] = true;
            return false;
        } else {
            augment(v);
            return true;
        }
    }

    inline void match(int sv) {
        memset(bookL, 0, sizeof(bool) * (n + 1));
        memset(bookR, 0, sizeof(bool) * (n + 1));
        memset(slackR, 0x3f, sizeof(long long) * (n + 1));
        memset(faR, 0, sizeof(int) * (n + 1));
        bookL[q[qSize = 1] = sv] = true;
        for (;;) {
            for (register int i = 1; i <= qSize; ++i) {
                register int u = q[i];
                for (register int v = 1; v <= n; v++) {
                    register long long d = labL[u] + labR[v] - map[u][v];
                    if (bookR[v] || d > slackR[v]) continue;
                    faR[v] = u;
                    if (d > 0)
                        slackR[v] = d;
                    else if (isOnFoundEdge(v))
                        return;
                }
            }
            register int nv = 0;
            register long long delta = INF;
            for (register int v = 1; v <= n; v++)
                if (!bookR[v] && tense(delta, slackR[v])) nv = v;
            for (register int u = 1; u <= n; u++)
                if (bookL[u]) labL[u] -= delta;
            for (register int v = 1; v <= n; v++) {
                if (bookR[v])
                    labR[v] += delta;
                else
                    slackR[v] -= delta;
            }
            qSize = 0;
            if (isOnFoundEdge(nv)) return;
        }
    }

    inline void addEdge(const int u, const int v, const long long w) {
        map[u][v] = w;
        relax(labL[u], w);
    }

    inline int km(const int nL, const int nR) {
        this->n = std::max(nL, nR);
        for (register int u = 1; u <= nL; u++) match(u);
        register long long ret = 0;
        for (register int u = 1; u <= nL; u++) ret += labL[u];
        for (register int v = 1; v <= nR; v++) ret += labR[v];
        return ret;
    }

    inline void init(const int n) {
        for (register int i = 0; i <= n; i++) {
            for (register int j = 0; j <= n; j++) {
                map[i][j] = -INF;
            }
        }
        memset(mateL, 0, sizeof(int) * (n + 1));
        memset(mateR, 0, sizeof(int) * (n + 1));
        memset(labL, 0, sizeof(long long) * (n + 1));
        memset(labR, 0, sizeof(long long) * (n + 1));
    }
} km;

char map[MAXN][MAXN];

struct L {
    int x, y, limit, w, r, f;
} ld[MAXN];

struct R {
    int x, y, p;
} rd[MAXN];

inline bool check(const L &a, const R &b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) <= a.r * a.r &&
           a.limit >= b.p;
}

inline void solve() {
    int n, m, sum = 0;
    io >> n >> m;
    for (register int i = 1; i <= n; i++) io >> rd[i].x >> rd[i].y >> rd[i].p;
    for (register int i = 1; i <= m; i++) {
        io >> ld[i].x >> ld[i].y >> ld[i].limit >> ld[i].w >> ld[i].r >>
            ld[i].f;
        if (ld[i].f) sum += ld[i].w;
    }
    if (n > m) {
        io << "-1\n";
        return;
    }
    km.init(std::max(n, m));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (check(ld[i], rd[j])) {
                if (ld[i].f)
                    km.addEdge(j, i, ld[i].w * SHIFT - i);
                else
                    km.addEdge(j, i, -ld[i].w * SHIFT - i);
            }
        }
    }

    km.km(n, m);
    int cnt = 0;
    for (int i = 1; i <= n; i++) {
        if (km.mateL[i]) {
            if (km.map[i][km.mateL[i]] == -INF) {
                io << "-1\n";
                return;
            }
            cnt++;
        }
    }
    if (cnt != n) {
        io << "-1\n";
    } else {
        sum = 0;
        for (register int i = 1; i <= m; i++) {
            if (km.mateR[i]) {
                if (!ld[i].f) sum += ld[i].w;
            } else {
                if (ld[i].f) sum += ld[i].w;
            }
        }
        io << sum << '\n';
        for (register int i = 1; i <= m; i++) {
            if (km.mateR[i]) io << i << ' ';
        }
        io << '\n';
    }
}

int main() {
    // freopen("dream.in", "r", stdin);
    // freopen("dream.out", "w", stdout);
    int T;
    io >> T;
    while (T--) solve();
    return 0;
}

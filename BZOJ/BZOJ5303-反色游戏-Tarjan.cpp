/**
 * Copyright (c) 2018, xehoth
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
 * 「BZOJ 5303」反色游戏 30-05-2018
 * Tarjan
 * @author xehoth
 */
#include <bits/stdc++.h>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    template <typename T>
    void read(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        if (iosig) x = -x;
    }

    int read(char *buf) {
        int s = 0;
        char c;
        while (c = read(), isspace(c) && c != -1)
            ;
        if (c == -1) {
            *buf = '\0';
            return -1;
        }
        do
            buf[s++] = c;
        while (c = read(), !isspace(c) && c != -1);
        buf[s] = '\0';
        return s;
    }

    template <typename T>
    InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    void print(char c) {
        if (oh == obuf + SIZE) {
            fwrite(obuf, 1, SIZE, stdout);
            oh = obuf;
        }
        *oh++ = c;
    }

    template <typename T>
    void print(T x) {
        static int buf[21], cnt;
        if (x != 0) {
            if (x < 0) {
                print('-');
                x = -x;
            }
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
            while (cnt) print((char)buf[cnt--]);
        } else {
            print('0');
        }
    }

    void print(const char *s) {
        for (; *s; s++) print(*s);
    }

    template <typename T>
    InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 100000 + 5;
const int MOD = 1e9 + 7;

int n, m, a[MAXN], pw[MAXN];

char str[MAXN];

int tot, head[MAXN], deg[MAXN];

struct Edge {
    int v, next;
} e[MAXN << 1];

inline void addEdge(int u, int v) {
    e[++tot] = (Edge){v, head[u]};
    head[u] = tot;
    e[++tot] = (Edge){u, head[v]};
    head[v] = tot;
    deg[u]++;
    deg[v]++;
}

int idx, dfn[MAXN], low[MAXN], fa[MAXN], w[MAXN];

int f[MAXN], g[MAXN], d[MAXN];

inline void tarjan(int u, int pre) {
    dfn[u] = low[u] = ++idx;
    fa[u] = pre;
    w[u] = a[u];
    for (int i = head[u]; i; i = e[i].next) {
        int v = e[i].v;
        if (!dfn[v]) {
            tarjan(v, pre);
            low[u] = std::min(low[u], low[v]);
            w[u] += w[v];
            if (low[v] >= dfn[u]) {
                d[u] |= (w[v] & 1);
                f[u]++;
                g[u] += w[v];
            }
        } else {
            low[u] = std::min(low[u], dfn[v]);
        }
    }

    if (u == pre) f[u]--;
}

inline void init() {
    tot = idx = 0;
    for (int i = 1; i <= n; i++) head[i] = dfn[i] = low[i] = f[i] = d[i] = deg[i] = g[i] = 0;
}

inline void solve() {
    init();
    io >> n >> m;
    for (int i = 1, u, v; i <= m; i++) {
        io >> u >> v;
        addEdge(u, v);
    }

    io >> str;
    for (int i = 1; i <= n; i++) a[i] = str[i - 1] - '0';

    int ans = m - n, flag = 0;
    for (int i = 1; i <= n; i++) {
        if (!dfn[i]) {
            tarjan(i, i);
            ans++;
            flag += (w[i] & 1);
        }
    }
    io << (flag ? 0 : pw[ans]) << ' ';

    for (int i = 1; i <= n; i++) {
        if (d[i])
            io << "0 ";
        else if (flag - (w[fa[i]] & 1))
            io << "0 ";
        else if ((w[fa[i]] - a[i] - g[i]) & 1)
            io << "0 ";
        else
            io << pw[ans - deg[i] + 1 + f[i]] << ' ';
    }
    io << '\n';
}

int main() {
    pw[0] = 1;
    for (int i = 1; i < MAXN; i++) pw[i] = pw[i - 1] * 2 % MOD;
    int T;
    io >> T;
    while (T--) solve();
    return 0;
}

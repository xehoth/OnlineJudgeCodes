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
 * 「BZOJ 4449」Distance on Triangulation 25-03-2018
 * 分治
 * @author xehoth
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <vector>

char pool[1024 * 1024 * 99];

inline void *operator new(size_t size) {
    static char *s = pool;
    char *t = s;
    s += size;
    return t;
}

inline void operator delete(void *) {}

inline void operator delete(void *, size_t) {}

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
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

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 200000 + 3;

std::vector<int> g[MAXN];
int pos[MAXN], que[MAXN];

struct Node {
    std::vector<int> v;
    std::vector<std::pair<int, int> > e;
    int L, R;
    std::vector<int> disL, disR;
    Node *lc, *rc;

    void bfs(int start, std::vector<int> &d) {
        int cnt = v.size();
        for (int i = 0; i < cnt; i++) g[v[i]].clear();
        for (int i = 0; i < cnt - 3; i++) {
            int x = e[i].first;
            int y = e[i].second;
            g[x].push_back(y);
            g[y].push_back(x);
        }
        for (int i = 0; i < cnt - 1; i++) {
            g[v[i]].push_back(v[i + 1]);
            g[v[i + 1]].push_back(v[i]);
        }
        g[v[cnt - 1]].push_back(v[0]);
        g[v[0]].push_back(v[cnt - 1]);
        d.assign(cnt, -1);
        int qh = 0, qt = 1;
        d[pos[que[0] = start]] = 0;
        for (int u; qh < qt;) {
            for (int i = 0, sz = g[u = que[qh++]].size(), v; i < sz; i++) {
                if (d[pos[v = g[u][i]]] == -1) {
                    d[pos[v]] = d[pos[u]] + 1;
                    que[qt++] = v;
                }
            }
        }
    }

    void build() {
        if (v.size() <= 3) return;
        int cnt = v.size();
        for (int i = 0; i < cnt; i++) pos[v[i]] = i;
        int best = -1;
        L = R = -1;
        for (int i = 0; i < cnt - 3; i++) {
            int x = e[i].first;
            int y = e[i].second;
            int diff = abs(pos[x] - pos[y]);
            diff = std::min(diff, cnt - diff);
            if (diff > best) {
                best = diff;
                L = std::min(x, y);
                R = std::max(x, y);
            }
        }
        bfs(L, disL);
        bfs(R, disR);
        lc = new Node();
        rc = new Node();
        for (int i = pos[L]; i <= pos[R]; i++) lc->v.push_back(v[i]);
        for (int i = 0; i <= pos[L]; i++) rc->v.push_back(v[i]);
        for (int i = pos[R]; i < cnt; i++) rc->v.push_back(v[i]);
        for (int i = 0, x, y; i < cnt - 3; i++) {
            x = e[i].first;
            y = e[i].second;
            if ((L < x && x < R) || (L < y && y < R)) lc->e.push_back(e[i]);

            if ((L > x || x > R) || (L > y || y > R)) rc->e.push_back(e[i]);
        }
        lc->build();
        rc->build();
    }

    int distance(int x, int y) {
        if (x == y) return 0;
        if (v.size() <= 3) return 1;
        if ((L < x && x < R) && (L < y && y < R)) return lc->distance(x, y);
        if ((L > x || x > R) && (L > y || y > R)) return rc->distance(x, y);
        int px = std::lower_bound(v.begin(), v.end(), x) - v.begin();
        int py = std::lower_bound(v.begin(), v.end(), y) - v.begin();
        return std::min(disL[px] + disL[py], disR[px] + disR[py]);
    }
};

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    int n, q;
    io >> n;
    Node *root = new Node();
    root->v.resize(n);
    for (int i = 0; i < n; i++) {
        root->v[i] = i;
    }
    for (int i = 0, u, v; i < n - 3; i++) {
        io >> u >> v;
        u--;
        v--;
        root->e.push_back(std::make_pair(u, v));
    }
    root->build();
    io >> q;
    for (int u, v; q--;) {
        io >> u >> v;
        u--;
        v--;
        io << root->distance(u, v) << '\n';
    }
    return 0;
}
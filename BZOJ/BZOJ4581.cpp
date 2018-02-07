/*******************************************************************************
 * Copyright (c) 2016-2017, xehoth
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name xehoth, nor the names of its contributors may be used
 *       to endorse or promote products derived from this software without
 *       specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY XEHOTH AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEHOTH AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
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
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace Task {

const int MAXN = 50010;

struct Node {
    int x, y;
    bool *vis;
} a[MAXN], b[20];

bool vis[MAXN];

int n, tot, ans = INT_MAX;

inline bool cmpX(const Node &a, const Node &b) { return a.x < b.x; }

inline bool cmpY(const Node &a, const Node &b) { return a.y < b.y; }

inline void update(int x, int y, int z) {
    register int maxX = 0, maxY = 0, minX = INT_MAX, minY = INT_MAX;
    for (register int i = 1; i <= tot; i++) {
        if (i == x || i == y || i == z) continue;
        maxX = std::max(maxX, b[i].x), minX = std::min(minX, b[i].x);
        maxY = std::max(maxY, b[i].y), minY = std::min(minY, b[i].y);
    }
    ans = std::min(ans, (maxX - minX) * (maxY - minY));
}

inline void add() {
    for (register int i = 1; i <= 4; i++)
        if (!*a[i].vis) *a[i].vis = true, b[++tot] = a[i];
    for (register int i = n; i >= n - 3; i--)
        if (!*a[i].vis) *a[i].vis = true, b[++tot] = a[i];
}

inline void solve() {
    using namespace IO;
    read(n);
    for (register int i = 1; i <= n; i++)
        read(a[i].x), read(a[i].y), a[i].vis = vis + i;
    std::sort(a + 1, a + n + 1, cmpX), add();
    std::sort(a + 1, a + n + 1, cmpY), add();
    for (register int i = 1; i <= tot; i++) {
        for (register int j = 1; j <= tot; j++) {
            if (i == j) continue;
            for (register int k = 1; k <= tot; k++) {
                if (i != k && j != k) update(i, j, k);
            }
        }
    }
    print(ans);
}
}  // namespace Task

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#else
// freopen("reduce.in", "r", stdin);
// freopen("reduce.out", "w", stdout);
#endif
    Task::solve();
    IO::flush();
    return 0;
}

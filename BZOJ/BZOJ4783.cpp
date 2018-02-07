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
#ifndef DBG

#define NDEBUG

#endif

#include <bits/stdc++.h>
/**
 * T3 28-05-2017
 * fourWaysFloodFill + 计算几何
 * O(n ^ 6 + S)
 * @author xehoth
 */
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

inline int read(char *buf) {
    register int s = 0;
    register char c;
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

inline void read(double &x) {
    static char buf[30];
    read(buf), x = atof(buf);
}

template <typename T1, typename T2>
inline void read(T1 &a, T2 &b) {
    read(a), read(b);
}

template <typename T1, typename T2, typename T3>
inline void read(T1 &a, T2 &b, T3 &c) {
    read(a), read(b), read(c);
}

template <typename T1, typename T2, typename T3, typename T4>
inline void read(T1 &a, T2 &b, T3 &c, T4 &d) {
    read(a), read(b), read(c), read(d);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline void read(T1 &a, T2 &b, T3 &c, T4 &d, T5 &e) {
    read(a), read(b), read(c), read(d), read(e);
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

template <typename T>
inline void println(T x) {
    print(x), print('\n');
}

template <typename T1, typename T2>
inline void print(T1 a, T2 b) {
    print(a), print(b);
}

template <typename T1, typename T2, typename T3>
inline void print(T1 a, T2 b, T3 c) {
    print(a), print(b), print(c);
}

template <typename T1, typename T2, typename T3, typename T4>
inline void print(T1 a, T2 b, T3 c, T4 d) {
    print(a), print(b), print(c), print(d);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline void print(T1 a, T2 b, T3 c, T4 d, T5 e) {
    print(a), print(b), print(c), print(d), print(e);
}

template <typename T1, typename T2>
inline void println(T1 a, T2 b) {
    print(a), print(b), print('\n');
}

template <typename T1, typename T2, typename T3>
inline void println(T1 a, T2 b, T3 c) {
    print(a), print(b), print(c), print('\n');
}

template <typename T1, typename T2, typename T3, typename T4>
inline void println(T1 a, T2 b, T3 c, T4 d) {
    print(a), print(b), print(c), print(d), print('\n');
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline void println(T1 a, T2 b, T3 c, T4 d, T5 e) {
    print(a), print(b), print(c), print(d), print(e), print('\n');
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace Task {

const int MAXN = 21;

int n;
char img[MAXN][MAXN];

struct Rectangle {
    // 左上右下
    int x1, y1, x2, y2;

    Rectangle(const int x1, const int y1, const int x2, const int y2)
        : x1(x1), y1(y1), x2(x2), y2(y2) {}
};

std::vector<Rectangle> rec;

bool vis[MAXN][MAXN];

// O(S)
inline void fourWaysFloodFill(int i, int j, int fillColor, int x1, int y1,
                              int x2, int y2) {
    vis[i][j] = true;
    if (i > x1 && img[i - 1][j] - 'A' == fillColor && !vis[i - 1][j])
        fourWaysFloodFill(i - 1, j, fillColor, x1, y1, x2, y2);
    if (i < x2 && img[i + 1][j] - 'A' == fillColor && !vis[i + 1][j])
        fourWaysFloodFill(i + 1, j, fillColor, x1, y1, x2, y2);
    if (j > y1 && img[i][j - 1] - 'A' == fillColor && !vis[i][j - 1])
        fourWaysFloodFill(i, j - 1, fillColor, x1, y1, x2, y2);
    if (j < y2 && img[i][j + 1] - 'A' == fillColor && !vis[i][j + 1])
        fourWaysFloodFill(i, j + 1, fillColor, x1, y1, x2, y2);
}

inline bool isPotentialCowLocation(int x1, int y1, int x2, int y2) {
    register int num = 0;
    int cnt[26] = {0};
    for (register int i = x1; i <= x2; i++)
        memset(vis[i] + y1, 0, sizeof(bool) * (y2 - y1 + 1));
    for (register int i = x1; i <= x2; i++) {
        for (register int j = y1; j <= y2; j++) {
            if (!vis[i][j]) {
                register int c = img[i][j] - 'A';
                if (cnt[c] == 0) num++;
                cnt[c]++;
                fourWaysFloodFill(i, j, c, x1, y1, x2, y2);
            }
        }
    }
    if (num != 2) return false;
    register bool first = false, flag = false;
    for (register int i = 0; i < 26; i++) {
        if (cnt[i] == 1) first = true;
        if (cnt[i] > 1) flag = true;
    }
    return first && flag;
}

inline bool isRectangleInRectangle(int x, int y) {
    return rec[x].x1 >= rec[y].x1 && rec[x].x2 <= rec[y].x2 &&
           rec[x].y1 >= rec[y].y1 && rec[x].y2 <= rec[y].y2;
}

inline bool isMaxRectangle(int x) {
    for (register int i = 0; i < rec.size(); i++)
        if (i != x && isRectangleInRectangle(x, i)) return false;
    return true;
}

inline void solve() {
    using namespace IO;
    read(n);
    for (register int i = 0; i < n; i++) read(img[i]);
    rec.reserve(1000);
    // O(n^6 + S)
    for (register int x1 = 0; x1 < n; x1++)
        for (register int y1 = 0; y1 < n; y1++)
            for (register int x2 = x1; x2 < n; x2++)
                for (register int y2 = y1; y2 < n; y2++)
                    if (isPotentialCowLocation(x1, y1, x2, y2))
                        rec.push_back(Rectangle(x1, y1, x2, y2));
    register int ans = 0;
    for (register int i = 0; i < rec.size(); i++)
        if (isMaxRectangle(i)) ans++;
    print(ans);
}
}  // namespace Task

int main() {
#ifndef ONLINE_JUDGE
#ifdef DBG
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    freopen("debug.log", "w", stderr);
#else
    freopen("where.in", "r", stdin);
    freopen("where.out", "w", stdout);
#endif
#endif
    Task::solve();
    IO::flush();
    return 0;
}
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
/**
 * 「SDOI2017」相关分析 10-05-2017
 * @author xehoth
 */
namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <class T>
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

inline void print(double x) {
    static char buf[30];
    sprintf(buf, "%.10f", x);
    for (char *s = buf; *s; s++) print(*s);
    print('\n');
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace SegmentTree {

#define long long long

const int MAXN = 100005;

int n, m;
int initX[MAXN], initY[MAXN];

inline long getSum(int n) { return n * (n + 1ll) * ((long)n << 1 | 1) / 6; }

struct Node {
    int l, r;
    long sumX, sumY;
    double sumA, sumB;

    Node() {}

    Node(const int l, const int r, const long sumX, const long sumY,
         const double sumA, const double sumB)
        : l(l), r(r), sumX(sumX), sumY(sumY), sumA(sumA), sumB(sumB) {}

    inline double calc() const {
        register int len = r - l + 1;
        return (sumA - (double)sumX / len * sumY) /
               (sumB - (double)sumX / len * sumX);
    }

    inline void init(const int &d, const int &x, const int &y) {
        l = r = d, sumX = x, sumA = (double)x * y, sumY = y,
        sumB = (double)x * x;
    }

    inline void add(long s, long t) {
        register int len = r - l + 1;
        sumA += (double)len * s * t + (double)t * sumX + (double)s * sumY,
            sumB += (double)len * s * s + 2.0 * s * sumX, sumX += len * s,
            sumY += len * t;
    }

    inline void cover(int s, int t) {
        register int len = r - l + 1;
        register long sumO = (long)(l + r) * len >> 1;
        register long sumT = getSum(r) - getSum(l - 1);
        sumA = (double)len * s * t + sumT + (double)sumO * (s + t);
        sumB = (double)len * s * s + sumT + (double)sumO * 2.0 * s;
        sumX = sumO + (long)len * s, sumY = sumO + (long)len * t;
    }

    friend inline Node operator+(const Node &lhs, const Node &rhs) {
        return Node(lhs.l, rhs.r, lhs.sumX + rhs.sumX, lhs.sumY + rhs.sumY,
                    lhs.sumA + rhs.sumA, lhs.sumB + rhs.sumB);
    }
} d[MAXN << 2];

long deltaX[MAXN << 2], deltaY[MAXN << 2];
int coverX[MAXN << 2], coverY[MAXN << 2];

inline void maintain(int p) { d[p] = d[p << 1] + d[p << 1 | 1]; }

inline void addTag(int p, long dx, long dy) {
    deltaX[p] += dx, deltaY[p] += dy, d[p].add(dx, dy);
}

inline void coverTag(int p, int cx, int cy) {
    deltaX[p] = deltaY[p] = 0, coverX[p] = cx, coverY[p] = cy;
    d[p].cover(cx, cy);
}

inline void pushDown(int p) {
    if (coverX[p] != INT_MAX) {
        coverTag(p << 1, coverX[p], coverY[p]);
        coverTag(p << 1 | 1, coverX[p], coverY[p]);
        coverX[p] = coverY[p] = INT_MAX;
    }

    if (deltaX[p] || deltaY[p]) {
        addTag(p << 1, deltaX[p], deltaY[p]);
        addTag(p << 1 | 1, deltaX[p], deltaY[p]);
        deltaX[p] = deltaY[p] = 0;
    }
}

inline void build(int p, int l, int r) {
    coverX[p] = coverY[p] = INT_MAX;
    if (l == r) {
        d[p].init(l, initX[l], initY[l]);
    } else {
        register int mid = l + r >> 1;
        build(p << 1, l, mid), build(p << 1 | 1, mid + 1, r);
        maintain(p);
    }
}

inline void add(int p, int ql, int qr, int dx, int dy) {
    register int l = d[p].l, r = d[p].r;
    if (ql <= l && qr >= r) {
        addTag(p, dx, dy);
        return;
    }
    pushDown(p);
    register int mid = l + r >> 1;
    if (ql <= mid) add(p << 1, ql, qr, dx, dy);
    if (qr > mid) add(p << 1 | 1, ql, qr, dx, dy);
    maintain(p);
}

inline void cover(int p, int ql, int qr, int cx, int cy) {
    register int l = d[p].l, r = d[p].r;
    if (ql <= l && qr >= r) {
        coverTag(p, cx, cy);
        return;
    }
    pushDown(p);
    register int mid = l + r >> 1;
    if (ql <= mid) cover(p << 1, ql, qr, cx, cy);
    if (qr > mid) cover(p << 1 | 1, ql, qr, cx, cy);
    maintain(p);
}

inline Node query(int p, int ql, int qr) {
    register int l = d[p].l, r = d[p].r;
    if (ql == l && qr == r) return d[p];
    pushDown(p);
    register int mid = l + r >> 1;
    if (qr <= mid)
        return query(p << 1 | 0, ql, qr);
    else if (ql > mid)
        return query(p << 1 | 1, ql, qr);
    else
        return query(p << 1, ql, mid) + query(p << 1 | 1, mid + 1, qr);
}

inline void solve() {
    using namespace IO;
    read(n), read(m);
    for (register int i = 1; i <= n; i++) read(initX[i]);
    for (register int i = 1; i <= n; i++) read(initY[i]);
    build(1, 1, n);
    while (m--) {
        register int cmd, l, r, s, t;
        read(cmd), read(l), read(r);
        switch (cmd) {
            case 1:
                print(query(1, l, r).calc());
                break;
            case 2:
                read(s), read(t), add(1, l, r, s, t);
                break;
            case 3:
                read(s), read(t), cover(1, l, r, s, t);
                break;
        }
    }
}
}

int main() {
    // freopen("relative.in", "r", stdin);
    // freopen("relative.out", "w", stdout);
    SegmentTree::solve();
    IO::flush();
    return 0;
}

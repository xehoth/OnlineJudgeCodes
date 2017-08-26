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
 * 「CQOI 2016」K远点对 18-05-2017
 * kd-tree
 * @author xehoth
 */
namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template<class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read())
        x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template<class T>
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

inline void flush() {
    fwrite(obuf, 1, oh - obuf, stdout);
}
}

static const int MAXN = 100010;

static struct Point {
    int x, y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}
} p[MAXN];

bool flag;

#define long long long

template<typename T>
static inline T square(const T &x) {
    return x * x;
}

static inline bool cmp(const Point &p1, const Point &p2) {
    return flag ? (p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x)) :
                  (p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y));
}

static inline bool getSplit(int l, int r) {
    register double vx = 0, vy = 0;
    register double ax = 0, ay = 0;
    for (register int i = l; i <= r; i++) ax += p[i].x, ay += p[i].y;
    ax /= r - l + 1, ay /= r - l + 1;
    for (register int i = l; i <= r; i++)
        vx += square(p[i].x - ax), vy += square(p[i].y - ay);
    return vx < vy;
}

class PriorityQueue : 
    public std::priority_queue<long, 
        std::vector<long>, std::greater<long> > {
    
public:
    
    inline void resize(const int n) {
        c.resize(n);
    }
} a;

PriorityQueue q;

struct Node {
    Node *c[2];
    int kind;
    Point p, r1, r2;

    Node(int kind = 0) : kind(0), c() {}

    Node(int kind, const Point &p) : kind(kind), p(p), r1(p), r2(p), c() {}

    inline void *operator new(size_t) {
        static Node pool[MAXN], *cur = pool;
        return cur++;
    }

    inline void maintain() {
        if (c[0]) {
            r1.x = std::min(c[0]->r1.x, r1.x);
            r1.y = std::min(c[0]->r1.y, r1.y);
            r2.x = std::max(c[0]->r2.x, r2.x);
            r2.y = std::max(c[0]->r2.y, r2.y);
        }
        if (c[1]) {
            r1.x = std::min(c[1]->r1.x, r1.x);
            r1.y = std::min(c[1]->r1.y, r1.y);
            r2.x = std::max(c[1]->r2.x, r2.x);
            r2.y = std::max(c[1]->r2.y, r2.y);
        }
    }

    inline long heuristic(const Point &p) {
        return std::max(square<long>(r1.x - p.x), square<long>(r2.x - p.x)) +
               std::max(square<long>(r1.y - p.y), square<long>(r2.y - p.y));
    }

} *root;

inline Node *build(int l, int r) {
    if (l > r) return NULL;
    register int mid = l + r >> 1;
    flag = getSplit(l, r), std::nth_element(p + l, p + mid, p + r + 1, cmp);
    Node *o = new Node(flag, p[mid]);
    o->c[0] = build(l, mid - 1), o->c[1] = build(mid + 1, r);
    return o->maintain(), o;
}

inline long dis(const Point &a, const Point &b) {
    return square<long>(a.x - b.x) + square<long>(a.y - b.y);
}

int cur;

inline void query(Node *o) {
    register long d = dis(o->p, p[cur]);
    if (d > q.top()) q.pop(), q.push(d);
    register long l = (o->c[0] ? o->c[0]->heuristic(p[cur]) : 0);
    register long r = (o->c[1] ? o->c[1]->heuristic(p[cur]) : 0);
    if (l > r) {
        if (o->c[0] && l > q.top()) query(o->c[0]);
        if (o->c[1] && r > q.top()) query(o->c[1]);
    } else {
        if (o->c[1] && r > q.top()) query(o->c[1]);
        if (o->c[0] && l > q.top()) query(o->c[0]);
    }
}

inline void query(const int pos) {
    cur = pos, query(root);
}

#undef long

int main() {
    // freopen("in.in", "r", stdin);
    using namespace IO;
    register int n, k;
    read(n), read(k);

    for (register int i = 1; i <= n; i++)
        read(p[i].x), read(p[i].y);
    root = build(1, n);
    q.resize(k * 2);
    for (register int i = 1; i <= n; i++) query(i);
    print(q.top());
    flush();
    return 0;
}
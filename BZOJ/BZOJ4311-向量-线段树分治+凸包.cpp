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
 * 「BZOJ 4311」向量 26-02-2018
 * 线段树分治 + 凸包
 * @author xehoth
 */
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <vector>

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

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 200000 + 9;

struct Point {
    int x, y;

    Point() {}

    Point(int x, int y) : x(x), y(y) {}

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline long long operator*(const Point &p) const {
        return (long long)x * p.y - (long long)y * p.x;
    }

    inline bool operator<(const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};

inline long long dot(const Point &a, const Point &b) {
    return (long long)a.x * b.x + (long long)a.y * b.y;
}

struct Node *null;

char *cur;

struct Node {
    static const int NODE_SIZE;

    Node *lc, *rc;
    std::vector<const Point *> c;
    int pos;

    inline void insert(const Point *p) {
        while (c.size() > 1 &&
               (*p - *c[c.size() - 1]) * (*p - *c[c.size() - 2]) <= 0)
            c.pop_back();
        c.push_back(p);
    }

    inline long long query(const Point *p) {
        if (c.empty()) return 0;
        while (pos < (int)c.size() - 1 &&
               dot(*p, *c[pos + 1]) >= dot(*p, *c[pos]))
            pos++;
        return dot(*p, *c[pos]);
    }

    Node() : lc(null), rc(null), c(), pos() {}

    inline void *operator new(size_t) { return cur += NODE_SIZE; }
};

const int Node::NODE_SIZE = sizeof(Node);

char pool[MAXN * Node::NODE_SIZE * 4];

struct Data {
    Point p;
    int l, r;
} d[MAXN], *que[MAXN], *id[MAXN];
int iCnt, eCnt, qCnt;

inline bool cmpX(const Data *a, const Data *b) { return a->p < b->p; }

inline bool cmpQ(const Data *a, const Data *b) { return a->p * b->p < 0; }

class SegmentTree {
   public:
    SegmentTree(const int n) : root(null), n(n) { build(root, 1, n); }

    inline void insert(int l, int r, const Point *p) {
        insert(root, 1, n, l, r, p);
    }

    inline long long query(int k, const Point *p) {
        return query(root, 1, n, k, p);
    }

   private:
    Node *root;
    int n;

    void build(Node *&p, int l, int r) {
        p = new Node;
        if (l == r) return;
        int mid = (l + r) >> 1;
        build(p->lc, l, mid);
        build(p->rc, mid + 1, r);
    }

    void insert(Node *p, int l, int r, int s, int t, const Point *pts) {
        if (s <= l && t >= r) {
            p->insert(pts);
            return;
        }
        int mid = (l + r) >> 1;
        if (s <= mid) insert(p->lc, l, mid, s, t, pts);
        if (t > mid) insert(p->rc, mid + 1, r, s, t, pts);
    }

    long long query(Node *p, int l, int r, int k, const Point *pts) {
        long long ans = p->query(pts);
        if (l == r) return ans;
        int mid = (l + r) >> 1;
        return k <= mid ? std::max(ans, query(p->lc, l, mid, k, pts))
                        : std::max(ans, query(p->rc, mid + 1, r, k, pts));
    }
};

long long ans[MAXN];

int main() {
    // freopen("sample/1.in", "r", stdin);
    cur = pool;
    null = (Node *)pool;
    null->lc = null;
    null->rc = null;
    null->pos = 0;
    null->c = std::vector<const Point *>();
    int n;
    io >> n;
    for (int i = 1, cmd, t; i <= n; i++) {
        io >> cmd;
        switch (cmd) {
            case 1: {
                io >> d[i].p.x >> d[i].p.y;
                d[i].l = i;
                d[i].r = n;
                id[++iCnt] = d + i;
                break;
            }
            case 2: {
                io >> t;
                id[t]->r = i - 1;
                d[i].r = -1;
                break;
            }
            case 3: {
                io >> d[i].p.x >> d[i].p.y;
                d[i].l = i;
                que[++qCnt] = d + i;
                break;
            }
        }
    }
    SegmentTree seg(n);
    iCnt = 0;
    for (int i = 1; i <= n; i++)
        if (d[i].r > 0) id[++iCnt] = d + i;
    std::sort(id + 1, id + iCnt + 1, cmpX);
    for (int i = 1; i <= iCnt; i++) seg.insert(id[i]->l, id[i]->r, &id[i]->p);
    std::sort(que + 1, que + qCnt + 1, cmpQ);
    for (int i = 1; i <= qCnt; i++)
        ans[que[i]->l] = seg.query(que[i]->l, &que[i]->p);
    for (int i = 1; i <= n; i++)
        if (d[i].r == 0) io << ans[i] << '\n';
    return 0;
}
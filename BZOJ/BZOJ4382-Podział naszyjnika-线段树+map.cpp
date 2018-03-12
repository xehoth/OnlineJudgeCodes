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
 * 「BZOJ 4382」Podział naszyjnika 11-03-2018
 * 线段树 + map
 * @author xehoth
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <map>
#include <vector>

inline void *operator new(size_t size) {
    static char pool[1024 * 1024 * 50], *s = pool;
    char *t = s;
    s += size;
    return t;
}

inline void operator delete(void *) {}

inline void operator delete(void *, size_t) {}

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
} io;

const int MAXN = 2000000 + 5;

struct PriorityQueue : public std::map<int, int, std::greater<int> > {
    typedef std::map<int, int, std::greater<int> > super;

    inline void push(int x) { super::operator[](x)++; }

    inline void erase(int x) {
        super::iterator it = super::find(x);
        if (it != super::end() && --it->second == 0) super::erase(it);
    }

    inline int top() { return super::empty() ? 0 : super::begin()->first; }
};

struct Node {
    static char *cur;
    static Node *null;
    static const int NODE_SIZE;

    bool active, tag;
    int sum;
    Node *lc, *rc;

    Node() : active(true), tag(), sum(), lc(null), rc(null) {}

    inline void *operator new(size_t) { return cur += NODE_SIZE; }

    inline void cover() {
        active = false;
        sum = 0;
        tag = true;
    }

    inline void maintain() {
        active = lc->active | rc->active;
        sum = lc->sum + rc->sum;
    }

    inline void pushDown() {
        if (tag) {
            lc->cover();
            rc->cover();
            tag = false;
        }
    }
};

char *Node::cur;
Node *Node::null;

const int Node::NODE_SIZE = sizeof(Node);

char pool[Node::NODE_SIZE * MAXN];

class SegmentTree {
   public:
    SegmentTree(int n) : n(n) { build(root, 1, n); }

    void erase(int l, int r) { erase(root, 1, n, l, r); }

    int precursor(int pos) { return precursor(root, 1, n, pos); }

    int successor(int pos) { return successor(root, 1, n, pos); }

    int query(int l, int r) { return query(root, 1, n, l, r); }

   private:
    Node *root;
    int n;

    void build(Node *&p, int l, int r) {
        p = new Node;
        p->sum = r - l + 1;
        if (l == r) return;
        int mid = (l + r) >> 1;
        build(p->lc, l, mid);
        build(p->rc, mid + 1, r);
    }

    void erase(Node *p, int l, int r, int s, int t) {
        if (s <= l && t >= r) {
            p->cover();
            return;
        }
        p->pushDown();
        int mid = (l + r) >> 1;
        if (s <= mid) erase(p->lc, l, mid, s, t);
        if (t > mid) erase(p->rc, mid + 1, r, s, t);
        p->maintain();
    }

    int precursor(Node *p, int l, int r, int pos) {
        if (!p->active) return -1;
        if (l == r) return l;
        p->pushDown();
        int mid = (l + r) >> 1, ret = -1;
        if (pos > mid) ret = precursor(p->rc, mid + 1, r, pos);
        if (ret == -1) ret = precursor(p->lc, l, mid, pos);
        return ret;
    }

    int successor(Node *p, int l, int r, int pos) {
        if (!p->active) return -1;
        if (l == r) return l;
        p->pushDown();
        int mid = (l + r) >> 1, ret = -1;
        if (pos <= mid) ret = successor(p->lc, l, mid, pos);
        if (ret == -1) ret = successor(p->rc, mid + 1, r, pos);
        return ret;
    }

    int query(Node *p, int l, int r, int s, int t) {
        if (s <= l && t >= r) return p->sum;
        p->pushDown();
        int mid = (l + r) >> 1, ret = 0;
        if (s <= mid) ret += query(p->lc, l, mid, s, t);
        if (t > mid) ret += query(p->rc, mid + 1, r, s, t);
        return ret;
    }
};

int c[MAXN];
std::vector<int> a[MAXN];

PriorityQueue q;

int main() {
    int n, k;
    Node::cur = pool;
    Node::null = (Node *)pool;
    Node::null->lc = Node::null->rc = Node::null;
    io >> n >> k;
    SegmentTree d(n);
    for (int i = 1; i <= n; i++) io >> c[i];
    for (int i = n; i; i--) a[c[i]].push_back(i);
    long long ans1 = 0;
    int ans2 = n;
    for (int i = 1; i < n; i++) {
        q.push(i);
        if (a[c[i]].front() == i) {
            for (int j = 0; j < (int)a[c[i]].size(); j++) q.erase(a[c[i]][j]);
            d.erase(a[c[i]].back() + 1, a[c[i]].front());
        }
        int now = q.top() + 1;
        int l = d.precursor(std::max(now, i - n / 2 + 1));
        int r = d.successor(std::max(now, i - n / 2 + 1));
        ans1 += d.query(now, i);
        if (l >= now) ans2 = std::min(ans2, std::abs(n - 2 * (i - l + 1)));
        if (r >= now) ans2 = std::min(ans2, std::abs(n - 2 * (i - r + 1)));
    }
    printf("%lld %d", ans1, ans2);
    return 0;
}

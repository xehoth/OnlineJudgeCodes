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
 * 「LOJ 6029」市场 11-01-2018
 * 线段树
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

    inline char read() {
        static const int IN_LEN = 1 << 18 | 1;
        static char buf[IN_LEN], *s, *t;
        return (s == t) && (t = (s = buf) + fread(buf, 1, IN_LEN, stdin)), s == t ? -1 : *s++;
    }

    const int OUT_LEN = 1 << 18 | 1;

    char obuf[OUT_LEN], *oh = obuf;

    inline void print(char c) {
        (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
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

    const int MAXN = 100000 + 9;

    char *cur;

    struct Node *null;

    struct Node {
        static const int NODE_SIZE;
        Node *lc, *rc;
        long long sum;
        int min, max, add, size;

        inline void *operator new(size_t) { return cur += NODE_SIZE; }

        Node() : lc(null), rc(null) {}

        inline void pushDown() {
            if (add) {
                lc->cover(add);
                rc->cover(add);
                add = 0;
            }
        }

        inline void cover(const int v) {
            if (this == null) return;
            sum += (long long)size * v;
            min += v;
            max += v;
            add += v;
        }

        inline void maintain() {
            sum = lc->sum + rc->sum;
            min = std::min(lc->min, rc->min);
            max = std::max(lc->max, rc->max);
        }
    };

    const int Node::NODE_SIZE = sizeof(Node);

    char pool[4 * MAXN * Node::NODE_SIZE];

    Node *root;

    int a[MAXN];

    inline void build(Node *&p, int l, int r) {
        p = new Node();
        p->size = r - l + 1;
        if (l == r) {
            p->sum = p->min = p->max = a[l];
            return;
        }
        register int mid = (l + r) >> 1;
        build(p->lc, l, mid);
        build(p->rc, mid + 1, r);
        p->maintain();
    }

    inline void add(Node *p, int l, int r, int s, int t, int v) {
        if (s <= l && t >= r) return (void)(p->cover(v));
        p->pushDown();
        register int mid = (l + r) >> 1;
        if (s <= mid) add(p->lc, l, mid, s, t, v);
        if (t > mid) add(p->rc, mid + 1, r, s, t, v);
        p->maintain();
    }

    inline void div(Node *p, int l, int r, int s, int t, int v) {
        if (s <= l && t >= r) {
            register int p1 = p->min - (floor((double)p->min / v));
            register int p2 = p->max - (floor((double)p->max / v));
            if (p1 == p2) return (void)(p->cover(-p1));
        }
        p->pushDown();
        register int mid = (l + r) >> 1;
        if (s <= mid) div(p->lc, l, mid, s, t, v);
        if (t > mid) div(p->rc, mid + 1, r, s, t, v);
        p->maintain();
    }

    inline long long querySum(Node *p, int l, int r, int s, int t) {
        if (s <= l && t >= r) return p->sum;
        p->pushDown();
        register int mid = (l + r) >> 1;
        register long long ret = 0;
        if (s <= mid) ret += querySum(p->lc, l, mid, s, t);
        if (t > mid) ret += querySum(p->rc, mid + 1, r, s, t);
        return ret;
    }

    inline int queryMin(Node *p, int l, int r, int s, int t) {
        if (s <= l && t >= r) return p->min;
        p->pushDown();
        register int mid = (l + r) >> 1, ret = INT_MAX;
        if (s <= mid) ret = std::min(ret, queryMin(p->lc, l, mid, s, t));
        if (t > mid) ret = std::min(ret, queryMin(p->rc, mid + 1, r, s, t));
        return ret;
    }

    inline void solve() {
        cur = pool;
        null = (Node *)pool;
        null->lc = null->rc = null;
        null->min = INT_MAX;
        null->max = INT_MIN;
        register int n, m;
        io >> n >> m;
        for (register int i = 1; i <= n; i++) io >> a[i];
        build(root, 1, n);
        for (register int cmd, l, r, v; m--;) {
            io >> cmd >> l >> r;
            l++;
            r++;
            switch (cmd) {
                case 1: {
                    io >> v;
                    add(root, 1, n, l, r, v);
                    break;
                }
                case 2: {
                    io >> v;
                    div(root, 1, n, l, r, v);
                    break;
                }
                case 3: {
                    io << queryMin(root, 1, n, l, r) << '\n';
                    break;
                }
                case 4: {
                    io << querySum(root, 1, n, l, r) << '\n';
                    break;
                }
            }
        }
    }
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}

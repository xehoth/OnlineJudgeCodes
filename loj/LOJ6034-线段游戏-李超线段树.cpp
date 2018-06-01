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
 * 「LOJ 6034」线段游戏 01-06-2018
 * 李超线段树
 * @author xehoth
 */
#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>

const double EPS = 1e-3;

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
    char *s, obuf[SIZE], *oh;

    InputOutputStream() : oh(obuf) {
        int fd = fileno(stdin);
        struct stat st;
        fstat(fd, &st);
        s = (char *)mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    }
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    InputOutputStream &operator>>(int &x) {
        static bool iosig;
        for (iosig = false; !isdigit(*s); s++) iosig |= *s == '-';
        for (x = 0; isdigit(*s); s++) x = x * 10 + (*s ^ 48);
        if (iosig) x = -x;
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

    static inline int sign(const double x) { return (x > EPS) - (x < -EPS); }

    void print(double x) {
        int sig = sign(x);
        if (sig == 0) {
            print('0');
            return;
        }
        (sig == -1) && (print('-'), x = -x);
        print((int)x);
        x = x - (int)x;
        print('.');
        static int buf[21], cnt;
        int t = x * 1000;
        for (cnt = 0; t; t /= 10) buf[++cnt] = t % 10 | 48;
        for (int i = 3; i > cnt; i--) print('0');
        while (cnt) print((char)buf[cnt--]);
    }

    template <typename T>
    InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int POOL_SIZE = 1024 * 1024 * 20;
const int INF = 0x3f3f3f3f;
const int B = 100000;

char pool[POOL_SIZE];

void *operator new(size_t size) {
    static char *s = pool + POOL_SIZE;
#ifdef _DEBUG
    if (s - pool < (int)size) {
        std::cerr << "MemoryPool Overflow!!!" << std::endl;
        exit(0);
    }
#endif
    return s -= size;
}

void operator delete(void *) {}

void operator delete(void *, size_t) {}

struct Line {
    double k, b;
    int id;

    double v(int x) const { return k * x + b; }
};

struct Node *null;

struct Node {
    Node *lc, *rc;
    Line d;

    Node() : lc(null), rc(null), d() {}
};

Node *root = null;

inline bool cmp(const Line &a, const Line &b, const int x) {
    if (!a.id) return false;
    if (!b.id) return true;
    return a.v(x) > b.v(x) || (a.v(x) == b.v(x) && a.id < b.id);
}

inline void fix(Node *p, int l, int r, Line d) {
    if (p == null) return;
    if (p->d.id == 0) {
        p->d = d;
        return;
    }
    if (cmp(d, p->d, l)) std::swap(d, p->d);
    if (p->d.v(r) > d.v(r)) return;
    int mid = (l + r) >> 1;
    double x = (p->d.b - d.b) / (d.k - p->d.k);
    if (x < l || x > r) return;
    if (x <= mid) {
        fix(p->lc, l, mid, p->d);
        p->d = d;
    } else {
        fix(p->rc, mid + 1, r, d);
    }
}

inline void insert(Node *p, int l, int r, int s, int t, const Line &d) {
    if (s > B || t <= 0) return;
    if (s <= l && t >= r) {
        fix(p, l, r, d);
        return;
    }
    int mid = (l + r) >> 1;
    if (s <= mid) insert(p->lc, l, mid, s, t, d);
    if (t > mid) insert(p->rc, mid + 1, r, s, t, d);
}

inline Line *query(Node *p, int l, int r, int x) {
    if (l == r) return &p->d;
    int mid = (l + r) >> 1;
    Line *ret;
    if (x <= mid)
        ret = query(p->lc, l, mid, x);
    else
        ret = query(p->rc, mid + 1, r, x);
    if (cmp(p->d, *ret, x)) return &p->d;
    return ret;
}

inline void build(Node *&p, int l, int r) {
    p = new Node;
    if (l == r) return;
    int mid = (l + r) >> 1;
    build(p->lc, l, mid);
    build(p->rc, mid + 1, r);
}

int main() {
    null = new Node;
    null->lc = null->rc = null;
    int n, q;
    io >> n >> q;
    build(root, 1, B);
    int tot = 0;
    for (int i = 1, x0, x1, y0, y1; i <= n; i++) {
        io >> x0 >> y0 >> x1 >> y1;
        if (x0 > x1) {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        Line tmp;
        if (x0 == x1) {
            tmp.k = 0;
            tmp.b = std::max(y1, y0);
        } else {
            tmp.k = (double)(y1 - y0) / (x1 - x0);
            tmp.b = y0 - x0 * tmp.k;
        }
        tmp.id = ++tot;
        insert(root, 1, B, x0, x1, tmp);
    }
    for (int cmd, x0, x1, y0, y1; q--;) {
        io >> cmd;
        switch (cmd) {
            case 0: {
                io >> x0 >> y0 >> x1 >> y1;
                if (x0 > x1) {
                    std::swap(x0, x1);
                    std::swap(y0, y1);
                }
                Line tmp;
                if (x0 == x1) {
                    tmp.k = 0;
                    tmp.b = std::max(y1, y0);
                } else {
                    tmp.k = (double)(y1 - y0) / (x1 - x0);
                    tmp.b = y0 - x0 * tmp.k;
                }
                tmp.id = ++tot;
                insert(root, 1, B, x0, x1, tmp);
                break;
            }
            case 1: {
                io >> x0;
                Line *ret = query(root, 1, B, x0);
                if (ret->id == 0) {
                    io << 0 << '\n';
                } else {
                    io << ret->v(x0) << '\n';
                }
                break;
            }
        }
    }
    return 0;
}
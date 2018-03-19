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
 * 「BZOJ 3786」星系探索 19-03-2018
 * AAA 树
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 1 << 18 | 1;
    static char buf[IN_LEN], *s, *t;
    return (s == t) && (t = (s = buf) + fread(buf, 1, IN_LEN, stdin)),
           s == t ? -1 : *s++;
}

const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

inline char read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
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
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        iosig |= c == '-';
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig && (x = -x);
}

struct InputOutputStream {
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 300000 + 9;

struct Info {
    int size;
    long long sum;
    Info(int size = 0, long long sum = 0) : size(size), sum(sum) {}
};

const Info EMPTY;

struct Mark {
    long long plus;
    Mark(long long plus = 0) : plus(plus) {}
    inline void clear() { plus = 0; }
};

inline void operator+=(long long &x, const Mark &b) { x = x + b.plus; }
inline void operator+=(Mark &a, const Mark &b) { a.plus = a.plus + b.plus; }

inline void operator+=(Info &a, const Mark &b) {
    if (a.size != 0) {
        a.sum = a.sum + a.size * b.plus;
    }
}

inline void operator+=(Info &a, const Info &b) {
    a.size += b.size;
    a.sum += b.sum;
}

struct Node *null;
char *cur;

struct Node {
    static const int NODE_SIZE;
    Node *c[4], *fa;
    Mark chainMark, treeMark;
    Info chain, tree, all;

    bool inner, rev;
    long long val;

    inline void *operator new(size_t) { return cur += NODE_SIZE; }

    Node()
        : fa(null),
          inner(true),
          rev(false),
          val(0),
          chainMark(),
          treeMark(),
          chain(),
          tree(),
          all() {
        c[0] = c[1] = c[2] = c[3] = null;
    }

    Node(long long v)
        : fa(null),
          inner(false),
          rev(false),
          val(v),
          chainMark(),
          treeMark(),
          chain(),
          tree(),
          all() {
        c[0] = c[1] = c[2] = c[3] = null;
        maintain();
    }

    inline void reverse() {
        rev ^= 1;
        std::swap(c[0], c[1]);
    }

    inline void applyChain(const Mark &a) {
        if (this != null) {
            chainMark += a;
            chain += a;
            val += a;
            all = chain;
            all += tree;
        }
    }

    inline void applyTree(const Mark &a, register bool flag = true) {
        if (this != null) {
            treeMark += a;
            tree += a;
            all += a;
            flag && (applyChain(a), 0);
        }
    }

    inline void pushDown() {
        rev && (c[0]->reverse(), c[1]->reverse(), rev = 0);

        if (treeMark.plus != 0) {
            c[0]->applyTree(treeMark, 0);
            c[1]->applyTree(treeMark, 0);
            c[2]->applyTree(treeMark);
            c[3]->applyTree(treeMark);
            treeMark.clear();
        }

        if (chainMark.plus != 0) {
            c[0]->applyChain(chainMark);
            c[1]->applyChain(chainMark);
            chainMark.clear();
        }
    }

    inline void maintain() {
        if (!inner) {
            tree = EMPTY;
            chain = all = Info(1, val);

            chain += c[0]->chain;
            tree += c[0]->tree;

            chain += c[1]->chain;
            tree += c[1]->tree;

            tree += c[2]->all;
            tree += c[3]->all;

            all = chain;
            all += tree;
        } else {
            chain = tree = EMPTY;

            tree += c[2]->all;
            tree += c[3]->all;

            all = tree;
        }
    }

    inline Node *get(register int d) {
        c[d]->pushDown();
        return c[d];
    }

    inline bool relation(int t) { return fa->c[t + 1] == this; }

    inline int pos() {
        if (fa->c[0] == this) return 0;
        if (fa->c[1] == this) return 1;
        if (fa->c[2] == this) return 2;
        if (fa->c[3] == this) return 3;
        return -1;
    }

    inline void set(register Node *pt, register int d) {
        (c[d] = pt)->fa = this;
    }

    inline bool isRoot(int t) {
        return t ? ((fa == null) || !fa->inner || !inner)
                 : ((fa == null) || (fa->c[0] != this && fa->c[1] != this));
    }

    inline void rotate(register int t) {
        register Node *o = fa;
        register int f = relation(t);
        (fa = o->fa)->c[o->pos()] = this;
        (o->c[f + t] = c[!f + t])->fa = o;
        (c[!f + t] = o)->fa = this;
        o->maintain();
    }

    inline void splay(int t = 0) {
        while (!isRoot(t)) {
            if (fa->isRoot(t))
                rotate(t);
            else if (fa->relation(t) == relation(t))
                fa->rotate(t), rotate(t);
            else
                rotate(t), rotate(t);
        }
        maintain();
    }
};

const int Node::NODE_SIZE = sizeof(Node);

char pool[MAXN * 2 * Node::NODE_SIZE];

inline void add(register Node *u, register Node *v) {
    v->pushDown();
    if (v->c[2] == null) {
        v->set(u, 2);
        return;
    }
    if (v->c[3] == null) {
        v->set(u, 3);
        return;
    }
    register Node *t = v, *x = new Node;

    for (; t->c[2]->inner; t = t->get(2))
        ;
    x->set(t->c[2], 2);
    x->set(u, 3);
    t->set(x, 2);
    x->splay(2);
}

inline void del(register Node *u) {
    if (u->fa->inner) {
        u->fa->fa->set(u->fa->c[5 - u->pos()], u->fa->pos());
        register Node *t = u->fa;
        u->fa->fa->splay(2);
        // delete t;
    } else {
        u->fa->set(null, u->pos());
    }
    u->fa = null;
}

inline void pushTo(register Node *u) {
    static Node *stack[MAXN * 2];
    register int top = 0;
    for (; u != null; u = u->fa) stack[top++] = u;
    while (top) stack[--top]->pushDown();
}

inline void access(register Node *u) {
    register Node *v = u, *t;
    pushTo(u);
    u->splay();
    if (u->c[1] != null) {
        t = u->c[1], u->c[1] = null, add(t, u), u->maintain();
    }
    while (u->fa != null) {
        for (t = u->fa; t->inner; t = t->fa)
            ;
        t->splay();
        if (t->c[1] != null)
            u->fa->set(t->c[1], u->pos()), u->fa->splay(2), t->set(u, 1);
        else
            del(u), t->set(u, 1);
        u = t;
        u->maintain();
    }
    v->splay();
}

inline void evert(register Node *u) {
    access(u);
    u->reverse();
}

inline Node *getPar(register Node *u) {
    access(u);
    u = u->get(0);
    while (u != null && u->c[1] != null) u = u->get(1);
    return u;
}

inline Node *getRoot(register Node *u) {
    while (u->fa != null) u = u->fa;
    return u;
}

inline Node *cut(register Node *u) {
    register Node *v = getPar(u);
    if (v != null) access(v), del(u), v->maintain();
    return v;
}

inline void link(register Node *u, register Node *v) {
    register Node *o = cut(u);
    if (getRoot(u) != getRoot(v)) o = v;
    if (o != null) access(o), add(u, o), o->maintain();
}

int n, q, fa[MAXN];
Node *v[MAXN];

inline void solve() {
    // freopen("sample/1.in", "r", stdin);
    cur = pool;
    null = (Node *)pool;
    *null = Node();
    null->inner = false;
    io >> n;
    v[0] = null;
    for (register int i = 2; i <= n; i++) io >> fa[i];
    for (register int i = 1, val; i <= n; i++) {
        io >> val;
        v[i] = new Node(val);
    }
    for (int i = 2; i <= n; i++) {
        add(v[i], v[fa[i]]);
        v[fa[i]]->maintain();
    }
    evert(v[1]);
    io >> q;
    char cmd;
    for (register int x, y; q--;) {
        io >> cmd;
        switch (cmd) {
            case 'Q': {
                io >> x;
                Node *u = v[x];
                access(u);
                io << u->chain.sum << '\n';
                break;
            }
            case 'C': {
                io >> x >> y;
                Node *u = v[x];
                cut(u);
                evert(v[y]);
                link(v[y], v[x]);
                evert(v[1]);
                break;
            }
            case 'F': {
                io >> x;
                Node *u = v[x];
                access(u);
                register int y;
                io >> y;
                Mark m(y);
                u->val += m;
                u->c[2]->applyTree(m);
                u->c[3]->applyTree(m);
                u->maintain();
                break;
            }
        }
    }
}
}  // namespace
#undef int

int main() { solve(); }
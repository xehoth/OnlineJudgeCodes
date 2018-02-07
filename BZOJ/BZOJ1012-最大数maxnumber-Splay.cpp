/**
 * Copyright (c) 2016, xehoth
 * All rights reserved.
 * 「BZOJ 1012」最大数maxnumber 19-12-2016
 * Splay
 * @author xehoth
 */
#include <bits/stdc++.h>
inline char read() {
    const int iol = 1024 * 1024;
    static char buf[iol], *ioh, *iot;
    if (ioh == iot) {
        iot = (ioh = buf) + fread(buf, 1, iol, stdin);
        if (iot == ioh) return -1;
    }
    return *ioh++;
}
template <class T>
inline void read(T &x) {
    static char ioc;
    static bool iosig = 0;
    for (iosig = 0, ioc = read(); !isdigit(ioc); ioc = read())
        if (ioc == '-') iosig = 1;
    for (x = 0; isdigit(ioc); ioc = read())
        x = (x << 1) + (x << 3) + (ioc ^ '0');
    if (iosig) x = -x;
}
#define null NULL
const int MAXN = 200000;
template <class T, size_t size>
struct MemoryPool {
    T buf[size], *tail, *st[size];
    int top;
    inline T *alloc() { return top ? st[--top] : tail++; }
    inline void recycle(T *x) { st[top++] = x; }
    MemoryPool() : top(0), tail(buf) {}
};
const int MAXM = 200000;
template <typename T>
struct Splay {
    enum Relation { L = 0, R = 1 };
    struct Node {
        Node *parent, *child[2], **root;
        int value, max;
        int size;
        bool bound;
        inline void init(Node *parent, const T &value, Node **root,
                         bool bound = false) {
            this->parent = parent, this->value = value, this->root = root,
            this->bound = bound, this->size = 1;
        }
        inline void maintain() {
            size = (child[L] ? child[L]->size : 0) +
                   (child[R] ? child[R]->size : 0) + 1,
            max = value;
            if (child[L] && !child[L]->bound)
                max = std::max(max, child[L]->max);
            if (child[R] && !child[R]->bound)
                max = std::max(max, child[R]->max);
        }
        inline Relation relation() { return this == parent->child[L] ? L : R; }
        inline void rotate() {
            Relation x = relation();
            Node *oldParent = parent;
            if (oldParent->parent)
                oldParent->parent->child[oldParent->relation()] = this;
            parent = oldParent->parent, oldParent->child[x] = child[x ^ 1];
            if (child[x ^ 1]) child[x ^ 1]->parent = oldParent;
            child[x ^ 1] = oldParent, oldParent->parent = this,
                      oldParent->maintain(), maintain();
            if (!parent) *root = this;
        }
        inline void splay(Node *targetParent = null) {
            while (parent != targetParent) {
                if (parent->parent == targetParent)
                    rotate();
                else if (parent->relation() == relation())
                    parent->rotate(), rotate();
                else
                    rotate(), rotate();
            }
        }
        inline int rank() { return child[L] ? child[L]->size : 0; }
        inline Node *precursor() {
            splay();
            Node *v = child[L];
            while (v->child[R]) v = v->child[R];
            return v;
        }
        inline Node *successor() {
            splay();
            Node *v = child[R];
            while (v->child[L]) v = v->child[L];
            return v;
        }
    } * root;
    MemoryPool<Node, MAXN> pool;
    Splay() { buildBound(L), buildBound(R); }
    inline void buildBound(Relation x) {
        Node **v = &root, *parent = null;
        while (*v) parent = *v, parent->size++, v = &parent->child[x];
        *v = pool.alloc(), (*v)->init(parent, x == L ? -1 : 1, &root, true);
    }
    inline void append(const T &value) {
        Node **v = &root, *parent = null;
        while (*v)
            parent = *v, parent->size++,
            v = (parent->bound && parent->value == 1 ? &parent->child[L]
                                                     : &parent->child[R]);
        *v = pool.alloc(), (*v)->init(parent, value, &root), (*v)->splay();
    }
    inline Node *select(int k) {
        k++;
        Node *v = root;
        while (k != v->rank() + 1)
            v = (k <= v->rank() ? v->child[L]
                                : (k -= v->rank() + 1, v->child[R]));
        return v;
    }
    inline Node *select(int l, int r) {
        Node *pre = select(l)->precursor(), *suc = select(r)->successor();
        return pre->splay(), suc->splay(root), suc->child[L];
    }
    inline const T &queryMax(int l, int r) { return select(l, r)->max; }
    inline int size() { return root->size - 2; }
};
int m, p, lastAns;
Splay<int> splay;
inline char isVaild(char ch) { return ch == 'A' || ch == 'Q'; }
int main() {
    read(m), read(p);
    for (register int i = 0; i < m; i++) {
        char cmd;
        while (!isVaild(cmd = read()))
            ;
        if (cmd == 'A') {
            register int n;
            read(n);
            splay.append((n + lastAns) % p);
        } else {
            register int l;
            read(l);
            std::cout << (lastAns = splay.queryMax(splay.size() - l + 1,
                                                   splay.size()))
                      << "\n";
        }
    }
    return 0;
}
#include <bits/stdc++.h>
inline char read() {
    const int iol = 1024 * 1024;
    static char buf[iol], *ioh, *iot;
    if (ioh == iot) {
        iot = (ioh = buf) + fread(buf, 1, iol, stdin);
        if (ioh == iot) return -1;
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
template <class T, size_t size>
struct MemoryPool {
    T buf[size], *tail, *st[size];
    int top;
    inline T *alloc() { return top ? st[--top] : tail++; }
    inline void recycle(T *x) { st[top++] = x; }
    MemoryPool() : top(0), tail(buf) {}
};
const int MAXN = 200100;
#define null NULL
template <class T, T INF>
struct Splay {
    enum Relation { L = 0, R = 1 };
    struct Node {
        Node *child[2], *parent, **root;
        T value, lazy;
        int size, count;
        inline void init(Node *parent, const T &value, Node **root) {
            this->parent = parent, this->value = value, this->root = root,
            this->count = this->size = 1, this->lazy = 0,
            child[L] = child[R] = null;
        }
        inline Relation relation() { return this == parent->child[L] ? L : R; }
        inline void recycle(MemoryPool<Node, MAXN> &pool) {
            if (child[L]) pool.recycle(child[L]);
            if (child[R]) pool.recycle(child[R]);
        }
        inline void update(const T &delta) {
            if (this->value != INF && this->value != -INF) this->value += delta;
            this->lazy += delta;
        }
        inline void pushDown() {
            if (lazy) {
                if (child[L]) child[L]->update(lazy);
                if (child[R]) child[R]->update(lazy);
                lazy = 0;
            }
        }
        inline void maintain() {
            pushDown(), size = count + (child[L] ? child[L]->size : 0) +
                               (child[R] ? child[R]->size : 0);
        }
        inline void rotate() {
            pushDown();
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
                parent->pushDown(), pushDown();
                if (parent->parent == targetParent)
                    rotate();
                else {
                    parent->parent->pushDown();
                    if (parent->relation() == relation())
                        parent->rotate(), rotate();
                    else
                        rotate(), rotate();
                }
            }
        }
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
        inline int rank() { return child[L] ? child[L]->size : 0; }
    } * root;
    MemoryPool<Node, MAXN> pool;
    Splay() : root(null) { insert(INF), insert(-INF); }
    inline Node *find(const T &value) {
        Node *v = root;
        while (v && value != v->value)
            v->pushDown(), v = (value < v->value ? v->child[L] : v->child[R]);
        return v ? (v->splay(), v) : null;
    }
    inline Node *insert(const T &value) {
        Node *v = find(value);
        if (v) return v->count++, v->maintain(), v;
        Node **target = &root, *parent = null;
        while (*target)
            parent = *target, parent->pushDown(), parent->size++,
            target =
                (value < parent->value ? &parent->child[L] : &parent->child[R]);
        return *target = pool.alloc(), (*target)->init(parent, value, &root),
               (*target)->splay(), root;
    }
    inline const T &select(int k) {
        k++;
        Node *v = root;
        while (v->pushDown(), !(v->rank() < k && v->rank() + v->count >= k))
            v = (k <= v->rank() ? v->child[L]
                                : (k -= v->rank() + v->count, v->child[R]));
        return v->splay(), v->value;
    }
    inline void erase(Node *l, Node *r) {
        Node *pre = l->precursor(), *suc = r->successor();
        pre->splay(), suc->splay(pre), suc->child[L]->recycle(pool);
        pool.recycle(suc->child[L]), suc->child[L] = null, suc->maintain(),
                                     pre->maintain();
    }
    inline void erase(Node *v) { v->count > 1 ? v->count-- : erase(v, v); }
    inline void erase(const T &l, const T &r) {
        Node *vl = find(l), *vr = find(r);
        if (!vl) vl = insert(l);
        if (!vr) vr = insert(r);
        erase(vl, vr);
    }
    inline void update(const T &value) { root->update(value); }
    inline int rank(const T &value) {
        Node *v = find(value);
        if (v)
            return v->rank();
        else {
            v = insert(value);
            const int ans = v->rank();
            return erase(v), ans;
        }
    }
    inline int size() { return root->size - 2; }
};
int n, min, deletedCount;
Splay<int, INT_MAX> splay;
inline bool isValid(char c) {
    return c == 'I' || c == 'A' || c == 'S' || c == 'F';
}
int main() {
    read(n), read(min);
    for (register int i = 0; i < n; i++) {
        char c;
        register int k;
        while (!isValid(c = read()))
            ;
        read(k);
        if (c == 'I') {
            if (k >= min) splay.insert(k);
        } else if (c == 'A') {
            splay.update(k);
        } else if (c == 'S') {
            splay.update(-k);
            register int oldSize = splay.size();
            splay.erase(-INT_MAX + 1, min - 1);
            deletedCount += oldSize - splay.size();
        } else if (c == 'F') {
            if (k < 1 || k > splay.size())
                std::cout << "-1\n";
            else
                std::cout << splay.select(splay.size() - k + 1) << "\n";
        }
    }
    printf("%d\n", deletedCount);
    return 0;
}
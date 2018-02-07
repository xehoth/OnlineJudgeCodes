#include <bits/stdc++.h>
inline char read() {
    static const int IO_LEN = 1024 * 1024;
    static char buf[IO_LEN], *ioh, *iot;
    if (ioh == iot) {
        iot = (ioh = buf) + fread(buf, 1, IO_LEN, stdin);
        if (ioh == iot) return -1;
    }
    return *ioh++;
}
template <class T>
inline void read(T &x) {
    static bool iosig = 0;
    static char ioc;
    for (iosig = 0, ioc = read(); !isdigit(ioc); ioc = read())
        if (ioc == '-') iosig = 1;
    for (x = 0; isdigit(ioc); ioc = read())
        x = (x << 1) + (x << 3) + (ioc ^ '0');
    if (iosig) x = -x;
}
template <class T, size_t size>
struct MemoryPool {
    T buf[size], *tail, *st[size], *end;
    int top;
    MemoryPool() : top(0), tail(buf), end(buf + size) {}
    inline T *alloc() {
        if (top) return st[--top];
        if (tail != end) return tail++;
        return new T;
    }
    inline void recycle(T *x) { st[top++] = x; }
};
const int MAXN = 200005;
const int MAXM = 100;
int f[MAXN], fp[MAXN], n, m;
struct Data {
    int s, e, p, num;
    inline bool operator<(const Data &b) const { return p < b.p; }
} data[MAXN];
struct TimeTable {
    int time, value, flag;
    inline bool operator<(const TimeTable &b) const { return time < b.time; }
    TimeTable(const int time, const int value, const int flag)
        : time(time), value(value), flag(flag) {}
    TimeTable() {}
} table[MAXN << 1];
struct Node {
    Node *lc, *rc;
    long long cnt, sum;
    inline Node *clear() { return lc = rc = NULL, cnt = sum = 0, this; }
} * root[MAXN];
MemoryPool<Node, MAXM> pool;
inline Node *newNode(const long long cnt, const long long sum) {
    Node *p = pool.alloc()->clear();
    return p->cnt = cnt, p->sum = sum, p;
}
inline void build(Node *&p, const int l, const int r) {
    p = pool.alloc()->clear();
    if (l == r) return;
    register int mid = l + r >> 1;
    build(p->lc, l, mid), build(p->rc, mid + 1, r);
}
inline void update(const Node *pre, Node *&cur, const int l, const int r,
                   const int value, const int delta) {
    cur = newNode(pre->cnt + delta,
                  pre->sum + static_cast<long long>(f[value]) * delta);
    if (l == r) return;
    cur->lc = pre->lc, cur->rc = pre->rc;
    register int mid = l + r >> 1;
    if (value <= mid)
        update(pre->lc, cur->lc, l, mid, value, delta);
    else
        update(pre->rc, cur->rc, mid + 1, r, value, delta);
}
inline long long query(const Node *p, const int num, const int l, const int r) {
    if (num == p->cnt) return p->sum;
    if (l == r) return p->sum / p->cnt * num;
    register int mid = l + r >> 1;
    if (num <= p->lc->cnt)
        return query(p->lc, num, l, mid);
    else
        return p->lc->sum + query(p->rc, num - p->lc->cnt, mid + 1, r);
}
int main() {
    //    freopen("in.in", "r", stdin);
    read(m), read(n);
    for (register int i = 1; i <= m; i++)
        read(data[i].s), read(data[i].e), read(data[i].p), data[i].num = i;
    std::sort(data + 1, data + m + 1);
    register int size = 0;
    for (register int i = 1; i <= m; i++) {
        if (i == 1 || data[i].p != data[i - 1].p) f[++size] = data[i].p;
        fp[i] = size;
    }
    for (register int i = 1; i <= m; i++)
        table[(i << 1) - 1] = TimeTable(data[i].s, fp[i], 1),
                         table[i << 1] = TimeTable(data[i].e + 1, fp[i], -1);
    std::sort(table + 1, table + (m << 1 | 1));
    build(root[0], 1, n);
    register int t = 1;
    for (register int i = 1; i <= n; i++) {
        Node *pre = root[i - 1], *tmp = root[i - 1];
        for (; t <= (m << 1) && table[t].time == i; t++)
            update(pre, tmp, 1, n, table[t].value, table[t].flag), pre = tmp;
        root[i] = tmp;
    }
    register long long pre = 1;
    for (register int i = 1; i <= n; i++) {
        long long x, a, b, c, k;
        read(x), read(a), read(b), read(c), k = (a * pre + b) % c + 1;
        if (k > root[x]->cnt)
            pre = root[x]->sum;
        else if (k == 0)
            pre = 0;
        else
            pre = query(root[x], k, 1, n);
        std::cout << pre << "\n";
    }
    return 0;
}
/*
 * created by xehoth on 25-02-2017
 */
#include <bits/stdc++.h>
#include <tr1/unordered_map>

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
inline void read(T &x) {
    static bool iosig;
    static char c;
    for (iosig = false, c = read(); !isdigit(c); c = read()) {
        if (c == '-') iosig = true;
        if (c == -1) return;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
}

const int MAXN = 100010;

#define long long long

struct Data {
    int x, y, l, r;
    long sum;

    Data(const int x, const int y, const int l, const int r, const long sum)
        : x(x), y(y), l(l), r(r), sum(sum) {}

    inline bool operator<(const Data &x) const { return sum < x.sum; }
};

struct Node {
    long max, lazy;
    Node *lc, *rc;
    int id;
} * root[MAXN];

const int NODE_SIZE = sizeof(Node);

char pool[MAXN * 40 * NODE_SIZE], *cur = pool;

inline void build(const int l, const int r, Node *&rt) {
    rt = (Node *)(cur += NODE_SIZE), rt->id = l;
    if (l == r) return;
    register int mid = l + r >> 1;
    build(l, mid, rt->lc), build(mid + 1, r, rt->rc);
}

inline void copy(Node *a, Node *b) {
    a->lc = b->lc, a->rc = b->rc, a->lazy = b->lazy;
}

int s, t, d;

inline void insert(const int l, const int r, Node *&rt, Node *pre) {
    rt = (Node *)(cur += NODE_SIZE), copy(rt, pre);
    if (::s <= l && ::t >= r) {
        rt->max = pre->max + ::d, rt->lazy += ::d, rt->id = pre->id;
        return;
    }
    register int mid = l + r >> 1;
    if (::s <= mid) insert(l, mid, rt->lc, pre->lc);
    if (mid < ::t) insert(mid + 1, r, rt->rc, pre->rc);
    if (rt->lc->max >= rt->rc->max) {
        rt->max = rt->lc->max + rt->lazy, rt->id = rt->lc->id;
    } else {
        rt->max = rt->rc->max + rt->lazy, rt->id = rt->rc->id;
    }
}

typedef std::pair<int, long> Pair;

inline Pair query(const int l, const int r, Node *rt) {
    if (::s <= l && ::t >= r) return Pair(rt->id, rt->max);
    register int mid = l + r >> 1;
    if (::t <= mid) {
        Pair tmp = query(l, mid, rt->lc);
        tmp.second += rt->lazy;
        return tmp;
    } else if (::s > mid) {
        Pair tmp = query(mid + 1, r, rt->rc);
        tmp.second += rt->lazy;
        return tmp;
    } else {
        Pair tmpl = query(l, mid, rt->lc), tmpr = query(mid + 1, r, rt->rc);
        tmpl.second += rt->lazy, tmpr.second += rt->lazy;
        return tmpl.second >= tmpr.second ? tmpl : tmpr;
    }
}

int n, k, a[MAXN];

inline void solve() {
    read(n), read(k);
    build(1, n, root[0]);
    static std::priority_queue<Data> q;
    for (register int i = 1; i <= n; i++) {
        static std::tr1::unordered_map<int, int> last;
        read(a[i]);
        s = last[a[i]] + 1, t = i, d = a[i];
        insert(1, n, root[i], root[i - 1]);
        s = 1;
        Pair tmp = query(1, n, root[i]);
        q.push(Data(i, tmp.first, 1, i, tmp.second));
        last[a[i]] = i;
    }
    while (--k) {
        Data data = q.top();
        q.pop();
        if (data.l < data.y) {
            s = data.l, t = data.y - 1;
            Pair tmp = query(1, n, root[data.x]);
            q.push(Data(data.x, tmp.first, data.l, data.y - 1, tmp.second));
        }
        if (data.r > data.y) {
            s = data.y + 1, t = data.r;
            Pair tmp = query(1, n, root[data.x]);
            q.push(Data(data.x, tmp.first, data.y + 1, data.r, tmp.second));
        }
    }
    std::cout << q.top().sum;
}

int main() {
    solve();
    return 0;
}
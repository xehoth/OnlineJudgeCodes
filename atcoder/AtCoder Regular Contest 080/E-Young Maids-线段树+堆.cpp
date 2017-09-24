/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 080E」Young Maids 26-08-2017
 * 线段树 + 堆
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

const int MAXN = 200000;

int M;

inline void init(const int n) {
    for (M = 1; M < n + 2; M <<= 1)
        ;
}

struct SegmentTree {
    int d[MAXN * 4];

    inline void maintain(int k) { d[k] = std::min(d[k << 1], d[k << 1 | 1]); }

    inline void build() {
        for (register int i = M - 1; i; i--) maintain(i);
    }

    inline int query(int s, int t) const {
        register int ret = INT_MAX;
        for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
            (~s & 1) ? ret = std::min(ret, d[s ^ 1]) : 0;
            (t & 1) ? ret = std::min(ret, d[t ^ 1]) : 0;
        }
        return ret;
    }
};

SegmentTree even, odd;

int a[MAXN + 1], pos[MAXN + 1];

struct Node {
    int l, r, x;

    Node(int l, int r, int x) : l(l), r(r), x(x) {}

    inline bool operator<(const Node &b) const { return x > b.x; }
};

template <typename T>
class PriorityQueue : public std::priority_queue<T> {
   private:
#define super std::priority_queue<T>
   public:
    inline void reserve(const int n) { super::c.reserve(n); }
#undef super
};

inline void initRmq(const int n, const int *a) {
    init(n);
    for (register int i = 1; i <= n; i++) {
        if (i & 1) {
            odd.d[M + i] = a[i], even.d[M + i] = INT_MAX;
        } else {
            even.d[M + i] = a[i], odd.d[M + i] = INT_MAX;
        }
    }
    even.build(), odd.build();
}

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n;
    std::cin >> n;
    for (register int i = 1; i <= n; i++) std::cin >> a[i], pos[a[i]] = i;
    initRmq(n, a);
    static PriorityQueue<Node> q;
    q.reserve(n * 2);
    q.push(Node(1, n, odd.query(1, n)));
    const SegmentTree *t[2] = {&even, &odd};
    while (!q.empty()) {
        Node d = q.top();
        q.pop();
        register int l = d.l, r = d.r;
        register int p1 = pos[d.x], p2 = pos[t[~p1 & 1]->query(p1 + 1, r)];
        std::cout << a[p1] << ' ' << a[p2] << ' ';
        if (l < p1) q.push(Node(l, p1 - 1, t[l & 1]->query(l, p1 - 1)));
        if (p1 < p2 - 1)
            q.push(Node(p1 + 1, p2 - 1, t[~p1 & 1]->query(p1 + 1, p2 - 1)));
        if (p2 < r) q.push(Node(p2 + 1, r, t[~p2 & 1]->query(p2 + 1, r)));
    }
}
}

int main() {
    Task::solve();
    return 0;
}
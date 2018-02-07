/*
 * created by xehoth on 29-04-2017
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

}  // namespace IO

namespace Task {

const int MAXN = 100005;

#define long long long

struct Node {
    int x, y, id, val;
    long ans;

    inline bool operator<(const Node &p) const { return x < p.x; }
} q[MAXN << 2], node[MAXN];

long d[MAXN << 2], ans;

int tot, xcnt, ycnt;

inline void modify(int k, int v) {
    for (; k <= ycnt; k += k & -k) d[k] += v;
}

inline long query(int k) {
    register long ret = 0;
    for (; k; k ^= k & -k) ret += d[k];
    return ret;
}

inline bool cmp(Node a, Node b) { return a.id < b.id; }

int n, m;

inline void init() {
    using namespace IO;
    static int xVal[MAXN << 2], yVal[MAXN << 2];
    read(n), read(m);
    xcnt = n, ycnt = n;

    for (register int i = 1; i <= n; i++) {
        read(node[i].x), read(node[i].y), node[i].id = i, read(node[i].val);
        xVal[i] = node[i].x, yVal[i] = node[i].y;
    }

    for (register int i = 1, x1, x2, y1, y2; i <= m; i++) {
        read(x1), read(y1), read(x2), read(y2);
        xVal[++xcnt] = x1, xVal[++xcnt] = x2, yVal[++ycnt] = y1,
        yVal[++ycnt] = y2, xVal[++xcnt] = x1 - 1, yVal[++ycnt] = y1 - 1;

        q[++tot].x = x1 - 1, q[tot].y = y1 - 1, q[tot].id = i, q[tot].val = 1,
        q[++tot].x = x1 - 1, q[tot].y = y2, q[tot].id = i, q[tot].val = -1,
        q[++tot].x = x2, q[tot].y = y1 - 1, q[tot].id = i, q[tot].val = -1,
        q[++tot].x = x2, q[tot].y = y2, q[tot].id = i, q[tot].val = 1;
    }

    std::sort(xVal + 1, xVal + xcnt + 1);
    std::sort(yVal + 1, yVal + ycnt + 1);
    xcnt = std::unique(xVal + 1, xVal + xcnt + 1) - (xVal + 1);
    ycnt = std::unique(yVal + 1, yVal + ycnt + 1) - (yVal + 1);

    for (register int i = 1; i <= n; i++) {
        node[i].x =
            std::lower_bound(xVal + 1, xVal + xcnt + 1, node[i].x) - xVal,
        node[i].y =
            std::lower_bound(yVal + 1, yVal + ycnt + 1, node[i].y) - yVal;
    }

    for (register int i = 1; i <= tot; i++) {
        q[i].x = std::lower_bound(xVal + 1, xVal + xcnt + 1, q[i].x) - xVal,
        q[i].y = std::lower_bound(yVal + 1, yVal + ycnt + 1, q[i].y) - yVal;
    }

    std::sort(q + 1, q + tot + 1);
    std::sort(node + 1, node + n + 1);
}

inline void solve() {
    using namespace IO;
    init();

    for (Node *p = node + 1, *r = node + n, *i = q + 1, *ir = q + tot; i <= ir;
         i++) {
        for (; p <= r && p->x <= i->x; p++) modify(p->y, p->val);
        i->ans = query(i->y);
    }

    std::sort(q + 1, q + tot + 1, cmp);

    Node *p = q;
    for (register int i = 1; i <= m; i++) {
        ans = (p + 1)->val * (p + 1)->ans + (p + 2)->val * (p + 2)->ans +
              (p + 3)->val * (p + 3)->ans + (p + 4)->val * (p + 4)->ans;
        p += 4;
        print(ans), print('\n');
    }
}
}  // namespace Task

int main() {
    // freopen("task.in", "r", stdin);
    // freopen("task.out", "w", stdout);
    Task::solve();
    IO::flush();
    return 0;
}

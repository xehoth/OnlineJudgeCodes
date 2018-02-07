#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
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

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = 0;
    return s;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace PairingHeap {

const int MAXN = 1000000;

struct Node {
    int num, v;
    inline bool operator<(const Node &b) const { return v > b.v; }

    Node(int num, int v) : num(num), v(v) {}
};

typedef __gnu_pbds::priority_queue<Node> PriorityQueue;

PriorityQueue q[MAXN + 1];

int fa[MAXN + 1];

inline int get(int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }

bool vis[MAXN + 1];
char s[10];

inline void solve() {
    using namespace IO;
    register int n;
    read(n);
    for (register int i = 1, x; i <= n; i++) {
        read(x), q[i].push(Node(i, x));
        fa[i] = i;
    }
    register int m;
    read(m);
    for (int i = 1; i <= m; i++) {
        read(s);
        if (s[0] == 'M') {
            register int l, r;
            read(l), read(r);
            if (vis[l] || vis[r]) continue;
            register int o = get(l), p = get(r);
            if (o != p) fa[o] = p, q[p].join(q[o]);
        } else if (s[0] == 'K') {
            register int x;
            read(x);
            if (vis[x])
                print("0\n");
            else {
                register int o = get(x);
                vis[q[o].top().num] = true, print(q[o].top().v), print('\n');
                q[o].pop();
            }
        }
    }
}
}  // namespace PairingHeap

int main() {
    PairingHeap::solve();
    IO::flush();
    return 0;
}
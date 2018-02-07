#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>

using namespace __gnu_pbds;

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
}

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
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

struct InputOutputStream {
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

    ~InputOutputStream() { flush(); }
} io;
}  // namespace IO

namespace {

const int MAXN = 150;
const int MAXM = 150;

int n, m, a[MAXN][MAXM], b[MAXN][MAXM];

struct Data;

struct Node {
    int x, y, k;

    Node(int x = 0, int y = 0, int k = 0) : x(x), y(y), k(k) {}

    inline Data *operator->() const;

    inline Node up(int k) { return Node(x, y, k); }

    inline Node move(int x, int y) {
        return Node(this->x + x, this->y + y, k - 1);
    }

    inline bool isValid() {
        return x >= 0 && x < n && y >= 0 && y < m && k >= 0;
    }

    inline int a() { return ::a[x][y]; }

    inline int b() { return ::b[x][y]; }

    inline bool operator<(const Node &p) const;
} p[3];

typedef priority_queue<Node> PriorityQueue;

struct Data {
    int dist;
    PriorityQueue::point_iterator id;
} nodes[MAXN][MAXM][MAXN + MAXM + 1];

inline Data *Node::operator->() const { return &nodes[x][y][k]; }

inline bool Node::operator<(const Node &p) const {
    return nodes[x][y][k].dist > p->dist;
}

inline bool tense(register int &a, const register int b) {
    return a > b ? (a = b, true) : false;
}

PriorityQueue q;

inline void push(const Node &p) {
    if (p->id != NULL)
        q.modify(p->id, p);
    else
        p->id = q.push(p);
}

inline void dijkstra(const Node &start) {
    for (register int i = 0; i < n; i++)
        for (register int j = 0; j < m; j++)
            for (register int k = 0; k <= n + m; k++)
                nodes[i][j][k].dist = INT_MAX, nodes[i][j][k].id = NULL;
    start->dist = 0;
    start->id = q.push(start);
    for (register Node p, side, up; !q.empty();) {
        p = q.top(), q.pop();
        if (p->dist > ::p[0]->dist && p->dist > ::p[1]->dist &&
            p->dist > ::p[2]->dist)
            continue;
        if (p.k == 0) {
            up = p.up(p.b());
            if (tense(up->dist, p->dist + p.a())) push(up);
        } else {
            if ((side = p.move(0, 1)).isValid() && tense(side->dist, p->dist))
                push(side);
            if ((side = p.move(0, -1)).isValid() && tense(side->dist, p->dist))
                push(side);
            if ((side = p.move(1, 0)).isValid() && tense(side->dist, p->dist))
                push(side);
            if ((side = p.move(-1, 0)).isValid() && tense(side->dist, p->dist))
                push(side);
            if ((side = p.move(0, 0)).isValid() && tense(side->dist, p->dist))
                push(side);
        }
    }
}

using IO::io;

inline void solve() {
    io >> n >> m;
    for (register int i = 0; i < n; i++)
        for (register int j = 0; j < m; j++)
            io >> b[i][j], b[i][j] > n + m ? b[i][j] = n + m : 0;
    for (register int i = 0; i < n; i++)
        for (register int j = 0; j < m; j++) io >> a[i][j];
    for (register int i = 0; i < 3; i++)
        io >> p[i].x >> p[i].y, p[i].x--, p[i].y--;
    register int dist[3][3];
    for (register int i = 0; i < 3; i++) {
        dijkstra(p[i]);
        for (register int j = 0; j < 3; j++) dist[i][j] = p[j]->dist;
    }
    register int ans = INT_MAX;
    char ansPos = '\0';
    for (register int i = 0; i < 3; i++) {
        if (dist[0][i] == INT_MAX || dist[1][i] == INT_MAX ||
            dist[2][i] == INT_MAX)
            continue;
        register int tmp = dist[0][i] + dist[1][i] + dist[2][i];
        if (tmp < ans) ans = tmp, ansPos = 'X' + i;
    }
    if (ans == INT_MAX)
        io << "NO";
    else
        io << ansPos << '\n' << ans;
}
}  // namespace

int main() {
    solve();
    return 0;
}
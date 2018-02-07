#include <bits/stdc++.h>

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

using namespace IO;

const int MAXN = 100000;
const int MAXM = 7000000;
template <class T>
struct Queue {
    T a[MAXN + MAXM], *l, *r;
    Queue() : l(a), r(a - 1) {}
    inline void push(const T &x) { *++r = x; }
    inline void pop() { l++; }
    inline T &front() { return *l; }
    inline bool empty() const { return l > r; }
};
Queue<int> q[3];
inline int getMax() {
    register int res = -1;
    for (register int i = 0; i < 3; i++)
        if (!q[i].empty() && (res == -1 || q[i].front() > q[res].front()))
            res = i;
    return res;
}
inline void solve() {
    register int n, m, k, u, v, t;
    read(n), read(m), read(k), read(u), read(v), read(t);
    static int a[MAXN + 1];
    for (register int i = 1; i <= n; i++) read(a[i]);
    std::sort(a + 1, a + n + 1);
    for (register int i = n; i >= 1; i--) q[0].push(a[i]);
    register int d = 0;
    for (register int i = 1; i <= m; i++) {
        register int j = getMax();
        register int x = q[j].front();
        q[j].pop();
        x += d;
        if (i % t == 0) io << x, i + t <= m ? print(' ') : (void)0;
        register int a = static_cast<long long>(x) * u / v, b = x - a;
        d += k;
        a -= d, b -= d;
        q[1].push(a), q[2].push(b);
    }
    io << '\n';
    for (register int i = 1; i <= n + m; i++) {
        register int j = getMax();
        register int x = q[j].front();
        q[j].pop();
        x += d;
        if (i % t == 0) io << x, i + t <= n + m ? print(' ') : (void)0;
    }
}
int main() {
    // freopen("in.in", "r", stdin);
    solve();
    return 0;
}
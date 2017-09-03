/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「NOIP 2008」双栈排序 03-09-2017
 * 二分图染色
 * @author xehoth
 */
#include <bits/stdc++.h>

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
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
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
}

namespace {

using IO::io;

const int MAXN = 1000;

struct Graph {
    typedef std::vector<int> Vector;

    Vector edge[MAXN + 1];

    inline Vector &operator[](const int i) { return edge[i]; }

    inline void addEdge(const int u, const int v) {
        edge[u].push_back(v), edge[v].push_back(u);
    }
};

struct Task {
    Graph g;

    typedef Graph::Vector::iterator Iterator;
    typedef char byte;

    int a[MAXN + 1], min[MAXN + 1], n;

    byte color[MAXN + 1];

    inline bool dfs(const int u, const byte c) {
        color[u] = c;
        for (Iterator v = g[u].begin(); v != g[u].end(); v++)
            if (color[u] == color[*v] || (color[*v] == 0 && !dfs(*v, -c)))
                return false;
        return true;
    }

    std::vector<int> A, B;
    std::vector<char> ans;

    static inline void print(char c) { io << c << ' '; }

    inline void check(int i) {
        std::vector<int> vec;
        static int now;
        for (;;) {
            register bool isUpdated = false;
            if (!A.empty() && A.back() == now)
                A.pop_back(), now++, vec.push_back('b'), isUpdated = true;
            if (!B.empty() && B.back() == now)
                B.pop_back(), now++, vec.push_back('d'), isUpdated = true;
            if (!isUpdated) break;
        }
        if (i + 1 < n && color[i + 1] == -1) {
            for (register int i = vec.size() - 1; i >= 0; --i)
                if (vec[i] == 'b') {
                    register int len = vec.size() - i - 1;
                    for (register int k = now - 1; k > now - 1 - len; --k)
                        B.push_back(k);
                    now -= len;
                    vec.resize(vec.size() - len);
                    break;
                }
        }
        ans.insert(ans.end(), vec.begin(), vec.end());
    }

    inline void solve(const int n) {
        A.reserve(n + 1), B.reserve(n + 1), ans.reserve(n + 1);
        for (register int i = 0; i < n; i++) {
            if (color[i] == -1) {
                A.push_back(a[i]), ans.push_back('a'), check(i);
            } else if (color[i] == 1) {
                B.push_back(a[i]), ans.push_back('c'), check(i);
            }
        }
        std::for_each(ans.begin(), ans.end(), print);
    }

    inline void solve() {
        io >> n;
        for (register int i = 0; i < n; i++) io >> a[i], a[i]--;
        min[n] = n;
        for (register int i = n - 1; i >= 0; i--)
            min[i] = std::min(min[i + 1], a[i]);
        for (register int i = 0; i < n; i++) {
            for (register int j = i + 1; j < n; j++) {
                if (a[i] < a[j] && min[j + 1] < a[i]) {
                    g.addEdge(i, j);
                }
            }
        }
        for (register int i = 0; i < n; i++) {
            if (color[i] == 0 && !dfs(i, -1)) {
                io << "0";
                return;
            }
        }
        solve(n);
    }
} task;
}

int main() {
    task.solve();
    return 0;
}

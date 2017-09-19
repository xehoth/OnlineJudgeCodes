/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「POJ 2932」Coneology 19-09-2017
 * 扫描线
 * @author xehoth
 */
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <set>
#include <vector>

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

inline void read(double &x) {
    static char buf[40];
    read(buf);
    sscanf(buf, "%lf", &x);
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

const int MAXN = 40000;

struct Task {
    inline bool check(const int i, const int j) {
        return r[i] <= r[j] &&
               (x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]) <=
                   (r[i] - r[j]) * (r[i] - r[j]);
    }

    typedef std::pair<double, int> Pair;
    typedef std::vector<Pair> Vector;
    typedef std::set<Pair> Set;
    Vector d;
    Set s;

    double r[MAXN + 1], x[MAXN + 1], y[MAXN + 1];

    inline void solve() {
        register int n;
        io >> n;
        for (register int i = 0; i < n; i++) io >> r[i] >> x[i] >> y[i];
        d.reserve(2 * n);
        for (register int i = 0; i < n; i++) {
            d.push_back(Pair(x[i] - r[i], i));
            d.push_back(Pair(x[i] + r[i], i + n));
        }
        std::sort(d.begin(), d.end());
        static std::vector<int> res;
        res.reserve(n);
        for (Vector::iterator p = d.begin(); p != d.end(); p++) {
            if (p->second < n) {
                Set::iterator it = s.lower_bound(Pair(y[p->second], p->second));
                if ((it != s.end() && check(p->second, it->second)) ||
                    (it != s.begin() && check(p->second, (--it)->second)))
                    continue;
                res.push_back(p->second);
                s.insert(Pair(y[p->second], p->second));
            } else {
                s.erase(Pair(y[p->second - n], p->second - n));
            }
        }
        std::sort(res.begin(), res.end());
        io << res.size() << '\n';
        for (register size_t i = 0; i < res.size(); i++)
            io << res[i] + 1 << ' ';
    }
} task;
}

int main() {
    task.solve();
    return 0;
}
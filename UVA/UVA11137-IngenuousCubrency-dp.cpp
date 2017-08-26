/**
 * 「UVA 11137」Ingenuous Cubrency 12-06-2017
 * dp
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
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}

namespace Task {

const int MAXN = 10000;

unsigned long long f[MAXN] = {1}, max;

const int CUBE[22] = {0,    1,    8,    27,   64,   125,  216,  343,
                      512,  729,  1000, 1331, 1728, 2197, 2744, 3375,
                      4096, 4913, 5832, 6859, 8000, 9261};

inline void init(int n) {
    for (register int i = 1; i <= 21; i++)
        for (register int v = CUBE[i]; v <= n; v++) f[v] += f[v - CUBE[i]];
}

inline void print(int n) { IO::print(f[n]), IO::print('\n'); }

inline void solve() {
    static std::vector<int> q;
    q.reserve(MAXN);
    for (register int n; IO::read(n);) q.push_back(n);
    init(*std::max_element(q.begin(), q.end()));
    std::for_each(q.begin(), q.end(), print);
}
}

int main() {
    // freopen("in.in", "r", stdin);
    Task::solve();
    IO::flush();
    return 0;
}
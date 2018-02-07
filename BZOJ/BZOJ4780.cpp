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
    while (c = read(), !isspace(c));
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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace Task {

const int MAXN = 100010;

int a[MAXN + 1];
int l[MAXN + 1], r[MAXN + 1];

std::vector<int> st;

inline void solve() {
    using namespace IO;
    register int n;
    read(n);
    memset(r, -1, sizeof(int) * (n + 2));
    memset(l, 127, sizeof(int) * (n + 2)), r[0] = n + 1, l[0] = 0;

    for (register int i = 1; i <= n; i++) {
        read(a[i]);
        l[a[i]] = std::min(l[a[i]], i);
        r[a[i]] = std::max(r[a[i]], i);
    }
    st.reserve(n + 1);
    register int ans = 0;
    for (register int i = 0; i <= n + 1; i++) {
        if (i == l[a[i]])
            st.push_back(a[i]), ans = std::max(ans, (int)st.size() - 1);
        if (st.back() != a[i]) {
            print(-1);
            return;
        }
        if (i == r[a[i]]) st.pop_back();
    }
    print(ans);
}
}  // namespace Task

int main() {
    // freopen("art2.in", "r", stdin);
    // freopen("art2.out", "w", stdout);
    Task::solve();
    IO::flush();
    return 0;
}

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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace {

using namespace IO;

const int MAX_SIGMA = 3;
const int MAXN = 60010;

struct Node {
    int c[MAX_SIGMA], fail, pos;
} d[MAXN + 1];

int cur, last;

int l[MAXN + 1], r[MAXN + 1], same[MAXN + 1], idx;
int vis[MAXN + 1], in[MAXN + 1], cnt[MAXN + 1];
char s[100010 + 1], buf[MAXN + 1];

inline void add(const char *s) {
    for (last = 0; *s; s++) {
        if (!d[last].c[*s - 'a']) d[last].c[*s - 'a'] = ++cur;
        last = d[last].c[*s - 'a'];
    }
}

inline void check(char c) {
    for (register int p = d[last].c[c]; p && vis[p] != idx;
         vis[p] = idx, p = d[p].fail) {
        if (d[p].pos && in[d[p].pos] != idx)
            cnt[d[p].pos]++, in[d[p].pos] = idx;
    }
    last = d[last].c[c];
}

inline void build() {
    static std::queue<int> q;
    q.push(0);
    for (register int p, v; !q.empty();) {
        p = q.front(), q.pop();
        for (register int i = 0; i < MAX_SIGMA; i++) {
            if (v = d[p].c[i]) {
                if (p) d[v].fail = d[d[p].fail].c[i];
                q.push(v);
            } else {
                d[p].c[i] = d[d[p].fail].c[i];
            }
        }
    }
}

inline void solve() {
    register int n, m;
    read(n), read(m);
    for (register int i = 1; i <= n; i++)
        l[i] = r[i - 1] + 1, r[i] = r[i - 1] + read(s + l[i]);
    for (register int i = 1; i <= m; i++) {
        read(buf), add(buf);
        d[last].pos ? same[i] = d[last].pos : d[last].pos = i;
    }
    build();
    for (register int i = 1; i <= n; i++) {
        idx++, last = 0;
        for (register int j = l[i]; j <= r[i]; j++) check(s[j] - 'a');
    }
    for (register int i = 1; i <= m; i++)
        print(cnt[same[i] ? same[i] : i]), print('\n');
    flush();
}
}  // namespace

int main() {
    solve();
    return 0;
}
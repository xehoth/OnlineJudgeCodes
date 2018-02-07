#include <bits/stdc++.h>
#include <tr1/unordered_set>

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

const int MAXN = 500;

typedef unsigned int uint;

uint hashS[MAXN + 1][MAXN + 1], hashT[MAXN + 1][MAXN + 1], pow31[MAXN + 1];
char s[MAXN + 1][MAXN + 1], t[MAXN + 1][MAXN + 1];
int n, m;

const uint HASH_BASE = 31;

inline uint hash(const uint *h, int l, int r) {
    return h[r] - h[l - 1] * pow31[r - l + 1];
}

inline void initPow(const int n) {
    pow31[0] = 1, pow31[1] = HASH_BASE;
    for (register int i = 2; i <= n; i++) pow31[i] = pow31[i - 1] * HASH_BASE;
}

std::tr1::unordered_set<uint> set;
// std::set<uint> set;

inline bool check(int len) {
    for (register int i = 1; i + len - 1 <= m; i++) {
        set.clear();
        for (register int j = 1; j <= n; j++) {
            set.insert(hash(hashS[j], i, i + len - 1));
        }
        register bool flag = true;
        for (register int j = 1; j <= n; j++) {
            if (set.find(hash(hashT[j], i, i + len - 1)) != set.end()) {
                flag = false;
                break;
            }
        }
        if (flag) return true;
    }
    return false;
}

inline void solve() {
    using namespace IO;
    read(n), read(m);
    initPow(m);
    for (register int i = 1; i <= n; i++) read(s[i] + 1);
    for (register int i = 1; i <= n; i++) read(t[i] + 1);
    for (register int i = 1; i <= n; i++) {
        for (register int j = 1; j <= m; j++) {
            hashS[i][j] = hashS[i][j - 1] * HASH_BASE + s[i][j];
            hashT[i][j] = hashT[i][j - 1] * HASH_BASE + t[i][j];
        }
    }
    register int l = 0, r = m + 1;
    while (r - l > 1) {
        register int mid = l + r >> 1;
        if (check(mid)) {
            r = mid;
        } else {
            l = mid;
        }
    }
    if (r - 2 > 0 && check(r - 2)) {
        print(r - 2);
    } else if (r - 1 > 0 && check(r - 1)) {
        print(r - 1);
    } else {
        print(r);
    }
}
}  // namespace Task

int main() {
    // freopen("cownomics.in", "r", stdin);
    // freopen("cownomics.out", "w", stdout);
    Task::solve();
    IO::flush();
    return 0;
}
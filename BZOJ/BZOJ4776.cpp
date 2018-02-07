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

const int MAXN = 1002;
const int MAX_COLOR = MAXN * MAXN;

struct Node {
    int x1, y1, x2, y2;

    Node() : x1(INT_MAX), y1(INT_MAX), x2(0), y2(0) {}

    inline void maintain(int x, int y) {
        this->x1 = std::min(this->x1, x), this->x2 = std::max(this->x2, x);
        this->y1 = std::min(this->y1, y), this->y2 = std::max(this->y2, y);
    }
} d[MAX_COLOR + 1];

int n, tot, color[MAXN + 1][MAXN + 1], cnt[MAX_COLOR + 1];
int f[MAXN + 1][MAXN + 1], g[MAXN + 1][MAXN + 1];

inline void solve() {
    using namespace IO;
    read(n);
    for (register int i = 0; i < n; i++)
        for (register int j = 0; j < n; j++) read(color[i][j]);
    for (register int i = 0; i < n; i++) {
        for (register int j = 0, c; j < n; j++) {
            if ((c = color[i][j]) > 0) {
                if (cnt[c] == 0) tot++;
                cnt[c]++, d[c].maintain(i + 1, j + 1);
            }
        }
    }
    if (tot == 1) {
        print(n == 1 ? 0 : n * n - 1);
        return;
    }
    int ans = n * n - tot;
    for (register int c = 1; c <= n * n; c++) {
        if (c > 0 && cnt[c] > 0) {
            f[d[c].x2][d[c].y2]++, f[d[c].x2][d[c].y1 - 1]--;
            f[d[c].x1 - 1][d[c].y2]--, f[d[c].x1 - 1][d[c].y1 - 1]++;
        }
    }
    g[n][n] = f[n][n];
    for (register int i = n - 1; i; i--) g[n][i] = g[n][i + 1] + f[n][i];
    for (register int i = n - 1; i >= 1; i--) {
        g[i][n] = g[i + 1][n] + f[i][n];
        for (register int j = n - 1; j >= 1; j--)
            g[i][j] = g[i + 1][j] + g[i][j + 1] - g[i + 1][j + 1] + f[i][j];
    }
    for (register int i = 1; i <= n; i++)
        for (register int j = 1, c; j <= n; j++)
            if ((c = color[i - 1][j - 1]) > 0 && cnt[c] > 0 && g[i][j] >= 2)
                cnt[c] = 0;
    for (register int c = 1; c <= n * n; c++)
        if (cnt[c] > 0) ans++;
    print(ans);
}
}  // namespace Task

int main() {
    // freopen("art.in", "r", stdin);
    // freopen("art.out", "w", stdout);
    Task::solve();
    IO::flush();
    return 0;
}
/*
 * created by xehoth on 30-04-2017
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <class T>
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

const int OUT_LEN = 550000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) { *oh++ = c; }

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

const int MAXN = 100010;

int n, m, blockSize, pos[MAXN], b[MAXN];

struct Node {
    int op, l, r, x, id;
    inline bool operator<(const Node &rhs) const {
        return pos[l] < pos[rhs.l] || pos[l] == pos[rhs.l] && r < rhs.r;
    }

    inline void init(const int id) {
        using namespace IO;
        read(op), read(l), read(r), read(x), this->id = id;
    }
} a[MAXN];

std::bitset<100001> f, g;
int cnt[MAXN], res[MAXN];
int main() {
    // freopen("in.in", "r", stdin);
    using namespace IO;
    read(n), read(m);
    register int blockSize = sqrt(n);
    for (register int i = 1; i <= n; i++) read(b[i]);
    for (register int i = 1; i <= m; i++) a[i].init(i);

    for (register int i = 1; i <= n; i++) pos[i] = (i - 1) / blockSize + 1;
    std::sort(a + 1, a + m + 1);
    register int l = 0, r = 0;
    for (register int i = 1; i <= m; i++) {
        while (l > a[i].l) {
            l--, cnt[b[l]]++, f[b[l]] = 1, g[100000 - b[l]] = 1;
        }
        while (r < a[i].r) {
            r++, cnt[b[r]]++, f[b[r]] = 1, g[100000 - b[r]] = 1;
        }
        while (l < a[i].l) {
            cnt[b[l]]--;
            (!cnt[b[l]]) ? (f[b[l]] = 0, g[100000 - b[l]] = 0) : 0;
            l++;
        }
        while (r > a[i].r) {
            cnt[b[r]]--;
            (!cnt[b[r]]) ? (f[b[r]] = 0, g[100000 - b[r]] = 0) : 0;
            r--;
        }
        if (a[i].op == 1) {
            (!((f >> a[i].x) & f).any()) ? res[a[i].id] = 0 : res[a[i].id] = 1;
        } else if (a[i].op == 2) {
            (!((g >> (100000 - a[i].x)) & f).any()) ? res[a[i].id] = 0
                                                    : res[a[i].id] = 1;
        } else {
            for (register int j = 1; j * j <= a[i].x; j++) {
                if (a[i].x % j == 0) {
                    if (f[j] == 1 && f[a[i].x / j] == 1) {
                        res[a[i].id] = 1;
                        break;
                    }
                }
            }
            (a[i].x == 0 && f[0]) ? res[a[i].id] = 1 : 0;
        }
    }
    for (register int i = 1; i <= m; i++) {
        res[i] ? (print('y'), print('u'), print('n'), print('o'))
               : (print('y'), print('u'), print('m'), print('i'));
        print('\n');
    }
    flush();
    return 0;
}

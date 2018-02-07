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

typedef unsigned int uint;
const int MAXN = 100000;

int prime[MAXN + 10], tot, n, m, max;
bool vis[MAXN + 10];
int t, id[MAXN + 10], sigma[MAXN + 10], mul[MAXN + 10], cnt[MAXN + 10];
uint mu[MAXN + 10], ans[MAXN + 10];

inline void fastLinearSieveMethod(const int n) {
    sigma[1] = mu[1] = 1;
    for (register int i = 2; i <= n; i++) {
        if (!vis[i]) {
            prime[tot++] = i, mu[i] = -1, cnt[i] = 1;
            mul[i] = i + 1, sigma[i] = i + 1;
        }
        for (register int j = 0, tmp; j < tot && (tmp = i * prime[j]) <= n;
             j++) {
            vis[tmp] = true;
            if (i % prime[j] == 0) {
                mu[tmp] = 0, cnt[tmp]++, mul[tmp] = mul[i] * prime[j] + 1;
                sigma[tmp] = sigma[i] / mul[i] * mul[tmp];
                break;
            } else {
                mu[tmp] = -mu[i], cnt[tmp] = 1, mul[tmp] = prime[j] + 1;
                sigma[tmp] = (prime[j] + 1) * sigma[i];
            }
        }
    }
}

struct BinaryIndexedTree {
    uint d[MAXN + 10];

    inline void modify(int x, uint v) {
        for (; x <= max; x += x & -x) d[x] += v;
    }

    inline uint query(int x) {
        register uint ret = 0;
        for (; x; x ^= x & -x) ret += d[x];
        return ret;
    }
} bit;

struct Query {
    int l, n, m, id;

    inline bool operator<(const Query &b) const { return l < b.l; }
} que[MAXN + 10];

inline bool cmp(const int x, const int y) { return sigma[x] < sigma[y]; }

inline void solve() {
    using namespace IO;
    read(t);
    for (register int i = 1; i <= t; i++) {
        read(que[i].n), read(que[i].m), read(que[i].l), que[i].id = i;
        max = std::max(max, std::max(que[i].n, que[i].m));
    }
    fastLinearSieveMethod(max);
    for (register int i = 1; i <= max; i++) id[i] = i;
    std::sort(id + 1, id + max, cmp);

    std::sort(que + 1, que + t + 1);
    for (register int i = 1, p = 1; i <= t; i++) {
        while (p <= max && sigma[id[p]] <= que[i].l) {
            for (register int x = id[p]; x <= max; x += id[p])
                bit.modify(x, sigma[id[p]] * mu[x / id[p]]);
            p++;
        }
        n = que[i].n, m = que[i].m, n > m ? std::swap(n, m) : (void)0;
        for (register int p = 1, q; p <= n; p = q + 1) {
            q = std::min(n / (n / p), m / (m / p));
            ans[que[i].id] += (uint)(n / p) * (uint)(m / p) *
                              (bit.query(q) - bit.query(p - 1));
        }
    }
    for (register int i = 1; i <= t; i++)
        print(ans[i] & ((uint)(1 << 31) - 1)), print('\n');
}
}  // namespace Task

int main() {
    Task::solve();
    IO::flush();
    return 0;
}
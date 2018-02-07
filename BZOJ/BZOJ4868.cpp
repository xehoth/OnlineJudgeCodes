/*
 * created by xehoth on 29-04-2017
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

}  // namespace IO

namespace Task {

const int MAXN = 100005;

#define long long long

inline void solve() {
    using namespace IO;
    register long a, b, c;
    register int n, m, r = 0;
    read(a), read(b), read(c), read(n), read(m);
    a = std::min(a, b);
    static int buc[MAXN];
    for (register int i = 1, x; i <= n; i++)
        read(x), buc[x]++, r = std::max(r, x);

    static int cnt[MAXN];
    for (register int i = 1, x; i <= m; i++)
        read(x), cnt[x]++, r = std::max(r, x);

    static long sum1[MAXN], sum2[MAXN], w1[MAXN], w2[MAXN], w[MAXN];
    for (register int i = 1; i <= r; i++) {
        buc[i] += buc[i - 1], sum1[i] = sum1[i - 1] + cnt[i];
        w[i] = w[i - 1] + buc[i - 1], w1[i] = w1[i - 1] + sum1[i - 1];
    }

    for (register int i = r; i; i--)
        sum2[i] = sum2[i + 1] + cnt[i], w2[i] = w2[i + 1] + sum2[i + 1];

    if (c > 1e12) {
        register int p = 0;
        for (register int i = 1;; i++) {
            if (buc[i]) {
                p = i;
                break;
            }
        }
        if (w1[p] >= w2[p])
            std::cout << a * w2[p];
        else
            std::cout << a * w1[p] + b * (w2[p] - w1[p]);
    } else {
        register long ans = LLONG_MAX, now;
        for (int i = 1; i <= r; i++) {
            now = c * w[i];
            w1[i] >= w2[i] ? now += a * w2[i]
                           : now += a * w1[i] + b * (w2[i] - w1[i]);
            ans = std::min(ans, now);
        }
        std::cout << ans;
    }
}
}  // namespace Task

int main() {
    // freopen("in.in", "r", stdin);
    Task::solve();
    return 0;
}

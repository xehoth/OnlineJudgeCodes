/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1044」木棍分割 26-10-2017
 *
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

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            c == '-' ? iosig = true : 0;
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        return iosig ? x = -x : 0, *this;
    }
} io;
}  // namespace IO

namespace {

using IO::io;

const int MOD = 10007;
const int MAXN = 50000;

int sum[MAXN + 1], L[MAXN + 1], m, n;
int f[2][MAXN], last[MAXN], sumF[MAXN];

inline bool check(const int mid) {
    register int cnt = 0, tmp;
    for (register int i = 1; i <= n; i++) {
        tmp += L[i];
        while (tmp <= mid && i <= n) i++, tmp += L[i];
        if (tmp > mid) i--;
        if (i > n) break;
        if (cnt > m) return false;
        tmp = 0, cnt++;
    }
    return cnt <= m;
}

inline void solve() {
    register int max = 0;
    io >> n >> m;
    for (register int i = 1; i <= n; i++)
        io >> L[i], sum[i] += sum[i - 1] + L[i], max = std::max(max, L[i]);
    register int l = max - 1, r = sum[n];
    for (register int mid; r - l > 1;) {
        if (check(mid = l + r >> 1))
            r = mid;
        else
            l = mid;
    }
    max = r;
    for (int i = 1, l = 0; i <= n; i++) {
        while (sum[i] - sum[l] > max) l++;
        last[i] = std::max(l - 1, 0);
    }
    register int now = 0, pre = 1, ans = 0;
    for (register int i = 1; i <= n && sum[i] <= max; i++) f[0][i] = 1;
    for (int i = 1; i <= n; i++) sumF[i] = (sumF[i - 1] + f[0][i]) % MOD;
    ans += f[0][n];
    for (register int i = 1; i <= m; i++) {
        for (register int j = 1; j <= n; j++) {
            f[now][j] = (sumF[j - 1] - sumF[last[j]]) % MOD;
        }
        sumF[0] = 0;
        for (register int j = 1; j <= n; j++)
            sumF[j] = (sumF[j - 1] + f[now][j]) % MOD;
        ans = (ans + f[now][n]) % MOD;
    }
    std::cout << max << ' ' << (ans + MOD) % MOD;
}
}  // namespace

int main() {
    solve();
    return 0;
}
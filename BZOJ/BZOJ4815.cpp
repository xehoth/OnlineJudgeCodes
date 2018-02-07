/*
 * created by xehoth on 26-04-2017
 */
#include <bits/stdc++.h>

namespace {

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

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), 0) : 0;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace

namespace SharedData {

const int MOD = 1e9 + 7;

inline int fixUp(int x) { return x < MOD ? x : x - MOD; }

inline int fixDown(int x) { return x < 0 ? x + MOD : x; }

inline void add(int &x, int v) { x = fixUp(x + v); }

inline void add(int *x, int *v) { *x = fixUp(*x + *v); }

inline void add(int *x, int v) { *x = fixUp(*x + v); }
}  // namespace SharedData

namespace Force {

using namespace SharedData;

const int MAXN = 4e6 + 5;

#define long long long

int m, n;

int blockSize;
int blockPos[MAXN], blockNum;

int revBlockPre[MAXN], revBlockSuf[MAXN];

int sum[MAXN], blockSum[MAXN], f[MAXN], q[MAXN], *qp = q;

inline int query(int x) {
    return x > blockSize ? fixUp(sum[x] + blockSum[blockPos[x]]) : sum[x];
}

inline void modify(int x, int y) {
    register int d = fixDown(y - fixDown(query(x) - query(x - 1)));
    register int *i = sum + x, *end = sum + revBlockPre[blockPos[x]],
                 *r = end - 14;
#define tmpFix(x) ((x) < MOD ? (x) : (x)-MOD)
#define tmpAdd(x, v) *(x) = tmpFix(*(x) + v)
    // r = i - 1;
    while (i <= r) {
        tmpAdd(i, d), tmpAdd(i + 1, d), tmpAdd(i + 2, d), tmpAdd(i + 3, d),
            tmpAdd(i + 4, d), tmpAdd(i + 5, d), tmpAdd(i + 6, d),
            tmpAdd(i + 7, d), tmpAdd(i + 8, d), tmpAdd(i + 9, d),
            tmpAdd(i + 10, d), tmpAdd(i + 11, d), tmpAdd(i + 12, d),
            tmpAdd(i + 13, d), tmpAdd(i + 14, d);
        i += 15;
    }

    while (i <= end) add(i++, d);
    i = blockSum + *(blockPos + x) + 1, end = blockSum + blockNum, r = end - 14;
    // r = i - 1;
    while (i <= r) {
        tmpAdd(i, d), tmpAdd(i + 1, d), tmpAdd(i + 2, d), tmpAdd(i + 3, d),
            tmpAdd(i + 4, d), tmpAdd(i + 5, d), tmpAdd(i + 6, d),
            tmpAdd(i + 7, d), tmpAdd(i + 8, d), tmpAdd(i + 9, d),
            tmpAdd(i + 10, d), tmpAdd(i + 11, d), tmpAdd(i + 12, d),
            tmpAdd(i + 13, d), tmpAdd(i + 14, d);
        i += 15;
    }
    while (i < end) add(i++, d);
#undef tmpFix
#undef tmpAdd
}

inline void initBlock() {
    blockSize = sqrt(n) * 3 / 2;
    for (register int i = 1; i <= n; i++) blockPos[i] = (i - 1) / blockSize;
    blockNum = blockPos[n] + 1;
    for (register int i = 1, *p = blockPos + 1; i <= n; i++)
        revBlockPre[*p++] = i;
    for (register int i = n, *p = blockPos + n; i; i--) revBlockSuf[*p--] = i;
}

inline void init() {
    for (register int i = 1, *p = blockPos + 1; i <= n; i++) {
        sum[i] = (long)i * i % MOD;
        add(blockSum[*p + 1], sum[i]);
        i != revBlockSuf[*p++] ? (add(sum[i], sum[i - 1]), 0) : 0;
    }
    for (register int *i = blockSum + 1, *end = blockSum + blockNum; i < end;
         i++)
        add(i, i - 1);
    f[1] = 1;
    for (register int i = 2; i <= n; i++) {
        !f[i] ? (f[i] = (long)i * i % MOD * (i - 1) % MOD, *qp++ = i) : 0;
        for (register int *j = q;; j++) {
            if (i * *j > n) break;
            if (i % *j == 0) {
                f[i * *j] = (long)*j * *j * *j % MOD * f[i] % MOD;
                break;
            }
            f[i * *j] = (long)f[i] * f[*j] % MOD;
        }
    }
    for (register int *i = f + 2, *end = f + n; i <= end; i++) add(i, i - 1);
}

inline void solve() {
    read(m), read(n);
    initBlock();
    init();
    while (m--) {
        register int a, b, k;
        register long x;
        read(a), read(b), read(x), read(k);
        register int d = std::__gcd(a, b);
        modify(d, x / (a / d) / (b / d) % MOD);
        register int i = 1, s = 0;
        while (i <= k) {
            register int j = k / (k / i);
            s = (s + (long)fixDown(f[j] - f[i - 1]) * query(k / i)) % MOD;
            i = j + 1;
        }
        print(s), print('\n');
    }
}
}  // namespace Force

int main() {
    // freopen("table.in", "r", stdin);
    // freopen("table.out", "w", stdout);
    Force::solve();
    flush();
    return 0;
}
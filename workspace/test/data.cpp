#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 1 << 18 | 1;
    static char buf[IN_LEN], *s, *t;
    return (s == t) && (t = (s = buf) + fread(buf, 1, IN_LEN, stdin)),
           s == t ? -1 : *s++;
}

const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[21], cnt;
    if (x != 0) {
        (x < 0) && (print('-'), x = -x);
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    } else {
        print('0');
    }
}

struct InputOutputStream {
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 250000 + 9;
const int BLOCK_SIZE = 500 * 0.7 + 9;
const int BLOCK_COUNT = 500 * (1 / 0.7) + 9;
const int MOD = 998244353;

typedef unsigned long long ulong;

#define add(x, v) (x = (x + v >= MOD ? x + v - MOD : x + v))

struct Block {
    int a[BLOCK_SIZE][3];
    int sum[3];
    int x[3], y[3], z[3], tag[3];
    int size;
} d[BLOCK_COUNT];

int n, m, blockSize, blockCount, id[MAXN];

inline void getId(int i, int &b, int &in) {
    b = id[i];
    in = i - 1 - b * blockSize;
}

inline void init() {
    blockSize = ceil(sqrt(n) * 0.7);
    for (register int i = 1; i <= n; i++) {
        id[i] = (i - 1) / blockSize;
    }
    blockCount = id[n];
    d[blockCount].size = n % blockSize;
    if (d[blockCount].size == 0) d[blockCount].size = blockSize;
    for (register int i = 0; i < blockCount; i++) {
        d[i].size = blockSize;
        d[i].x[0] = 1;
        d[i].y[1] = 1;
        d[i].z[2] = 1;
    }
    d[blockCount].x[0] = 1;
    d[blockCount].y[1] = 1;
    d[blockCount].z[2] = 1;
    for (register int i = 1, j = 0; i <= n; i++) {
        Block &now = d[id[i]];
        io >> now.a[j][0] >> now.a[j][1] >> now.a[j][2];
        add(now.sum[0], now.a[j][0]);
        add(now.sum[1], now.a[j][1]);
        add(now.sum[2], now.a[j][2]);
        if (++j == blockSize) j = 0;
    }
}

inline void pushDown(Block &p) {
    static int tmpA[BLOCK_SIZE], tmpB[BLOCK_SIZE];
    for (register int i = 0; i < p.size; i++) {
        tmpA[i] = p.a[i][0];
        tmpB[i] = p.a[i][1];
    }
    if (p.x[0] != 1 || p.y[0] || p.z[0] || p.tag[0]) {
        for (register int i = 0; i < p.size; i++) {
            p.a[i][0] = ((ulong)p.x[0] * p.a[i][0] + (ulong)p.y[0] * p.a[i][1] +
                         (ulong)p.z[0] * p.a[i][2] + p.tag[0]) %
                        MOD;
        }
        p.x[0] = 1;
        p.y[0] = p.z[0] = p.tag[0] = 0;
    }
    if (p.y[1] != 1 || p.x[1] || p.z[1] || p.tag[1]) {
        for (register int i = 0; i < p.size; i++) {
            p.a[i][1] = ((ulong)p.y[1] * p.a[i][1] + (ulong)p.x[1] * tmpA[i] +
                         (ulong)p.z[1] * p.a[i][2] + p.tag[1]) %
                        MOD;
        }
        p.y[1] = 1;
        p.x[1] = p.z[1] = p.tag[1] = 0;
    }
    if (p.z[2] != 1 || p.x[2] || p.y[2] || p.tag[2]) {
        for (register int i = 0; i < p.size; i++) {
            p.a[i][2] = ((ulong)p.z[2] * p.a[i][2] + (ulong)p.x[2] * tmpA[i] +
                         (ulong)p.y[2] * tmpB[i] + p.tag[2]) %
                        MOD;
        }
        p.z[2] = 1;
        p.x[2] = p.y[2] = p.tag[2] = 0;
    }
}

inline void modify1(int l, int r) {
    register int lb, rb, lbi, rbi;
    getId(l, lb, lbi);
    getId(r, rb, rbi);
    if (lb != rb) {
        {
            Block &now = d[lb];
            pushDown(now);
            for (register int i = lbi; i < now.size; i++) {
                add(now.a[i][0], now.a[i][1]);
                add(now.sum[0], now.a[i][1]);
            }
        }
        lb++;
        {
            Block &now = d[rb];
            pushDown(now);
            for (register int i = 0; i <= rbi; i++) {
                add(now.a[i][0], now.a[i][1]);
                add(now.sum[0], now.a[i][1]);
            }
        }
        rb--;
        if (lb <= rb) {
            for (register int i = lb; i <= rb; i++) {
                Block &now = d[i];
                add(now.x[0], now.x[1]);
                add(now.y[0], now.y[1]);
                add(now.z[0], now.z[1]);
                add(now.tag[0], now.tag[1]);
                add(now.sum[0], now.sum[1]);
            }
        }
    } else {
        Block &now = d[lb];
        pushDown(now);
        for (register int i = lbi; i <= rbi; i++) {
            add(now.a[i][0], now.a[i][1]);
            add(now.sum[0], now.a[i][1]);
        }
    }
}

inline void modify2(int l, int r) {
    register int lb, rb, lbi, rbi;
    getId(l, lb, lbi);
    getId(r, rb, rbi);
    if (lb != rb) {
        {
            Block &now = d[lb];
            pushDown(now);
            for (register int i = lbi; i < now.size; i++) {
                add(now.a[i][1], now.a[i][2]);
                add(now.sum[1], now.a[i][2]);
            }
        }
        lb++;
        {
            Block &now = d[rb];
            pushDown(now);
            for (register int i = 0; i <= rbi; i++) {
                add(now.a[i][1], now.a[i][2]);
                add(now.sum[1], now.a[i][2]);
            }
        }
        rb--;
        if (lb <= rb) {
            for (register int i = lb; i <= rb; i++) {
                Block &now = d[i];
                add(now.x[1], now.x[2]);
                add(now.y[1], now.y[2]);
                add(now.z[1], now.z[2]);
                add(now.tag[1], now.tag[2]);
                add(now.sum[1], now.sum[2]);
            }
        }
    } else {
        Block &now = d[lb];
        pushDown(now);
        for (register int i = lbi; i <= rbi; i++) {
            add(now.a[i][1], now.a[i][2]);
            add(now.sum[1], now.a[i][2]);
        }
    }
}

inline void modify3(int l, int r) {
    register int lb, rb, lbi, rbi;
    getId(l, lb, lbi);
    getId(r, rb, rbi);
    if (lb != rb) {
        {
            Block &now = d[lb];
            pushDown(now);
            for (register int i = lbi; i < now.size; i++) {
                add(now.a[i][2], now.a[i][0]);
                add(now.sum[2], now.a[i][0]);
            }
        }
        lb++;
        {
            Block &now = d[rb];
            pushDown(now);
            for (register int i = 0; i <= rbi; i++) {
                add(now.a[i][2], now.a[i][0]);
                add(now.sum[2], now.a[i][0]);
            }
        }
        rb--;
        if (lb <= rb) {
            for (register int i = lb; i <= rb; i++) {
                Block &now = d[i];
                add(now.x[2], now.x[0]);
                add(now.y[2], now.y[0]);
                add(now.z[2], now.z[0]);
                add(now.tag[2], now.tag[0]);
                add(now.sum[2], now.sum[0]);
            }
        }
    } else {
        Block &now = d[lb];
        pushDown(now);
        for (register int i = lbi; i <= rbi; i++) {
            add(now.a[i][2], now.a[i][0]);
            add(now.sum[2], now.a[i][0]);
        }
    }
}

inline void modify4(int l, int r, const register int v) {
    register int lb, rb, lbi, rbi;
    getId(l, lb, lbi);
    getId(r, rb, rbi);
    if (lb != rb) {
        {
            Block &now = d[lb];
            pushDown(now);
            for (register int i = lbi; i < now.size; i++) {
                add(now.a[i][0], v);
                add(now.sum[0], v);
            }
        }
        lb++;
        {
            Block &now = d[rb];
            pushDown(now);
            for (register int i = 0; i <= rbi; i++) {
                add(now.a[i][0], v);
                add(now.sum[0], v);
            }
        }
        rb--;
        if (lb <= rb) {
            for (register int i = lb; i <= rb; i++) {
                Block &now = d[i];
                add(now.tag[0], v);
                now.sum[0] = (now.sum[0] + (ulong)v * now.size) % MOD;
            }
        }
    } else {
        Block &now = d[lb];
        pushDown(now);
        for (register int i = lbi; i <= rbi; i++) {
            add(now.a[i][0], v);
            add(now.sum[0], v);
        }
    }
}

inline void modify5(int l, int r, const register int v) {
    register int lb, rb, lbi, rbi;
    getId(l, lb, lbi);
    getId(r, rb, rbi);
    if (lb != rb) {
        {
            Block &now = d[lb];
            pushDown(now);
            for (register int i = lbi; i < now.size; i++) {
                add(now.sum[1], MOD - now.a[i][1]);
                now.a[i][1] = (ulong)now.a[i][1] * v % MOD;
                add(now.sum[1], now.a[i][1]);
            }
        }
        lb++;
        {
            Block &now = d[rb];
            pushDown(now);
            for (register int i = 0; i <= rbi; i++) {
                add(now.sum[1], MOD - now.a[i][1]);
                now.a[i][1] = (ulong)now.a[i][1] * v % MOD;
                add(now.sum[1], now.a[i][1]);
            }
        }
        rb--;
        if (lb <= rb) {
            for (register int i = lb; i <= rb; i++) {
                Block &now = d[i];
                now.x[1] = (ulong)now.x[1] * v % MOD;
                now.y[1] = (ulong)now.y[1] * v % MOD;
                now.z[1] = (ulong)now.z[1] * v % MOD;
                now.tag[1] = (ulong)now.tag[1] * v % MOD;
                now.sum[1] = (ulong)now.sum[1] * v % MOD;
            }
        }
    } else {
        Block &now = d[lb];
        pushDown(now);
        for (register int i = lbi; i <= rbi; i++) {
            add(now.sum[1], MOD - now.a[i][1]);
            now.a[i][1] = (ulong)now.a[i][1] * v % MOD;
            add(now.sum[1], now.a[i][1]);
        }
    }
}

inline void modify6(int l, int r, const register int v) {
    register int lb, rb, lbi, rbi;
    getId(l, lb, lbi);
    getId(r, rb, rbi);
    if (lb != rb) {
        {
            Block &now = d[lb];
            pushDown(now);
            for (register int i = lbi; i < now.size; i++) {
                add(now.sum[2], MOD - now.a[i][2]);
                add(now.sum[2], v);
                now.a[i][2] = v;
            }
        }
        lb++;
        {
            Block &now = d[rb];
            pushDown(now);
            for (register int i = 0; i <= rbi; i++) {
                add(now.sum[2], MOD - now.a[i][2]);
                add(now.sum[2], v);
                now.a[i][2] = v;
            }
        }
        rb--;
        if (lb <= rb) {
            for (register int i = lb; i <= rb; i++) {
                Block &now = d[i];
                now.x[2] = now.y[2] = now.z[2] = 0;
                now.tag[2] = v;
                now.sum[2] = (ulong)v * now.size % MOD;
            }
        }
    } else {
        Block &now = d[lb];
        pushDown(now);
        for (register int i = lbi; i <= rbi; i++) {
            add(now.sum[2], MOD - now.a[i][2]);
            add(now.sum[2], v);
            now.a[i][2] = v;
        }
    }
}

inline void query(int l, int r) {
    register int ans1 = 0, ans2 = 0, ans3 = 0;
    register int lb, rb, lbi, rbi;
    getId(l, lb, lbi);
    getId(r, rb, rbi);
    if (lb != rb) {
        {
            Block &now = d[lb];
            pushDown(now);
            for (register int i = lbi; i < now.size; i++) {
                add(ans1, now.a[i][0]);
                add(ans2, now.a[i][1]);
                add(ans3, now.a[i][2]);
            }
        }
        lb++;
        {
            Block &now = d[rb];
            pushDown(now);
            for (register int i = 0; i <= rbi; i++) {
                add(ans1, now.a[i][0]);
                add(ans2, now.a[i][1]);
                add(ans3, now.a[i][2]);
            }
        }
        rb--;
        if (lb <= rb) {
            for (register int i = lb; i <= rb; i++) {
                Block &now = d[i];
                add(ans1, now.sum[0]);
                add(ans2, now.sum[1]);
                add(ans3, now.sum[2]);
            }
        }
    } else {
        Block &now = d[lb];
        pushDown(now);
        for (register int i = lbi; i <= rbi; i++) {
            add(ans1, now.a[i][0]);
            add(ans2, now.a[i][1]);
            add(ans3, now.a[i][2]);
        }
    }
    io << ans1 << ' ' << ans2 << ' ' << ans3 << '\n';
}

inline void solve() {
    io >> n;
    init();
    io >> m;
    for (register int cmd, l, r, v; m--;) {
        io >> cmd >> l >> r;
        switch (cmd) {
            case 1: {
                modify1(l, r);
                break;
            }
            case 2: {
                modify2(l, r);
                break;
            }
            case 3: {
                modify3(l, r);
                break;
            }
            case 4: {
                io >> v;
                modify4(l, r, v);
                break;
            }
            case 5: {
                io >> v;
                modify5(l, r, v);
                break;
            }
            case 6: {
                io >> v;
                modify6(l, r, v);
                break;
            }
            case 7: {
                query(l, r);
                break;
            }
        }
    }
}
}  // namespace

int main() {
    // freopen("sample/out.out", "w", stdout);
    solve();
    return 0;
}
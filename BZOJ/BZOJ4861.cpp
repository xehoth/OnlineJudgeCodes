/*
 * created by xehoth on 24-04-2017
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

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = '\0';
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = '\0';
    return s;
}

const int OUT_LEN = 100;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
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
const int MAXN = 51, MAXS = 101, MAXL = 101;
const int MOD = 1000000007;

struct Node {
    int next[26];
    int fail;
    bool end;
} tr[MAXS];

char st[MAXS];
char word[MAXN][MAXS];
int len[MAXN], tar[MAXS][MAXN];
int dp[MAXL][MAXS], id[MAXS];
int a[MAXS * 2][MAXS * 2], b[MAXS * 2][MAXS * 2], c[MAXS * 2][MAXS * 2];
int sz, n, m, L, ans;

inline void insert() {
    int p = 0;
    for (char *c = st; *c; c++) {
        int ch = *c - 'a';
        if (!tr[p].next[ch]) tr[p].next[ch] = sz++;
        p = tr[p].next[ch];
    }
    tr[p].end = true;
}

inline void build() {
    int h = 0, t = 0;
    static int que[MAXS];
    que[0] = 0;
    while (h <= t) {
        int u = que[h++], v;
        for (int i = 0; i < 26; i++) {
            int x = tr[u].fail;
            while (x && !tr[x].next[i]) x = tr[x].fail;
            if (u && tr[x].next[i]) x = tr[x].next[i];
            if (v = tr[u].next[i]) {
                tr[v].fail = x;
                tr[v].end |= tr[x].end;
                que[++t] = v;
            } else {
                tr[u].next[i] = x;
            }
        }
    }
}

inline void prepare() {
    for (int i = 0; i < sz; i++) {
        for (int j = 0; j < n; j++) {
            int p = i;
            for (int k = 0; k < len[j] && !tr[p].end; k++)
                p = tr[p].next[word[j][k] - 'a'];
            tar[i][j] = p;
        }
    }
}

inline void solve1() {
    dp[0][0] = 1;
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < sz; j++) {
            if (dp[i][j]) {
                for (int k = 0; k < n; k++) {
                    int p = tar[j][k];
                    if (tr[p].end || len[k] > L - i) continue;
                    int &r = dp[i + len[k]][p];
                    ((r += dp[i][j]) >= MOD) ? r -= MOD : 0;
                }
            }
        }
    }
    ans = 0;
    for (int j = 0; j < sz; j++)
        if (!tr[j].end) ((ans += dp[L][j]) >= MOD) ? ans -= MOD : 0;
}

inline void copy(int src[][MAXS * 2], int des[][MAXS * 2], int n) {
    for (int i = 0; i < n; i++) memcpy(des[i], src[i], sizeof(int) * n);
}

inline void solve2() {
    int lm = 0;
    for (int i = 0; i < n; i++)
        if (len[i] > lm) lm = len[i];
    int is = 0;
    for (int i = 0; i < sz; i++)
        if (!tr[i].end) id[i] = is++;
    int s0 = sz;
    sz = is;
    int n2 = sz * 2;
    for (int i = 0; i < sz; i++) a[i][i + sz] = 1;
    for (int i = 0; i < s0; i++) {
        if (!tr[i].end) {
            for (int j = 0; j < n; j++) {
                if (!tr[tar[i][j]].end) {
                    if (len[j] == 1)
                        a[id[tar[i][j]] + sz][id[i] + sz]++;
                    else
                        a[id[tar[i][j]] + sz][id[i]]++;
                }
            }
        }
    }
    copy(a, c, n2);

    for (int t = 0; (1 << t) <= L; t++) {
        if (L & (1 << t)) {
            copy(c, b, n2);
            for (int i = 0; i < n2; i++) {
                for (int j = 0; j < n2; j++) {
                    c[i][j] = 0;
                    for (int k = 0; k < n2; k++) {
                        ((c[i][j] += (long long)a[i][k] * b[k][j] % MOD) >= MOD)
                            ? c[i][j] -= MOD
                            : 0;
                    }
                }
            }
            L ^= 1 << t;
        }
        copy(a, b, n2);
        for (int i = 0; i < n2; i++) {
            for (int j = 0; j < n2; j++) {
                a[i][j] = 0;
                for (int k = 0; k < n2; k++) {
                    ((a[i][j] += (long long)b[i][k] * b[k][j] % MOD) >= MOD)
                        ? a[i][j] -= MOD
                        : 0;
                }
            }
        }
    }

    ans = 0;
    for (int i = 0; i < sz; i++) ((ans += c[i][sz]) >= MOD) ? ans -= MOD : 0;
}

int main() {
    // freopen("in.in", "r", stdin);

    read(n), read(m), read(L);
    for (int i = 0; i < n; i++) len[i] = read(word[i]);
    sz = 1;
    for (int i = 0; i < m; i++) {
        read(st);
        insert();
    }
    build();
    prepare();
    if (L < MAXL)
        solve1();
    else
        solve2();

    printf("%d\n", ans);
    return 0;
}

#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1 << 18 | 1;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline void read(T &x) {
    static char c;
    for (c = read(); !isdigit(c); c = read())
        if (c == -1) return;
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
}

const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[31], cnt;
    if (x != 0) {
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    } else {
        print('0');
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace SuffixArray {

const int MAXN = 100005;

int sa[MAXN], rk[MAXN << 1], ht[MAXN << 1];
bool t[MAXN << 1];

inline bool islms(const int i, const bool *t) {
    return i > 0 && t[i] && !t[i - 1];
}

template <typename T>
inline void sort(T s, int *sa, const int len, const int sz, const int sigma,
                 bool *t, int *b, int *cb, int *p) {
    memset(b, 0, sizeof(int) * sigma);
    memset(sa, -1, sizeof(int) * len);
    for (register int i = 0; i < len; i++) b[s[i]]++;
    cb[0] = b[0];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
    for (register int i = sz - 1; i >= 0; i--) sa[--cb[s[p[i]]]] = p[i];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i - 1];
    for (register int i = 0; i < len; i++)
        if (sa[i] > 0 && !t[sa[i] - 1]) sa[cb[s[sa[i] - 1]]++] = sa[i] - 1;
    cb[0] = b[0];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
    for (register int i = len - 1; i >= 0; i--)
        if (sa[i] > 0 && t[sa[i] - 1]) sa[--cb[s[sa[i] - 1]]] = sa[i] - 1;
}

template <typename T>
inline void sais(T s, int *sa, const int len, bool *t, int *b, int *b1,
                 const int sigma) {
    register int i, j, x, p = -1, cnt = 0, sz = 0, *cb = b + sigma;
    for (t[len - 1] = 1, i = len - 2; i >= 0; i--)
        t[i] = s[i] < s[i + 1] || (s[i] == s[i + 1] && t[i + 1]);
    for (i = 1; i < len; i++)
        if (t[i] && !t[i - 1]) b1[sz++] = i;
    sort(s, sa, len, sz, sigma, t, b, cb, b1);
    for (i = sz = 0; i < len; i++)
        if (islms(sa[i], t)) sa[sz++] = sa[i];
    for (i = sz; i < len; i++) sa[i] = -1;
    for (i = 0; i < sz; i++) {
        for (x = sa[i], j = 0; j < len; j++) {
            if (p == -1 || s[x + j] != s[p + j] || t[x + j] != t[p + j]) {
                p = x, cnt++;
                break;
            } else if (j > 0 && (islms(x + j, t) || islms(p + j, t))) {
                break;
            }
        }
        sa[sz + (x >>= 1)] = cnt - 1;
    }
    for (i = j = len - 1; i >= sz; i--)
        if (sa[i] >= 0) sa[j--] = sa[i];
    register int *s1 = sa + len - sz, *b2 = b1 + sz;
    if (cnt < sz)
        sais(s1, sa, sz, t + len, b, b1 + sz, cnt);
    else
        for (i = 0; i < sz; i++) sa[s1[i]] = i;
    for (i = 0; i < sz; i++) b2[i] = b1[sa[i]];
    sort(s, sa, len, sz, sigma, t, b, cb, b2);
}

template <typename T>
inline void getHeight(T s, const int n) {
    for (register int i = 1; i <= n; i++) rk[sa[i]] = i;
    for (register int i = 0, j = 0, k = 0; i < n; ht[rk[i++]] = k)
        for (k ? k-- : 0, j = sa[rk[i] - 1]; s[i + k] == s[j + k]; k++)
            ;
}

template <typename T>
inline void init(T s, const int n, const int sigma) {
    sais(s, sa, n, t, rk, ht, sigma), rk[0] = 0;
    getHeight(s, n - 1);
}

int pre[MAXN], next[MAXN];

std::pair<int, int> tmp[MAXN], *pos[MAXN];

inline bool cmp(const std::pair<int, int> *a, const std::pair<int, int> *b) {
    return a->first < b->first;
}

inline void solve() {
    using namespace IO;
    register int n, m = 0;
    read(n);
    static int a[MAXN];
    for (register int i = n - 1; i >= 0; i--) {
        read(a[i]);
        tmp[m].first = a[i], tmp[m].second = i;
        pos[m] = tmp + m;
        m++;
    }
    pos[m] = tmp + m;
    std::sort(pos, pos + m, cmp);
    register int l = 0, id = 1;
    for (; l < m;) {
        a[pos[l]->second] = id;
        while (l < m && pos[l]->first == pos[l + 1]->first) {
            l++;
            a[pos[l]->second] = id;
        }
        id++;
        l++;
    }
    init(a, n + 1, n + 1);
    static int ans[MAXN];
    for (register int i = 0; i <= n; i++) next[i] = i + 1;
    for (register int i = 0; i <= n; i++) pre[i] = i - 1;
    for (register int i = 0, k; i < n; i++) {
        k = rk[i];
        ht[k] > ht[next[k]]
            ? (ans[i] = n - i - ht[k])
            : (ans[i] = n - i - ht[next[k]], ht[next[k]] = ht[k]);
        next[pre[k]] = next[k];
        pre[next[k]] = pre[k];
    }
    register long long res = 0;
    for (register int i = n - 1; i >= 0; i--) {
        res += ans[i + 1];
        print(res + ans[i]);
        print('\n');
    }
}
}  // namespace SuffixArray

int main() {
    // freopen("sample/1.in", "r", stdin);
    SuffixArray::solve();
    IO::flush();
    return 0;
}

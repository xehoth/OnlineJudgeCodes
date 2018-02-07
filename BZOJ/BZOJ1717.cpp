// #include <bits/stdc++.h>
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdio>
#include <cstring>
#include <iostream>

class InputOutputStream {
   private:
    static const int BUFFER_SIZE = 1024 * 1024;
    char ibuf[BUFFER_SIZE], obuf[BUFFER_SIZE], *s, *t, *oh;
    bool isEof;

   public:
    InputOutputStream(char *in = NULL, char *out = NULL)
        : s(ibuf), oh(obuf), isEof(false) {
        in ? freopen(in, "r", stdin) : 0;
        out ? freopen(out, "w", stdout) : 0;
        t = ibuf + fread(ibuf, 1, BUFFER_SIZE, stdin);
    }

    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read();
    template <typename T>
    inline void read(T &);
    inline int read(char *);
    inline void print(char);
    inline void print(const char *);
    template <typename T>
    inline void print(T);
    template <typename T>
    inline InputOutputStream &operator>>(T &);
    template <typename T>
    inline InputOutputStream &operator<<(T);
    inline bool hasNext() const { return !isEof; }
}
#ifndef ONLINE_JUDGE
#ifdef DBG
io((char *)"in.in");
#else
io;
#endif
#else
io;
#endif

namespace SuffixArray {

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
inline void getHeight(T s, const int n, int *sa, int *rk, int *ht) {
    for (register int i = 1; i <= n; i++) rk[sa[i]] = i;
    for (register int i = 0, j = 0, k = 0; i < n; ht[rk[i++]] = k)
        for (k ? k-- : 0, j = sa[rk[i] - 1]; s[i + k] == s[j + k]; k++)
            ;
}

const int MAXN = 20500;

struct SuffixArray {
    int sa[MAXN], rk[MAXN], ht[MAXN];
    bool t[MAXN << 1];
    int n;
    int s[MAXN];

    inline void build(const int sigma) {
        s[n] = 0, sais(s, sa, n + 1, t, rk, ht, sigma);
        rk[0] = ht[0] = 0, getHeight(s, n, sa, rk, ht);
    }

    inline int &operator[](const int i) { return s[i]; }
} suffixArray;

int *sa, *rk, *ht, n, k;

inline bool check(int mid) {
    register int cnt = 1;
    for (register int i = 2; i <= n; i++) {
        if (ht[i] >= mid) {
            if (++cnt >= k) return true;
            continue;
        }
        cnt = 1;
    }
    return false;
}

inline void solve() {
    io >> n >> k;
    for (register int i = 0; i < n; i++) io >> suffixArray[i], suffixArray[i]++;
    suffixArray.n = n;
    suffixArray.build(*std::max_element(suffixArray.s, suffixArray.s + n) + 1);
    sa = suffixArray.sa, ht = suffixArray.ht, rk = suffixArray.rk;
    register int l = 0, r = n + 1;
    while (r - l > 1) {
        register int mid = l + r >> 1;
        check(mid) ? l = mid : r = mid;
    }
    io << l << '\n';
}
}  // namespace SuffixArray

int main() {
    SuffixArray::solve();
    return 0;
}

inline char InputOutputStream::read() {
    s == t ? t = (s = ibuf) + fread(ibuf, 1, BUFFER_SIZE, stdin) : 0;
    return (char)(s == t ? -1 : *s++);
}

template <typename T>
inline void InputOutputStream::read(T &x) {
    register char c;
    register bool iosig = false;
    for (c = read(); !isdigit(c); c = read()) {
        if (c == -1) return (void)(isEof = true);
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

inline int InputOutputStream::read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0, isEof = true;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    c == -1 ? isEof = true : 0;
    buf[s] = 0;
    return s;
}

inline void InputOutputStream::print(char c) {
    oh == obuf + BUFFER_SIZE ? (fwrite(obuf, 1, BUFFER_SIZE, stdout), oh = obuf)
                             : 0;
    *oh++ = c;
}

inline void InputOutputStream::print(const char *s) {
    for (; *s; s++) print(*s);
}

template <typename T>
inline void InputOutputStream::print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

template <typename T>
inline InputOutputStream &InputOutputStream::operator>>(T &x) {
    read(x);
    return *this;
}

template <typename T>
inline InputOutputStream &InputOutputStream::operator<<(T x) {
    print(x);
    return *this;
}
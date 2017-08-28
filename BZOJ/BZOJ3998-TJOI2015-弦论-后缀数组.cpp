/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3998」「TJOI 2015」弦论 28-08-2017
 * 后缀数组
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
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = 0;
    return s;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
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
}

namespace SuffixArray {

const int MAXN = 500000;

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
    register int i, j, x, p = -1, sz = 0, cnt = 0, *cb = b + sigma;
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

struct SuffixArray {
    int sa[MAXN + 2], rk[MAXN + 2], ht[MAXN + 2];
    bool t[MAXN * 2 + 4];
    int n;
    char s[MAXN + 2];

    inline void build(const int sigma) {
        s[n] = 0, sais(s, sa, n + 1, t, rk, ht, sigma);
        rk[0] = ht[0] = 0, getHeight(s, n, sa, rk, ht);
    }

    inline void tie(int *&sa, int *&rk, int *&ht, char *&s) {
        sa = this->sa, rk = this->rk, ht = this->ht, s = this->s;
    }
} suffixArray;

int *sa, *rk, *ht, n, type, varK;
char *s;

inline void solveTask0() {
    register int cnt = 0;
    for (register int i = 1, cur; i <= n; i++) {
        cur = n - sa[i] - ht[i];
        if (cnt + cur >= varK) {
            register int remain = varK - cnt + ht[i];
            for (register int j = 0; j < remain; j++) IO::print(s[sa[i] + j]);
            return;
        }
        cnt += cur;
    }
}

typedef std::pair<int, int> Pair;
typedef std::vector<Pair> Vector;
Vector buc[MAXN + 1];

inline void solveTask1() {
    using namespace IO;
    static std::vector<int> st, p;
    st.reserve(n + 1), st.push_back(-1);
    p.reserve(n + 1), p.push_back(0);
    for (register int i = 1, r; i <= n; i++) {
        r = i;
        while (st.back() >= ht[i]) {
            r = std::min(r, p.back());
            if (st.back() > ht[i]) {
                buc[p.back()].push_back(Pair(st.back(), i - p.back()));
#ifdef DBG
                std::cerr << p.back() << " " << st.back() << " "
                          << i - p.back();
                std::cerr << std::endl;
#endif
            }
            st.pop_back(), p.pop_back();
        }
        st.push_back(ht[i]), p.push_back(r);
    }
    while (!st.empty() && st.back() > 0) {
        buc[p.back()].push_back(Pair(st.back(), n + 1 - p.back()));
#ifdef DBG
        std::cerr << p.back() << " " << st.back() << " " << n + 1 - p.back();
        std::cerr << std::endl;
#endif
        st.pop_back(), p.pop_back();
    }
    register int tot = 0;
    for (register int i = 1, j; i <= n; i++) {
        j = ht[i] + 1;
        for (Vector::reverse_iterator it = buc[i + 1].rbegin(),
                                      end = buc[i + 1].rend();
             it != end; it++) {
            register int d = it->second + 1, cnt = it->first - j + 1;

            if (varK - tot - 1 <= (long)cnt * d) {
                j += (varK - tot - 1) / d;
                for (register int l = 0; l < j; l++) print(s[sa[i] + l]);
                // std::cerr << "done";
                return;
            }
            tot += d * cnt, j += cnt;
        }
        register int remain = n - sa[i] - j + 1;
        if (tot + remain >= varK) {
            register int d = varK - tot + j - 1;
            for (j = 0; j < d; j++) print(s[sa[i] + j]);
            return;
        }
        tot += remain;
    }
    print("-1");
}

inline void solve() {
    using namespace IO;
    n = suffixArray.n = read(suffixArray.s), read(type), read(varK);
    suffixArray.build(124), suffixArray.tie(sa, rk, ht, s);
    type ? solveTask1() : solveTask0();
}
}

int main() {
    // freopen("in.in", "r", stdin);
    SuffixArray::solve();
    IO::flush();
    return 0;
}
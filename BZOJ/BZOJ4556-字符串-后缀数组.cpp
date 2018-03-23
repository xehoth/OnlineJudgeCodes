/**
 * Copyright (c) 2016-2018, xehoth
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * 「BZOJ 4556」字符串 22-03-2018
 * 后缀数组
 * @author xehoth
 */

#include <algorithm>
#include <climits>
#include <cstdio>
#include <cstring>
#include <iostream>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}

    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    template <typename T>
    inline void read(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        if (iosig) x = -x;
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

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    inline void print(char c) {
        if (oh == obuf + SIZE) {
            fwrite(obuf, 1, SIZE, stdout);
            oh = obuf;
        }
        *oh++ = c;
    }

    template <typename T>
    inline void print(T x) {
        static int buf[21], cnt;
        if (x != 0) {
            if (x < 0) {
                print('-');
                x = -x;
            }
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
            while (cnt) print((char)buf[cnt--]);
        } else {
            print('0');
        }
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 100000 + 3;

struct SuffixArray {
    char s[MAXN];
    int sa[MAXN], ht[MAXN], rk[MAXN], n;
    bool t[MAXN * 2];

    static inline bool islms(const int i, const bool *t) {
        return i > 0 && t[i] && !t[i - 1];
    }

    template <typename T>
    static inline void sort(T s, int *sa, const int len, const int sz,
                            const int sigma, bool *t, int *b, int *cb, int *p) {
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
    void sais(T s, int *sa, const int len, bool *t, int *b, int *b1,
              const int sigma) {
        int i, j, x, p = -1, cnt = 0, sz = 0, *cb = b + sigma;
        for (t[len - 1] = 1, i = len - 2; i >= 0; i--)
            t[i] = s[i] < s[i + 1] || (s[i] == s[i + 1] && t[i + 1]);
        for (i = 1; i < len; i++)
            if (t[i] && !t[i - 1]) b1[sz++] = i;
        sort(s, sa, len, sz, sigma, t, b, cb, b1);
        for (i = sz = 0; i < len; i++)
            if (islms(sa[i], t)) sa[sz++] = sa[i];
        for (i = sz; i < len; i++) sa[i] = -1;
        for (i = 0; i < sz; i++) {
            for (j = 0, x = sa[i]; j < len; j++) {
                if (p == -1 || s[x + j] != s[p + j] || t[x + j] != t[p + j]) {
                    p = x;
                    cnt++;
                    break;
                } else if (j > 0 && (islms(x + j, t) || islms(p + j, t))) {
                    break;
                }
            }
            sa[sz + (x >>= 1)] = cnt - 1;
        }
        for (i = j = len - 1; i >= sz; i--)
            if (sa[i] >= 0) sa[j--] = sa[i];
        int *s1 = sa + len - sz, *b2 = b1 + sz;
        if (cnt < sz)
            sais(s1, sa, sz, t + len, b, b1 + sz, cnt);
        else
            for (i = 0; i < sz; i++) sa[s1[i]] = i;
        for (i = 0; i < sz; i++) b2[i] = b1[sa[i]];
        sort(s, sa, len, sz, sigma, t, b, cb, b2);
    }

    template <typename T>
    static inline void getHeight(T s, const int n, int *sa, int *rk, int *ht) {
        for (int i = 1; i <= n; i++) rk[sa[i]] = i;
        for (int i = 0, j = 0, k = 0; i < n; ht[rk[i++]] = k)
            for (k ? k-- : 0, j = sa[rk[i] - 1]; s[i + k] == s[j + k]; k++)
                ;
    }

    inline void build(int sigma) {
        s[n] = '\0';
        sais(s, sa, n + 1, t, rk, ht, sigma);
        rk[0] = ht[0] = 0;
        getHeight(s, n, sa, rk, ht);
    }
} suffixArray;

int n, m;

int main() {
    io >> n >> m;
    io.read(suffixArray.s);
    suffixArray.n = n;
    suffixArray.build(127);
    int *sa = suffixArray.sa, *rk = suffixArray.rk, *ht = suffixArray.ht;
    for (int a, b, c, d; m--;) {
        io >> a >> b >> c >> d;
        a--;
        b--;
        c--;
        d--;
        int pos = rk[c], ans = 0, min = INT_MAX, len = d - c + 1;
        if (a <= c && c <= b) ans = std::min(len, b - c + 1);
        for (int i = pos; ht[i] > ans && i >= 2; i--) {
            min = std::min(min, ht[i]);
            if (sa[i - 1] >= a && sa[i - 1] <= b) {
                ans = std::max(ans,
                               std::min(min, std::min(len, b - sa[i - 1] + 1)));
            }
        }
        min = INT_MAX;
        for (int i = pos + 1; ht[i] > ans && i <= n; i++) {
            min = std::min(min, ht[i]);
            if (sa[i] >= a && sa[i] <= b) {
                ans =
                    std::max(ans, std::min(min, std::min(len, b - sa[i] + 1)));
            }
        }
        io << ans << '\n';
    }
    return 0;
}
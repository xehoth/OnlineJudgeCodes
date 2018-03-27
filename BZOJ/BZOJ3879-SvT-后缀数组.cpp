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
 * 「BZOJ 3879」SvT 27-03-2018
 * 后缀数组 + 单调栈
 * @author xehoth
 */
#include <bits/stdc++.h>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 * 5 };
    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}

    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    inline int read(char *buf) {
        int s = 0;
        char c;
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

    inline InputOutputStream &operator>>(int &x) {
        static char c;
        for (c = read(); !isdigit(c); c = read())
            if (c == -1) return *this;
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        return *this;
    }

    inline void print(char c) {
        if (oh == obuf + SIZE) {
            fwrite(obuf, 1, SIZE, stdout);
            oh = obuf;
        }
        *oh++ = c;
    }

    inline InputOutputStream &operator<<(long long x) {
        static int buf[21], cnt;
        if (x != 0) {
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
            while (cnt) print((char)buf[cnt--]);
        } else {
            print('0');
        }
        print('\n');
        return *this;
    }
} io;

const int MAXN = 500000 + 9;

int logs[MAXN];

char s[MAXN];
int sa[MAXN], rk[MAXN], ht[19][MAXN], n;
bool t[MAXN * 2];

inline bool islms(const int i, const bool *t) {
    return i > 0 && t[i] && !t[i - 1];
}

template <typename T>
inline void sort(T s, int *sa, const int len, const int sz, const int sigma,
                 bool *t, int *b, int *cb, int *p) {
    memset(b, 0, sizeof(int) * sigma);
    memset(sa, -1, sizeof(int) * len);
    for (int i = 0; i < len; i++) b[s[i]]++;
    cb[0] = b[0];
    for (int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
    for (int i = sz - 1; i >= 0; i--) sa[--cb[s[p[i]]]] = p[i];
    for (int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i - 1];
    for (int i = 0; i < len; i++)
        if (sa[i] > 0 && !t[sa[i] - 1]) sa[cb[s[sa[i] - 1]]++] = sa[i] - 1;
    cb[0] = b[0];
    for (int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
    for (int i = len - 1; i >= 0; i--)
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
inline void getHeight(T s, int n, int *sa, int *rk, int *ht) {
    for (int i = 1; i <= n; i++) rk[sa[i]] = i;
    for (int i = 0, j = 0, k = 0; i < n; ht[rk[i++]] = k)
        for (k ? k-- : 0, j = sa[rk[i] - 1]; s[i + k] == s[j + k]; k++)
            ;
}

inline void build(int sigma) {
    s[n] = '\0';
    sais(s, sa, n + 1, t, rk, ht[0], sigma);
    rk[0] = ht[0][0] = 0;
    getHeight(s, n, sa, rk, ht[0]);
    for (int w = 1; (1 << w) < n; w++)
        for (int i = 1; i + (1 << w) <= n + 1; i++)
            ht[w][i] = std::min(ht[w - 1][i], ht[w - 1][i + (1 << (w - 1))]);
}

inline int lcp(int l, int r) {
    int bit = logs[r - l + 1];
    return std::min(ht[bit][l], ht[bit][r - (1 << bit) + 1]);
}

int m;

int q[MAXN], h[MAXN], cnt[MAXN];

inline bool cmp(int a, int b) { return rk[a] < rk[b]; }

int main() {
    io >> n >> m;
    logs[0] = -1;
    for (int i = 1; i <= n; i++) logs[i] = logs[i >> 1] + 1;
    io.read(s);
    build(127);
    for (int t; m--;) {
        io >> t;
        for (int i = 0; i < t; i++) {
            io >> q[i];
            q[i]--;
        }
        std::sort(q, q + t, cmp);
        t = std::unique(q, q + t) - q;
        for (int i = 1; i < t; i++) h[i] = lcp(rk[q[i - 1]] + 1, rk[q[i]]);
        for (int i = 0; i < t; i++) cnt[i] = 1;
        static int st[MAXN];
        int top = 0;
        h[t] = -1;
        long long ans = 0;
        for (int i = 1; i <= t; i++) {
            for (; top && h[i] <= h[st[top]]; top--) {
                ans += (long long)h[st[top]] * cnt[st[top - 1]] * cnt[st[top]];
                cnt[st[top - 1]] += cnt[st[top]];
            }
            st[++top] = i;
        }
        io << ans;
    }
    return 0;
}
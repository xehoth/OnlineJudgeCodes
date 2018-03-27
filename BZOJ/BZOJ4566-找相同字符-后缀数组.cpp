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
 * 「BZOJ 4566」找相同字符 24-03-2018
 * 后缀数组 + 单调栈
 * @author xehoth
 */
#include <cctype>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };

    char ibuf[SIZE], *s;

    InputOutputStream() : s(ibuf) {
#ifdef DBG
        freopen("sample/1.in", "r", stdin);
#endif
        fread(ibuf, 1, SIZE, stdin);
    }

    inline int read(char *buf) {
        int l = 0;
        char c;
        while (c = *s++, isspace(c))
            ;
        do
            buf[l++] = c;
        while (c = *s++, !isspace(c));
        buf[l] = '\0';
        return l;
    }
} io;

const int MAXN = 200000 * 2 + 5;

struct SuffixArray {
    char s[MAXN];
    int sa[MAXN], rk[MAXN], ht[MAXN], n;
    bool t[MAXN << 1];

    static inline bool islms(const int i, const bool *t) {
        return i > 0 && t[i] && !t[i - 1];
    }

    template <typename T>
    static inline void sort(T s, int *sa, const int len, const int sz,
                            const int sigma, bool *t, int *b, int *cb, int *p) {
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
    static void sais(T s, int *sa, const int len, bool *t, int *b, int *b1,
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
            for (x = sa[i], j = 0; j < len; j++) {
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
    static inline void getHeight(T s, int n, int *sa, int *rk, int *ht) {
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
} d;

struct Node {
    int h, id;
} st[MAXN];

int l[MAXN], r[MAXN], s1[MAXN], s2[MAXN];

int main() {
    d.n = io.read(d.s);
    int pos = d.n;
    d.s[d.n++] = '|';
    d.n += io.read(d.s + d.n);
    d.build(127);
    int n = d.n, *sa = d.sa, *ht = d.ht;
    int top = 0;
    st[top].h = -1;
    for (int i = 1; i <= n; i++) {
        while (st[top].h >= ht[i]) {
            r[st[top].id] = i - 1;
            top--;
        }
        l[i] = st[top++].id + 1;
        st[top].h = ht[i];
        st[top].id = i;
    }
    while (top) r[st[top--].id] = n;
    for (int i = 1; i <= n; i++) s1[i] = s1[i - 1] + (sa[i] < pos);
    for (int i = 1; i <= n; i++) s2[i] = s2[i - 1] + (sa[i] > pos);
    long long ans = 0;
    for (int i = 2; i <= n; i++) {
        ans += (long long)ht[i] *
               ((s2[i - 1] - s2[l[i] - 2]) * (s1[r[i]] - s1[i - 1]) +
                (s1[i - 1] - s1[l[i] - 2]) * (s2[r[i]] - s2[i - 1]));
    }
    std::cout << ans;
    return 0;
}
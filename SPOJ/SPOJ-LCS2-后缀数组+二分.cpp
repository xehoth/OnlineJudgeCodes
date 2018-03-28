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
 * 「SPOJ LCS2」03-28-2018
 * 后缀数组 + 二分
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 1000000 + 10 + 9;
const int MAXM = 100000 + 9;
int sa[MAXN], rk[MAXN], ht[MAXN], n;
bool t[MAXN * 2];
int s[MAXN];
char str[MAXM];

inline bool islms(const int i, const bool *t) {
    return i > 0 && t[i] && !t[i - 1];
}

template <typename T>
inline void sort(T s, int *sa, const int len, const int sz, const int sigma,
                 bool *t, int *b, int *cb, int *p) {
    memset(sa, -1, sizeof(int) * len);
    memset(b, 0, sizeof(int) * sigma);
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
    sais(s, sa, n + 1, t, ht, rk, sigma);
    ht[0] = rk[0] = 0;
    getHeight(s, n, sa, rk, ht);
}

int id[MAXN], idx, min, flag[10 + 9];

inline bool check(int mid) {
    int pos = 1, cnt = 1;
    memset(flag, 0, sizeof(flag));
    flag[id[sa[0]]] = pos;
    for (int i = 1; i < n; i++) {
        if (ht[i] < mid) {
            cnt = 1;
            flag[id[sa[i]]] = ++pos;
        } else {
            int now = id[sa[i]];
            if (flag[now] != pos) {
                cnt++;
                flag[now] = pos;
            }
            if (cnt == idx) return true;
        }
    }
    return false;
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    int split = 'z' + 1;
    min = INT_MAX;
    for (idx = 1; ~scanf("%s", str); idx++) {
        int start = n;
        for (char *c = str; *c; c++) {
            id[n] = idx;
            s[n++] = *c;
        }
        id[n] = idx;
        s[n++] = split++;
        min = std::min(min, n - start);
    }
    idx--;
    build(split + 1);
    int l = -1, r = min + 1;
    for (int mid; r - l > 1;) {
        mid = (l + r) >> 1;
        if (check(mid)) {
            l = mid;
        } else {
            r = mid;
        }
    }
    std::cout << l;
    return 0;
}

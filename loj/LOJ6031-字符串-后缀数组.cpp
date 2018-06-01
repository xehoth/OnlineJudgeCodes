/**
 * Copyright (c) 2018, xehoth
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
 * 「LOJ 6031」字符串 01-06-2018
 * 后缀数组
 * @author xehoth
 */
#include <bits/stdc++.h>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    void read(int &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ 48);
        if (iosig) x = -x;
    }

    int read(char *buf) {
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

    template <typename T>
    InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    void print(char c) {
        if (oh == obuf + SIZE) {
            fwrite(obuf, 1, SIZE, stdout);
            oh = obuf;
        }
        *oh++ = c;
    }

    template <typename T>
    void print(T x) {
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
    InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 100000;
const int MAXS = MAXN * 2 + 5;

struct Range {
    int l, r, id;

    int len() const { return r - l + 1; }

    bool operator<(const Range &p) const {
        return len() < p.len() || (len() == p.len() && l < p.l);
    }
} seg[MAXN + 1], que[MAXN + 1];

char s[MAXS];
int sa[MAXS], rk[MAXS], ht[MAXS], len, suf[MAXS], cnt[MAXS], fa[MAXS], sz[MAXS], end[MAXS];
bool t[MAXS * 2 + 1];
int begin[MAXN + 1], buc[MAXS];
long long ans[MAXN + 1];

inline bool islms(const int i, const bool *t) { return i > 0 && t[i] && !t[i - 1]; }

template <typename T>
inline void sort(T s, int *sa, const int len, const int sz, const int sigma, bool *t, int *b,
                 int *cb, int *p) {
    memset(b, 0, sizeof(int) * sigma);
    memset(sa, -1, sizeof(int) * len);
    for (int i = 0; i < len; i++) b[(int)s[i]]++;
    cb[0] = b[0];
    for (int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
    for (int i = sz - 1; i >= 0; i--) sa[--cb[(int)s[p[i]]]] = p[i];
    for (int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i - 1];
    for (int i = 0; i < len; i++)
        if (sa[i] > 0 && !t[sa[i] - 1]) sa[cb[(int)s[sa[i] - 1]]++] = sa[i] - 1;
    cb[0] = b[0];
    for (int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
    for (int i = len - 1; i >= 0; i--)
        if (sa[i] > 0 && t[sa[i] - 1]) sa[--cb[(int)s[sa[i] - 1]]] = sa[i] - 1;
}

template <typename T>
void sais(T s, int *sa, const int len, bool *t, int *b, int *b1, const int sigma) {
    int i, j, x, p = -1, sz = 0, cnt = 0, *cb = b + sigma;
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
inline void getHeight(T s, const int n, int *sa, int *rk, int *ht) {
    for (int i = 1; i <= n; i++) rk[sa[i]] = i;
    for (int i = 1, j = 0, k = 0; i <= n; ht[rk[i++]] = k)
        for (k ? k-- : 0, j = sa[rk[i] + 1]; s[i + k] == s[j + k]; k++)
            ;
    for (int i = 1; i <= n; i++) suf[i] = ht[rk[i]];
    memcpy(ht, suf, sizeof(int) * (n + 1));
    for (int i = 1; i <= n; i++) cnt[ht[i] + 1]++;
    for (int i = 1; i <= n; i++) cnt[i] += cnt[i - 1];
    for (int i = 1; i <= n; i++) suf[cnt[ht[i] + 1]--] = i;
}

inline void build(int sigma) {
    s[len + 1] = '\0';
    sais(s + 1, sa, len + 1, t, rk, ht, sigma);
    for (int i = 1; i <= len; i++) sa[i]++;
    s[0] = '\0';
    rk[0] = ht[0] = 0;
    getHeight(s, len, sa, rk, ht);
}

inline void solve() {
    int n, m, q, k;
    io >> n >> m >> q >> k;
    io.read(s + 1);
    len = n + 1;
    s[len] = 1;
    for (int i = 1; i <= m; i++) {
        io >> seg[i].l >> seg[i].r;
        seg[i].id = i;
    }
    for (int i = 1; i <= q; i++) {
        begin[i] = len + 1;
        io.read(s + len + 1);
        len += k;
        io >> que[i].l >> que[i].r;
        que[i].l++;
        que[i].r++;
    }
    build(127);
    cnt[0] = 0;
    for (int i = 1; i <= len; i++) fa[i] = i;
    for (int i = 1; i <= len; i++) cnt[i] = 1;
    for (int i = 1; i <= n; i++) sz[i] = 1;
    memcpy(end, rk, sizeof(int) * (len + 1));
    std::sort(seg + 1, seg + m + 1);
    seg[0].l = -1;
    for (int head = m, j = len, nlen; head;) {
        for (nlen = seg[head].len(); ht[suf[j]] >= nlen;) {
            int x = suf[j--], l = fa[x], r = fa[sa[rk[x] + 1]];
            if (cnt[r] >= cnt[l]) {
                for (int i = end[l] - cnt[l] + 1; i <= end[l]; i++) fa[sa[i]] = r;
                cnt[r] += cnt[l];
                sz[r] += sz[l];
            } else {
                for (int i = end[r] - cnt[r] + 1; i <= end[r]; i++) fa[sa[i]] = l;
                cnt[l] += cnt[r];
                sz[l] += sz[r];
                end[l] = end[r];
            }
        }
        int nl = seg[head].l, to = head;
        while (seg[to].l == nl && seg[to].len() == nlen) to--;
        if ((long long)(head - to) * q > m) {
            memset(buc, 0, sizeof(int) * (m + 1));
            for (; head > to; head--) buc[seg[head].id]++;
            for (int i = 1; i <= m; i++) buc[i] += buc[i - 1];
            for (int i = 1; i <= q; i++) {
                ans[i] += (long long)sz[fa[begin[i] + nl]] * (buc[que[i].r] - buc[que[i].l - 1]);
            }
        } else {
            for (int id; head > to; head--) {
                id = seg[head].id;
                for (int i = 1; i <= q; i++) {
                    if (que[i].l <= id && que[i].r >= id) {
                        ans[i] += sz[fa[begin[i] + nl]];
                    }
                }
            }
        }
    }
    for (int i = 1; i <= q; i++) io << ans[i] << '\n';
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}

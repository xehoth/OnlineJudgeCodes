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
 * 「BZOJ 5304」字串覆盖 30-05-2018
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

    template <typename T>
    void read(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
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

const int POOL_SIZE = 1024 * 1024 * 64;

char pool[POOL_SIZE];

void *operator new(size_t size) {
    static char *s = pool + POOL_SIZE;
    return s -= size;
}

void operator delete(void *) {}

void operator delete(void *, size_t) {}

const int MAXN = 200000 + 5;
const int MAXQ = 100000;
const int MAX_LOG = 18;

char s[MAXN];
int sa[MAXN], rk[MAXN], ht[MAX_LOG][MAXN], n, K, q, tot;
bool t[MAXN * 2 + 1];

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
inline void sais(T s, int *sa, const int len, bool *t, int *b, int *b1, const int sigma) {
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
}

inline void build(int sigma) {
    s[n] = '\0';
    sais(s, sa, n + 1, t, rk, ht[0], sigma);
    rk[0] = ht[0][0] = 0;
    memmove(s + 1, s, n);
    for (int i = 1; i <= n; i++) sa[i]++;
    getHeight(s, n, sa, rk, ht[0]);
    for (int w = 1; (1 << w) <= n; w++)
        for (int i = 1; i + (1 << w) <= n + 1; i++)
            ht[w][i] = std::min(ht[w - 1][i], ht[w - 1][i + (1 << (w - 1))]);
}

const int BLOCK = 50;

long long ans[MAXQ + 1];

struct Query {
    int s, t, l, len, pos;
    long long *ans;

    inline bool operator<(const Query &p) const {
        return len < p.len || (len == p.len && pos < p.pos);
    }
} que[MAXQ + 1];

struct Node *null;

struct Node {
    Node *lc, *rc;
    int size;

    Node() : lc(null), rc(null), size() {}
};

Node *root[MAXN];

void insert(Node *&p, Node *pre, int l, int r, int v) {
    p = new Node(*pre);
    p->size++;
    if (l == r) return;
    int mid = (l + r) >> 1;
    v <= mid ? insert(p->lc, pre->lc, l, mid, v) : insert(p->rc, pre->rc, mid + 1, r, v);
}

int query(Node *L, Node *R, int l, int r, int v) {
    if (R->size - L->size == 0) return 0;
    if (l == r) return l;
    int mid = (l + r) >> 1, ret;
    if (v <= mid && (ret = query(L->lc, R->lc, l, mid, v))) return ret;
    return query(L->rc, R->rc, mid + 1, r, v);
}

int fa[MAX_LOG][MAXN];
long long sum[MAX_LOG][MAXN];

inline void solve(int len, int &ptr) {
    static int pos[MAXN];
    for (int l = 1, r, cnt, k; l <= tot; l = r) {
        if (que[ptr].len != len || que[ptr].pos < l) return;
        for (r = l; ht[0][r] >= len;) r++;
        if (que[ptr].pos > r) {
            r++;
            continue;
        }
        cnt = 0;
        for (int j = l; j <= r; j++)
            if (sa[j] <= n) pos[++cnt] = sa[j];
        std::sort(pos + 1, pos + cnt + 1);
        const int limit = std::__lg(std::min(cnt + 1, n / len));
        k = 1;
        pos[cnt + 1] = tot;
        for (int i = 0; i <= limit; i++) {
            memset(fa[i], 0, sizeof(int) * (cnt + 2));
            memset(sum[i], 0, sizeof(long long) * (cnt + 2));
        }
        for (int j = 1; j <= cnt; j++) {
            while (pos[k] - pos[j] < len) k++;
            fa[0][j] = k;
            sum[0][j] = K - pos[j];
        }
        for (int j = 1; j <= limit; j++) {
            for (int i = 1; i + (1 << j) - 1 <= cnt; i++) {
                fa[j][i] = fa[j - 1][fa[j - 1][i]];
                sum[j][i] = sum[j - 1][i] + sum[j - 1][fa[j - 1][i]];
            }
        }
        for (int L, R, x; que[ptr].len == len && que[ptr].pos <= r; ptr++) {
            L = que[ptr].s;
            R = que[ptr].t;
            x = std::lower_bound(pos + 1, pos + cnt + 2, L) - pos;
            if (pos[x] > R) {
                *que[ptr].ans = 0;
            } else {
                long long ret = 0;
                for (int i = limit; i >= 0; i--) {
                    if (pos[fa[i][x]] && pos[fa[i][x]] <= R) {
                        ret += sum[i][x];
                        x = fa[i][x];
                    }
                }
                *que[ptr].ans = ret + sum[0][x];
            }
        }
    }
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    io >> n >> K;
    io.read(s);
    s[n] = '#';
    io.read(s + n + 1);
    n = n + n + 1;
    build(127);
    tot = n;
    n >>= 1;
    io >> q;
    for (int i = 1; i <= q; i++) {
        io >> que[i].s >> que[i].t >> que[i].l >> que[i].len;
        que[i].len -= que[i].l - 1;
        que[i].ans = ans + i;
        que[i].pos = rk[que[i].l + n + 1];
        que[i].t -= que[i].len - 1;
    }
    std::sort(que + 1, que + q + 1);
    null = new Node;
    null->lc = null->rc = null;
    null->size = 0;
    root[0] = null;
    int ptr = 1;
    for (int len = 1; len <= std::min(n, BLOCK); len++) solve(len, ptr);
    if (ptr <= q) {
        for (int i = 1; i <= tot; i++) {
            if (sa[i] <= n)
                insert(root[i], root[i - 1], 1, n, sa[i]);
            else
                root[i] = root[i - 1];
        }
        for (int i = ptr, x, l, r, pos; i <= q; i++) {
            Query &p = que[i];
            l = r = x = p.pos;
            pos = p.s;
            for (int i = MAX_LOG - 1; i >= 0; i--)
                if (l - (1 << i) > 0 && ht[i][l - (1 << i)] >= p.len) l -= 1 << i;
            for (int i = MAX_LOG - 1; i >= 0; i--)
                if (r + (1 << i) <= tot && ht[i][r] >= p.len) r += 1 << i;
            for (int next = 0; pos <= n; pos = next + p.len) {
                next = query(root[l - 1], root[r], 1, n, pos);
                if (next == 0 || next > p.t) break;
                *p.ans += K - next;
            }
        }
    }
    for (int i = 1; i <= q; i++) io << ans[i] << '\n';
    return 0;
}

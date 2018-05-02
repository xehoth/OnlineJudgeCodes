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
 * 「CC PRIMEDST」Prime Distance On Tree 02-05-2018
 * 点分治 + FFT
 * @author xehoth
 */
#include <bits/stdc++.h>

struct InputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    inline char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    template <typename T>
    inline InputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        if (iosig) x = -x;
        return *this;
    }
} io;

constexpr double PI = acos(-1);
const int MAXN = 50000;
const int MAXM = 1 << 17 | 1;

struct Complex {
    double r, i;

    inline Complex operator+(const Complex &p) const {
        return {r + p.r, i + p.i};
    }

    inline Complex operator-(const Complex &p) const {
        return {r - p.r, i - p.i};
    }

    inline Complex operator*(const Complex &p) const {
        return {r * p.r - i * p.i, r * p.i + i * p.r};
    }

    inline Complex conj() const { return {r, -i}; }

    inline Complex inv() const {
        double x = r * r + i * i;
        return {r / x, -i / x};
    }

    inline void operator+=(const Complex &p) {
        r += p.r;
        i += p.i;
    }
} rt[MAXM], irt[MAXM], a[MAXM];

inline void init(int n) {
    rt[0] = {1, 0};
    rt[1] = {cos(2 * PI / n / 2), sin(2 * PI / n / 2)};
    for (int i = 2; i < n; i++) rt[i] = rt[i - 1] * rt[1];
    irt[0] = {1, 0};
    irt[1] = rt[1].inv();
    for (int i = 2; i < n; i++) irt[i] = irt[i - 1] * irt[1];
    for (int i = 0, j = 0; i < n; i++) {
        if (i > j) {
            std::swap(rt[i], rt[j]);
            std::swap(irt[i], irt[j]);
        }
        for (int k = n >> 1; (j ^= k) < k; k >>= 1)
            ;
    }
}

inline void dit(Complex *a, int n) {
    for (int i = 1, l = n >> 1; i < n; i <<= 1, l >>= 1) {
        for (int j = 0, o = 0; j < i; j++, o += l << 1) {
            const Complex &w = rt[i + j];
            for (int k = o; k < o + l; k++) {
                Complex t = a[k + l] * w;
                a[k + l] = a[k] - t;
                a[k] += t;
            }
        }
    }
}

inline void dif(Complex *a, int n) {
    for (int i = n >> 1, l = 1; i; i >>= 1, l <<= 1) {
        for (int j = 0, o = 0; j < i; j++, o += l << 1) {
            const Complex &w = irt[i + j];
            for (int k = o; k < o + l; k++) {
                Complex t = a[k + l];
                a[k + l] = (a[k] - t) * w;
                a[k] += t;
            }
        }
    }
    for (int i = 0; i < n; i++) a[i].r /= n;
}

int pr[MAXN + 1], cnt, n;
bool isV[MAXM * 2 + 9];

inline void sieve(int n) {
    isV[1] = true;
    for (int i = 2; i <= n; i++) {
        if (!isV[i]) pr[cnt++] = i;
        for (int j = 0, k; j < cnt && (k = i * pr[j]) <= n; j++) {
            isV[k] = true;
            if (i % pr[j] == 0) break;
        }
    }
}

std::vector<int> g[MAXN + 9];
long long ans;
int sz[MAXN + 9];
bool vis[MAXN + 9];

void dfsSize(int u, int pre) {
    sz[u] = 1;
    for (int v : g[u]) {
        if (!vis[v] && v != pre) {
            dfsSize(v, u);
            sz[u] += sz[v];
        }
    }
}

int get(int u, int pre, int n) {
    for (int v : g[u])
        if (!vis[v] && v != pre && sz[v] > n) return get(v, u, n);
    return u;
}

std::vector<long long> sub, all;

void dfsDep(int u, int pre, int dep) {
    if (!isV[dep]) ans += 2;
    for (; (int)sub.size() <= dep;) sub.push_back(0);
    sub[dep]++;
    for (; (int)all.size() <= dep;) all.push_back(0);
    all[dep]++;
    for (int v : g[u])
        if (!vis[v] && v != pre) dfsDep(v, u, dep + 1);
}

inline void convolve(std::vector<long long> &v) {
    int m = v.size() + v.size() + 1, k = 1;
    for (; k <= m;) k <<= 1;
    for (int i = 0; i < (int)v.size(); i++) {
        a[i].r = v[i];
        a[i].i = 0;
    }
    for (int i = (int)v.size(); i < k; i++) a[i].r = a[i].i = 0;
    dit(a, k);
    for (int i = 0; i < k; i++) a[i] = a[i] * a[i];
    dif(a, k);
    for (; (int)(a[m].r + 0.3) == 0 && m;) m--;
    v.resize(m + 1);
    for (int i = 0; i <= m; i++) v[i] = (long long)(a[i].r + 0.3);
}

void solve(int u) {
    dfsSize(u, 0);
    vis[u = get(u, 0, sz[u] / 2)] = true;
    all.clear();
    for (int v : g[u]) {
        if (!vis[v]) {
            sub.clear();
            dfsDep(v, u, 1);
            convolve(sub);
            for (int j = 0; j < cnt && pr[j] < (int)sub.size(); j++)
                ans -= sub[pr[j]];
        }
    }
    convolve(all);
    for (int j = 0; j < cnt && pr[j] < (int)all.size(); j++) {
        ans += all[pr[j]];
    }
    for (int v : g[u])
        if (!vis[v]) solve(v);
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    sieve(MAXN * 2 + 1);
    io >> n;
    for (int i = 1, u, v; i < n; i++) {
        io >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) std::reverse(g[i].begin(), g[i].end());
    int k = 1;
    for (; k <= n + n + 1;) k <<= 1;
    init(k);
    solve(1);
    printf("%.8f", (double)ans / ((double)n * (n - 1) / 2.0) / 2.0);
    return 0;
}

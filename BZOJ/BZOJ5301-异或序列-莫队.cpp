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
 * 「BZOJ 5301」异或序列 04-05-2018
 * 莫队
 * @author xehoth
 */
#include <bits/stdc++.h>

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
    inline InputOutputStream &operator>>(T &x) {
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

const int MAXN = 100000 + 9;
const int MAXV = 131072 + 1;

int n, m, K, block, a[MAXN], sum[MAXN], id[MAXN], ans[MAXN], cnt[MAXV], now;

struct Query {
    int l, r, *ans;

    inline bool operator<(const Query &p) const {
        return id[l] == id[p.l] ? ((id[l] & 1) ? r > p.r : r < p.r) : l < p.l;
    }
} que[MAXN];

inline void add(int x) {
    now += cnt[K ^ sum[x]] + (sum[x] == K);
    cnt[sum[x]]++;
}

inline void dec(int x) {
    cnt[sum[x]]--;
    now -= cnt[K ^ sum[x]] + (sum[x] == K);
}

int main() {
    io >> n >> m >> K;
    block = sqrt(n) * 1.2;
    for (int i = 1; i <= n; i++) io >> a[i];
    for (int i = 1; i <= n; i++) sum[i] = sum[i - 1] ^ a[i];
    for (int i = 1; i <= n; i++) id[i] = (i - 1) / block + 1;
    for (int i = 1; i <= m; i++) {
        io >> que[i].l >> que[i].r;
        que[i].ans = ans + i;
    }
    std::sort(que + 1, que + m + 1);
    for (int l = 1, r = 0, i = 1; i <= m; i++) {
        while (l > que[i].l) {
            l--;
            add(l - 1);
        }
        while (l < que[i].l) {
            dec(l - 1);
            l++;
        }
        while (r > que[i].r) {
            dec(r--);
        }
        while (r < que[i].r) {
            add(++r);
        }
        *que[i].ans = now;
    }
    for (int i = 1; i <= m; i++) io << ans[i] << '\n';
    return 0;
}
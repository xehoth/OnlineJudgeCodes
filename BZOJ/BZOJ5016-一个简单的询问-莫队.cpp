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
 * 「BZOJ 5016」一个简单的询问 07-03-2018
 * 莫队
 * @author xehoth
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };

    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}

    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
               s == t ? -1 : *s++;
    }

    inline InputOutputStream &operator>>(int &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }

    inline void print(char c) {
        (oh == obuf + SIZE) && (fwrite(obuf, 1, SIZE, stdout), oh = obuf);
        *oh++ = c;
    }

    template <typename T>
    inline void print(T x) {
        static int buf[21], cnt;
        if (x != 0) {
            (x < 0) && (print('-'), x = -x);
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

const int MAXN = 50000 + 1;
const int MAXQ = MAXN * 4 + 1;

int n, q, a[MAXN], id[MAXN], sz, ans[MAXN], tot;

struct Node {
    int l, r, flag, *ans;

    Node() {}

    Node(int l, int r, int flag, int *ans) : l(l), r(r), flag(flag), ans(ans) {}

    inline bool operator<(const Node &p) const {
        return id[l] == id[p.l] ? ((id[l] & 1) ? r > p.r : r < p.r) : l < p.l;
    }
} que[MAXQ];

int cntL[MAXN], cntR[MAXN];

int main() {
    io >> n;
    for (int i = 1; i <= n; i++) io >> a[i];
    io >> q;
    sz = sqrt(n / 2);
    for (int i = 1; i <= n; i++) id[i] = i / sz + 1;
    for (int i = 1, l1, r1, l2, r2; i <= q; i++) {
        io >> l1 >> r1 >> l2 >> r2;
        que[++tot] = Node(r1, r2, 1, ans + i);
        if (l1 > 1) que[++tot] = Node(l1 - 1, r2, -1, ans + i);
        if (l2 > 1) que[++tot] = Node(l2 - 1, r1, -1, ans + i);
        if (l1 > 1 && l2 > 1) que[++tot] = Node(l1 - 1, l2 - 1, 1, ans + i);
    }
    for (int i = 1; i <= tot; i++)
        if (que[i].l > que[i].r) std::swap(que[i].l, que[i].r);
    std::sort(que + 1, que + tot + 1);
    for (int l = 0, r = 0, i = 1, ret = 0; i <= tot; i++) {
        while (l < que[i].l) {
            l++;
            ret += cntR[a[l]];
            cntL[a[l]]++;
        }
        while (l > que[i].l) {
            ret -= cntR[a[l]];
            cntL[a[l]]--;
            l--;
        }
        while (r < que[i].r) {
            r++;
            ret += cntL[a[r]];
            cntR[a[r]]++;
        }
        while (r > que[i].r) {
            ret -= cntL[a[r]];
            cntR[a[r]]--;
            r--;
        }
        (que[i].flag == 1) ? (*que[i].ans += ret) : (*que[i].ans -= ret);
    }
    for (int i = 1; i <= q; i++) io << ans[i] << '\n';
    return 0;
}
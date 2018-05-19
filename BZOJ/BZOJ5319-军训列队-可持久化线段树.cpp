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
 * 「BZOJ 5319」军训列队 19-05-2018
 * 可持久化线段树
 * @author xehoth
 */
#include <bits/stdc++.h>

#ifdef _DEBUG
struct Debug {
    std::chrono::steady_clock::time_point start, end;

    Debug() { start = std::chrono::steady_clock::now(); }

    ~Debug() {
        end = std::chrono::steady_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double count = (double)duration.count() *
                       std::chrono::microseconds::period::num /
                       std::chrono::microseconds::period::den;
        std::cerr << std::endl
                  << "time: " << std::fixed << std::setprecision(3)
                  << count * 1000 << "ms" << std::endl;
    }
} debug;
#endif

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

const int POOL_SIZE = 1024 * 1024 * 256;
char pool[POOL_SIZE];

void *operator new(size_t size) {
    static char *s = pool;
    char *t = s;
    s += size;
    return t;
}

void operator delete(void *) {}

void operator delete(void *, size_t) {}

const int MAXN = 500000;
const int BOUND = 1000000;

struct Node *null;

struct Node {
    Node *lc, *rc;
    int size;
    long long sum;
};

Node *root[MAXN + 1];

void insert(Node *&p, Node *pre, int l, int r, int v) {
    if (l == r) return;
    p = new Node(*pre);
    p->size++;
    p->sum += v;
    int mid = (l + r) >> 1;
    v <= mid ? insert(p->lc, pre->lc, l, mid, v)
             : insert(p->rc, pre->rc, mid + 1, r, v);
}

int query(Node *L, Node *R, int l, int r, int k) {
    int cnt = R->size - L->size;
    if (cnt == 0) return -1;
    if (k + cnt - 1 >= r) return k + cnt - 1;
    if (k < l) return -1;
    int mid = (l + r) >> 1;
    return std::max(
        query(L->lc, R->lc, l, mid, k),
        query(L->rc, R->rc, mid + 1, r, k + R->lc->size - L->lc->size));
}

long long query(Node *L, Node *R, int l, int r, int s, int t) {
    if (s <= l && t >= r) return R->sum - L->sum;
    int mid = (l + r) >> 1;
    long long ret = 0;
    if (s <= mid) ret += query(L->lc, R->lc, l, mid, s, t);
    if (t > mid) ret += query(L->rc, R->rc, mid + 1, r, s, t);
    return ret;
}

int n, m, a[MAXN + 1];

long long sum[MAXN + 1];

inline long long calc(int l, int r) {
    return l > r ? 0 : (l + r) * (r - l + 1ll) / 2;
}

int main() {
    null = new Node;
    null->lc = null->rc = null;
    null->size = null->sum = 0;
    root[0] = null;
    io >> n >> m;
    for (int i = 1; i <= n; i++) io >> a[i];
    for (int i = 1; i <= n; i++) sum[i] = sum[i - 1] + a[i];
    for (int i = 1; i <= n; i++) insert(root[i], root[i - 1], 1, BOUND, a[i]);
    for (int l, r, k, p; m--;) {
        io >> l >> r >> k;
        p = std::max(-1, query(root[l - 1], root[r], 1, BOUND, k) - k);
        long long ret = calc(k, k + p) - calc(k + p + 1, k + r - l);
        ret += sum[r] - sum[l - 1];
        if (p >= 0) {
            ret -= 2 * query(root[l - 1], root[r], 1, BOUND, 1, k + p);
        }
        io << ret << '\n';
    }
    return 0;
}
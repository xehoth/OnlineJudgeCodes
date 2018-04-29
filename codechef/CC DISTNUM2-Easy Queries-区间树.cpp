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
 * 「CC DISTNUM2」Easy Queries 29-04-2018
 * 区间树
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

const int POOL_SIZE = 1024 * 1024 * 400;

char pool[POOL_SIZE];

void *operator new(size_t size) {
    static char *s = pool;
    char *t = s;
    s += size;
    return t;
}

void operator delete(void *) {}

void operator delete(void *, size_t) {}

struct RangeTree2D {
    using Node = RangeTree2D;
    int l, r;
    std::vector<int> d;
    Node *lc, *rc;

    RangeTree2D(const std::vector<std::pair<int, int> > &a, int l, int r) {
        this->l = a[l].first;
        this->r = a[r].first;
        if (l == r) {
            d.push_back(a[l].second);
            lc = rc = nullptr;
            return;
        }
        int mid = (l + r) >> 1;
        lc = new Node(a, l, mid);
        rc = new Node(a, mid + 1, r);
        d.resize(r - l + 1);
        std::merge(lc->d.begin(), lc->d.end(), rc->d.begin(), rc->d.end(),
                   d.begin());
    }

    int query(int l, int r) {
        l = std::lower_bound(d.begin(), d.end(), l) - d.begin();
        r = std::upper_bound(d.begin(), d.end(), r) - d.begin();
        return r - l;
    }

    int query(int lX, int rX, int lY, int rY) {
        if (lX <= l && rX >= r) return query(lY, rY);
        if (r < lX || l > rX) return 0;
        return lc->query(lX, rX, lY, rY) + rc->query(lX, rX, lY, rY);
    }
};

struct RangeTree3D {
    using Node = RangeTree3D;
    int l, r;
    std::vector<std::pair<int, int> > d;
    RangeTree2D *tree;
    Node *lc, *rc;

    RangeTree3D(const std::vector<std::pair<int, std::pair<int, int> > > &a,
                int l, int r) {
        this->l = a[l].first;
        this->r = a[r].first;
        if (l == r) {
            d.emplace_back(a[l].second);
        } else {
            int mid = (l + r) >> 1;
            lc = new Node(a, l, mid);
            rc = new Node(a, mid + 1, r);
            d.resize(r - l + 1);
            std::merge(lc->d.begin(), lc->d.end(), rc->d.begin(), rc->d.end(),
                       d.begin());
        }
        tree = new RangeTree2D(d, 0, d.size() - 1);
    }

    int query(int lX, int rX, int lY, int rY, int lZ, int rZ) {
        if (lX <= l && rX >= r) return tree->query(lY, rY, lZ, rZ);
        if (r < lX || l > rX) return 0;
        return lc->query(lX, rX, lY, rY, lZ, rZ) +
               rc->query(lX, rX, lY, rY, lZ, rZ);
    }

    int query(int l, int r, int k) {
        if (this->l == this->r) return this->l;
        int cnt = lc->tree->query(0, l - 1, l, r);
        return k <= cnt ? lc->query(l, r, k) : rc->query(l, r, k - cnt);
    }
};

const int MAXN = 100000 + 9;

int a[MAXN], num[MAXN], pre[MAXN];

int main() {
    int n, q;
    io >> n >> q;
    std::vector<std::pair<int, int> > b(n);
    for (int i = 1; i <= n; i++) {
        io >> a[i];
        b[i - 1] = {a[i], i};
    }
    std::sort(b.begin(), b.end());
    for (int i = 0, idx; i < n; i++) {
        idx = b[i].second;
        if (i == 0 || b[i].first != b[i - 1].first) {
            num[i + 1] = a[idx];
            a[idx] = i + 1;
        } else {
            a[idx] = a[b[i - 1].second];
        }
    }
    num[0] = -1;

    std::vector<std::pair<int, std::pair<int, int> > > pts(n);
    for (int i = 1; i <= n; i++) {
        pts[i - 1] = {a[i], {pre[a[i]], i}};
        pre[a[i]] = i;
    }
    std::sort(pts.begin(), pts.end());

    RangeTree3D *tree = new RangeTree3D(pts, 0, n - 1);

    int ans = 0, inf = pts[n - 1].first;
    for (int a, b, c, d, k, l, r, idx; q--;) {
        io >> a >> b >> c >> d >> k;
        l = (a * std::max(ans, 0) + b) % n + 1;
        r = (c * std::max(ans, 0) + d) % n + 1;
        if (l > r) std::swap(l, r);
        idx = tree->query(l, r, k);
        if (idx == inf && tree->query(0, idx, 0, l - 1, l, r) != k) {
            idx = 0;
        }
        ans = num[idx];
        io << ans << '\n';
    }
    return 0;
}
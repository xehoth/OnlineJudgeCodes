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
 * 「BZOJ 3533」向量集 27-02-2018
 * 线段树 + 凸包 + 三分
 * @author xehoth
 */
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <vector>

inline void *operator new(size_t size) {
    static const int POOL_SIZE = 1024 * 1024 * 70;
    static char pool[POOL_SIZE], *s = pool;
    char *t = s;
    s += size;
    return t;
}

inline void operator delete(void *) {}

inline void operator delete(void *, size_t) {}

struct InputOutputStream {
    enum { SIZE = 1 << 18 | 1 };

    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}

    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
               s == t ? -1 : *s++;
    }

    template <typename T>
    inline void read(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
    }

    inline void read(char &c) {
        while (c = read(), isspace(c) && c != -1)
            ;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
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

struct Point {
    int x, y;

    Point() {}

    Point(int x, int y) : x(x), y(y) {}

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline long long operator*(const Point &p) const {
        return (long long)x * p.y - (long long)y * p.x;
    }

    inline bool operator<(const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};

inline long long dot(const Point &a, const Point &b) {
    return (long long)a.x * b.x + (long long)a.y * b.y;
}

const int MAXM = 1048574 + 3;

typedef std::vector<Point> Vector;

struct Node {
    Vector up, down;
    bool f;
} d[MAXM];

int M, n, online, cnt;

long long lastans;

inline int decode(int x) { return x ^ (lastans & 0x7fffffff); }

inline bool fUp(long long x) { return x >= 0; }

inline bool fDown(long long x) { return x <= 0; }

template <bool (*f)(long long)>
inline void update(Vector &c, Vector &l, Vector &r) {
    c.resize(l.size() + r.size());
    std::merge(l.begin(), l.end(), r.begin(), r.end(), c.begin());
    int top = 0, n = c.size();
    for (int i = 0; i < n; i++) {
        while (top > 1 && f((c[top - 1] - c[top - 2]) * (c[i] - c[top - 2])))
            top--;
        c[top++] = c[i];
    }
    c.resize(top);
}

inline void maintain(int k) {
    Node &lc = d[k << 1], &rc = d[k << 1 | 1];
    update<fUp>(d[k].up, lc.up, rc.up);
    update<fDown>(d[k].down, lc.down, rc.down);
}

inline long long get(const Vector &c, const Point &p) {
    int l = 0, r = c.size() - 1;
    for (int lMid, rMid; r - l > 2;) {
        lMid = l + (r - l) / 3;
        rMid = r - (r - l) / 3;
        if (dot(c[lMid], p) > dot(c[rMid], p))
            r = rMid;
        else
            l = lMid;
    }
    long long ans = LLONG_MIN;
    for (; l <= r; l++) ans = std::max(ans, dot(c[l], p));
    return ans;
}

inline long long query(int k, const Point &p) {
    return p.y > 0 ? get(d[k].up, p) : get(d[k].down, p);
}

inline long long query(int s, int t, const Point &p) {
    long long ret = LLONG_MIN;
    for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
        if (~s & 1) ret = std::max(ret, query(s ^ 1, p));
        if (t & 1) ret = std::max(ret, query(t ^ 1, p));
    }
    return ret;
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    char cmd;
    io >> n >> cmd;
    online = (cmd != 'E');
    for (M = 1; M < n + 2;) M <<= 1;
    Point pts;
    for (int i = 1, l, r, p; i <= n; i++) {
        io >> cmd;
        io >> pts.x >> pts.y;
        if (online) {
            pts.x = decode(pts.x);
            pts.y = decode(pts.y);
        }
        switch (cmd) {
            case 'A': {
                d[p = ++cnt + M].f = true;
                d[p].up.push_back(pts);
                d[p].down.push_back(pts);
                for (p >>= 1; p && d[p << 1 | 1].f; p >>= 1) {
                    maintain(p);
                    d[p].f = true;
                    d[p << 1 | 1].f = false;
                }
                break;
            }
            case 'Q': {
                io >> l >> r;
                if (online) {
                    l = decode(l);
                    r = decode(r);
                }
                lastans = query(l, r, pts);
                io << lastans << '\n';
                break;
            }
        }
    }
}
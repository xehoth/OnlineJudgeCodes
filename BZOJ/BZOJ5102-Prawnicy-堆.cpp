/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 5102」Prawnicy 06-02-2018
 * 堆
 * @author xehoth
 */
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdio>
#include <iostream>
#include <queue>

inline void *operator new(size_t size) {
    static const int POOL_SIZE = 1024 * 1024 * 40;
    static char pool[POOL_SIZE], *s = pool;
    char *t = s;
    s += size;
    return t;
}

inline void operator delete(void *) {}

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
    inline InputOutputStream &operator>>(T &x) {
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

class PriorityQueue
    : public std::priority_queue<int, std::vector<int>, std::greater<int> > {
   private:
    typedef std::priority_queue<int, std::vector<int>, std::greater<int> >
        super;

   public:
    inline int &operator[](const int i) { return super::c[i]; }

    inline void resize(const int n) { super::c.resize(n); }

    inline void reserve(const int n) { super::c.reserve(n); }

    inline void init() {
        std::make_heap(super::c.begin(), super::c.end(), std::greater<int>());
    }
};

struct Node {
    int l, r, id;
    inline bool operator<(const Node &p) const { return l < p.l; }
};

int ans, L = INT_MAX, R = INT_MIN;

inline void update(int l, int r) { (r - l > ans) && (ans = (R = r) - (L = l)); }

int main() {
    int n, k;
    io >> n >> k;
    std::vector<Node> d(n);
    for (int i = 0; i < n; i++) {
        io >> d[i].l >> d[i].r;
        d[i].id = i + 1;
    }
    std::sort(d.begin(), d.end());
    PriorityQueue q;
    q.resize(k);
    for (int i = 0; i < k; i++) q[i] = d[i].r;
    q.init();
    update(d[k - 1].l, q.top());
    for (int i = k, x; i < n; i++) {
        x = d[i].r;
        if (x > q.top()) {
            q.pop();
            q.push(x);
        }
        update(d[i].l, q.top());
    }
    io << ans << '\n';
    for (int i = 0; k && i < n; i++) {
        if (d[i].l <= L && d[i].r >= R) {
            io << d[i].id << ' ';
            k--;
        }
    }
    return 0;
}
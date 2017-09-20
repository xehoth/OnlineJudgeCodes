/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2120」数颜色 20-09-2017
 * 带修改莫队
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
}

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = 0;
    return s;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

    ~InputOutputStream() { flush(); }
} io;
}

namespace {
using IO::io;

const int MAXN = 10000;
const int MAXC = 1000000;

int id[MAXN + 1];

struct Query {
    int l, r, id, time, *ans;

    Query(int l = 0, int r = 0, int id = 0, int time = 0, int *ans = NULL)
        : l(l), r(r), id(id), time(time), ans(ans) {}

    inline bool operator<(const Query &q) const {
        return ::id[l] != ::id[q.l]
                   ? ::id[l] < ::id[q.l]
                   : (::id[r] != ::id[q.r] ? ::id[r] < ::id[q.r]
                                           : time < q.time);
    }
};

struct Modify {
    int p, c, last;

    Modify(int p = 0, int c = 0, int last = 0) : p(p), c(c), last(last) {}
};

struct Task {
    int n, m, blockSize;
    int color[MAXN + 1], ans[MAXN + 1];

    Query query[MAXN + 1];
    Modify modify[MAXN + 1];
    int qcnt, mcnt;

    inline void inTime(int i, int l, int r) {
        register int pos = modify[i].p, c = modify[i].c, &last = modify[i].last;
        if (l <= pos && r >= pos) shrink(pos);
        last = color[pos], color[pos] = c;
        if (l <= pos && r >= pos) expand(pos);
    }

    inline void outTime(int i, int l, int r) {
        register int pos = modify[i].p, &last = modify[i].last;
        if (l <= pos && r >= pos) shrink(pos);
        color[pos] = last;
        if (l <= pos && r >= pos) expand(pos);
    }

    int nowAns, cnt[MAXC + 1];

    inline void shrink(int i) {
        cnt[color[i]]--;
        if (cnt[color[i]] == 0) nowAns--;
    }

    inline void expand(int i) {
        cnt[color[i]]++;
        if (cnt[color[i]] == 1) nowAns++;
    }

    inline void solveCase() {
        register int l = 1, r = 0, now = 0;
        for (register int i = 1; i <= qcnt; i++) {
            while (r < query[i].r) expand(++r);
            while (r > query[i].r) shrink(r--);
            while (l < query[i].l) shrink(l++);
            while (l > query[i].l) expand(--l);
            while (now < query[i].time) inTime(++now, l, r);
            while (now > query[i].time) outTime(now--, l, r);
            *query[i].ans = nowAns;
        }
    }

    inline void solve() {
        io >> n >> m;
        blockSize = sqrt(n) * 1.3;
        for (register int i = 1; i <= n; i++) id[i] = (i - 1) / blockSize + 1;
        for (register int i = 1; i <= n; i++) io >> color[i];
        register char cmd;
        for (register int i = 1, l, r; i <= m; i++) {
            io >> cmd;
            switch (cmd) {
                case 'Q':
                    io >> l >> r, qcnt++;
                    query[qcnt] = Query(l, r, i, mcnt, &ans[qcnt]);
                    break;
                case 'R':
                    io >> l >> r;
                    modify[++mcnt] = Modify(l, r);
                    break;
            }
        }
        std::sort(query + 1, query + qcnt + 1);
        solveCase();
        for (register int i = 1; i <= qcnt; i++) io << ans[i] << '\n';
    }
} task;
}

int main() {
    task.solve();
    return 0;
}
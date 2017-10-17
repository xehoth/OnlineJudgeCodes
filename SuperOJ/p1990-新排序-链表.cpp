/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 1990」新排序 17-10-2017
 * 链表
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

typedef std::list<std::list<int> > List;
List list;

inline void solveCase() {
    list.clear();
    register int n;
    static std::list<int> now;
    io >> n, now.clear();
    for (register int i = 1, v, last = -1; i <= n; i++) {
        io >> v;
        if (v < last) list.push_back(now), now.clear();
        last = v, now.push_back(v);
    }
    if (!now.empty()) list.push_back(now);
    while (list.begin() != --list.end()) {
        for (register List::iterator it = list.begin(); it != list.end();
             it++) {
            if (it != list.begin()) it->pop_front();
            if (!it->empty() && it != --list.end()) it->pop_back();
        }
        while (!list.empty() && list.begin()->empty()) list.pop_front();
        for (register List::iterator it = list.begin(), next; it != list.end();
             it++) {
            while (++(next = it) != list.end()) {
                if (next->empty() || next->front() >= it->back())
                    it->splice(it->end(), *next), list.erase(next);
                else
                    break;
            }
        }
    }
    if (!list.empty()) {
        io << list.begin()->size() << '\n';
        for (register std::list<int>::iterator it = list.begin()->begin();
             it != list.begin()->end(); it++)
            io << *it << ' ';
        io << '\n';
    } else {
        io << "0\n\n";
    }
}

inline void solve() {
    register int T;
    io >> T;
    while (T--) solveCase();
}
}

int main() {
    solve();
    return 0;
}
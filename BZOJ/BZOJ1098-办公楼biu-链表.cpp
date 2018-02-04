/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 1098」办公楼biu 04-02-2018
 * 链表
 * @author xehoth
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <list>
#include <queue>
#include <vector>

inline void *operator new(size_t size) {
    static const int POOL_SIZE = 1024 * 1024 * 60;
    static char pool[POOL_SIZE], *s = pool;
    char *t = s;
    s += size;
    return t;
}

inline void operator delete(void *) {}

struct InputOutputStream {
    enum { SIZE = 1 << 18 | 1 };
    char ibuf[SIZE], obuf[SIZE], *s, *t, *oh;

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

    InputOutputStream() : s(), t(), oh(obuf) {}

    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }
} io;

typedef std::list<int> List;
List l;
int n, m, idx;
std::vector<std::vector<int> > g;
std::vector<int> cnt, vis;

inline void addEdge(const int u, const int v) {
    g[u].push_back(v);
    g[v].push_back(u);
}

inline void init() {
    l.resize(n);
    List::iterator it = l.begin();
    for (int i = 0; i < n; ++it, i++) *it = i;
}

inline void bfs(const int s) {
    static std::queue<int> q;
    int sz = 1;
    vis[s] = s;
    q.push(s);
    for (int u; !q.empty();) {
        u = q.front();
        q.pop();
        idx++;
        for (int i = 0; i < (int)g[u].size(); i++) vis[g[u][i]] = idx;
        for (List::iterator it = l.begin(); it != l.end(); ++it) {
            while (it != l.end() && vis[*it] != idx) {
                q.push(*it);
                it = l.erase(it);
                sz++;
            }
        }
    }
    cnt.push_back(sz);
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    io >> n >> m;
    vis.resize(n, -1);
    cnt.reserve(n);
    g.resize(n);
    for (int i = 0, u, v; i < m; i++) {
        io >> u >> v;
        addEdge(u - 1, v - 1);
    }
    init();
    for (int u; !l.empty();) {
        u = l.front();
        l.pop_front();
        bfs(u);
    }
    io << (int)cnt.size() << '\n';
    std::sort(cnt.begin(), cnt.end());
    for (int i = 0; i < (int)cnt.size(); i++) io << cnt[i] << ' ';
    return 0;
}
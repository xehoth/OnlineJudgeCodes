/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 1984」玩积木 15-09-2017
 * 搜索
 * @author xehoth
 */
#include <bits/stdc++.h>
#include <tr1/unordered_map>

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

namespace Force {

using IO::io;

struct Data {
    int v[6][6];

    inline bool operator==(const Data &d) const {
        return v[0][0] == d.v[0][0] && v[1][0] == d.v[1][0] &&
               v[1][1] == d.v[1][1] && v[2][0] == d.v[2][0] &&
               v[2][1] == d.v[2][1] && v[2][2] == d.v[2][2] &&
               v[3][0] == d.v[3][0] && v[3][1] == d.v[3][1] &&
               v[3][2] == d.v[3][2] && v[3][3] == d.v[3][3] &&
               v[4][0] == d.v[4][0] && v[4][1] == d.v[4][1] &&
               v[4][2] == d.v[4][2] && v[4][3] == d.v[4][3] &&
               v[4][4] == d.v[4][4] && v[5][0] == d.v[5][0] &&
               v[5][1] == d.v[5][1] && v[5][2] == d.v[5][2] &&
               v[5][3] == d.v[5][3] && v[5][4] == d.v[5][4] &&
               v[5][5] == d.v[5][5];
    }
};

inline int hash(const Data &t) {
    register int ret = 0;
    for (register int i = 0; i < 6; i++)
        for (register int j = 0; j <= i; j++) ret = ret * 5 + t.v[i][j];
    return ret;
}

std::tr1::unordered_map<int, int> vis;

Data O;

int ans;

void dfs(const int x, const int y, Data now, int step) {
    if (step > ans) return;
    if (step > 14) return;
    if (now == O) {
        ans = std::min(ans, step);
        return;
    }
    if (x > 0 && y > 0) {
        Data tmp = now;
        std::swap(tmp.v[x][y], tmp.v[x - 1][y - 1]);
        register int h;
        if (!vis[h = hash(tmp)]) {
            vis[h]++;
            dfs(x - 1, y - 1, tmp, step + 1);
            vis[h]--;
        }
    }
    if (x > 0 && y <= x - 1) {
        register int h;
        Data tmp = now;
        std::swap(tmp.v[x][y], tmp.v[x - 1][y]);
        if (!vis[h = hash(tmp)]) {
            vis[h]++;
            dfs(x - 1, y, tmp, step + 1);
            vis[h]--;
        }
    }
    if (x < 5) {
        Data tmp = now;
        std::swap(tmp.v[x][y], tmp.v[x + 1][y]);
        register int h;
        if (!vis[h = hash(tmp)]) {
            vis[h]++;
            dfs(x + 1, y, tmp, step + 1);
            vis[h]--;
        }
    }
    if (x < 5 && y < 5) {
        Data tmp = now;
        std::swap(tmp.v[x][y], tmp.v[x + 1][y + 1]);
        register int h;
        if (!vis[h = hash(tmp)]) {
            vis[h]++;
            dfs(x + 1, y + 1, tmp, step + 1);
            vis[h]--;
        }
    }
}

Data d;

inline void solve() {
    vis.clear();
    O.v[0][0] = 0;
    O.v[1][0] = O.v[1][1] = 1;
    O.v[2][0] = O.v[2][1] = O.v[2][2] = 2;
    O.v[3][0] = O.v[3][1] = O.v[3][2] = O.v[3][3] = 3;
    O.v[4][0] = O.v[4][1] = O.v[4][2] = O.v[4][3] = O.v[4][4] = 4;
    O.v[5][0] = O.v[5][1] = O.v[5][2] = O.v[5][3] = O.v[5][4] = O.v[5][5] = 5;
    io >> d.v[0][0] >> d.v[1][0] >> d.v[1][1] >> d.v[2][0] >> d.v[2][1] >>
        d.v[2][2] >> d.v[3][0] >> d.v[3][1] >> d.v[3][2] >> d.v[3][3] >>
        d.v[4][0] >> d.v[4][1] >> d.v[4][2] >> d.v[4][3] >> d.v[4][4] >>
        d.v[5][0] >> d.v[5][1] >> d.v[5][2] >> d.v[5][3] >> d.v[5][4] >>
        d.v[5][5];
    register int initX, initY;
    for (register int i = 0; i < 6; i++) {
        for (register int j = 0; j <= i; j++) {
            if (d.v[i][j] == 0) {
                initX = i, initY = j;
                goto end;
            }
        }
    }
end:;
    ans = INT_MAX;
    dfs(initX, initY, d, 0);
    if (ans == INT_MAX) {
        io << "too difficult\n";
    } else {
        io << ans << '\n';
    }
}
}

namespace {

using IO::io;

inline void solve() {
    register int T;
    io >> T;
    while (T--) {
        Force::solve();
    }
}
}

int main() {
    // freopen("blocks.in", "r", stdin);
    // freopen("blocks.out", "w", stdout);
    solve();
    return 0;
}
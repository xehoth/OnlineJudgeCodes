/*
 * created by xehoth on 19-02-2017
 */
#include <bits/stdc++.h>

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
inline void read(T &x) {
    static bool iosig;
    static char c;
    for (iosig = false, c = read(); !isdigit(c); c = read()) {
        if (c == '-') iosig = true;
        if (c == -1) return;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
}

template <class T1, class T2>
inline void read(T1 &a, T2 &b) {
    read(a), read(b);
}

template <class T1, class T2, class T3>
inline void read(T1 &a, T2 &b, T3 &c) {
    read(a), read(b), read(c);
}

template <class T1, class T2, class T3, class T4>
inline void read(T1 &a, T2 &b, T3 &c, T4 &d) {
    read(a), read(b), read(c), read(d);
}

const int OUT_LEN = 1000000;
char obuf[OUT_LEN], *oh = obuf;

inline void writeChar(char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        writeChar(48);
    } else {
        if (x < 0) writeChar('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) writeChar(buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

#define long long long

int main() {
    register int t, n, m;
    read(t);
    static int map[1002][1002];
    static long f[1002][1002];

    while (t--) {
        read(m, n);
        register long ans = 0;
        for (register int i = 1; i <= m; i++)
            for (register int j = 1; j <= n; j++) read(map[i][j]);

        memset(f, 0, sizeof(f));
        for (register int j = 1; j <= n; j++) {
            for (register int i = m; i; i--)
                f[i][j] = std::max(f[i][j - 1], f[i + 1][j - 1] + map[i][j]);
            for (register int i = m; i; i--)
                f[i][j] = std::max(f[i][j], f[i + 1][j]);
            ans = std::max(ans, f[1][j]);
        }
        print(ans), writeChar('\n');
    }
    flush();
    return 0;
}
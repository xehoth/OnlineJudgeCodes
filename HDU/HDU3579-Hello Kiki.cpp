/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「HDU 3579」Hello Kiki 30-06-2017
 * 中国剩余定理
 * @author xehoth
 */
#include <bits/stdc++.h>

class InputOutputStream {
   private:
    static const int BUFFER_SIZE = 1024 * 1024;
    char ibuf[BUFFER_SIZE], obuf[BUFFER_SIZE], *s, *t, *oh;
    bool isEof;
    std::streambuf *cinBuf, *coutBuf;

   public:
    InputOutputStream(char *in = NULL, char *out = NULL)
        : s(ibuf), oh(obuf), isEof(false) {
        std::ios::sync_with_stdio(false);
        std::cin.tie(NULL);
        std::cout.tie(NULL);
        in ? freopen(in, "r", stdin) : 0;
        out ? freopen(out, "w", stdout) : 0;
        cinBuf = std::cin.rdbuf(), coutBuf = std::cout.rdbuf();
        t = ibuf + cinBuf->sgetn(ibuf, BUFFER_SIZE);
    }

    ~InputOutputStream() { coutBuf->sputn(obuf, oh - obuf); }

    inline char read();
    template <typename T>
    inline void read(T &);
    inline int read(char *);
    inline void print(char);
    inline void print(const char *);
    template <typename T>
    inline void print(T);
    template <typename T>
    inline InputOutputStream &operator>>(T &);
    template <typename T>
    inline InputOutputStream &operator<<(T);
    inline bool hasNext() const { return !isEof; }
}
#ifndef ONLINE_JUDGE
#ifdef DBG
io((char *)"in.in");
#else
io;
#endif
#else
io;
#endif

#define long long long

template <typename T>
inline void exgcd(T a, T b, T &g, T &x, T &y) {
    !b ? (x = 1, y = 0, g = a) : (exgcd(b, a % b, g, y, x), y -= (a / b) * x);
}

template <typename T>
inline T excrt(T *a, T *m, int n) {
    register T M = m[0], ans = a[0], g, x, y;
    for (register int i = 1; i < n; i++) {
        exgcd(M, m[i], g, x, y);
        if ((a[i] - ans) % g) return -1;
        x = (a[i] - ans) / g * x % (m[i] / g);
        ans = (ans + x * M) % (M = M / g * m[i]);
    }
    return ans > 0 ? ans : ans + M;
}

const int MAXN = 100005;

int main() {
    register int t, n, cas = 1;
    for (io >> t; t--; cas++) {
        io >> n;
        static long a[MAXN], m[MAXN];
        for (register int i = 0; i < n; i++) io >> m[i];
        for (register int i = 0; i < n; i++) io >> a[i];
        io << "Case " << cas << ": " << excrt(a, m, n) << '\n';
    }
    return 0;
}

inline char InputOutputStream::read() {
    s == t ? t = (s = ibuf) + cinBuf->sgetn(ibuf, BUFFER_SIZE) : 0;
    return (char)(s == t ? -1 : *s++);
}

template <typename T>
inline void InputOutputStream::read(T &x) {
    register char c;
    register bool iosig = false;
    for (c = read(); !isdigit(c); c = read()) {
        if (c == -1) return (void)(isEof = true);
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

inline int InputOutputStream::read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0, isEof = true;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    c == -1 ? isEof = true : 0;
    buf[s] = 0;
    return s;
}

inline void InputOutputStream::print(char c) {
    oh == obuf + BUFFER_SIZE ? (coutBuf->sputn(obuf, BUFFER_SIZE), oh = obuf)
                             : 0;
    *oh++ = c;
}

inline void InputOutputStream::print(const char *s) {
    for (; *s; s++) print(*s);
}

template <typename T>
inline void InputOutputStream::print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

template <typename T>
inline InputOutputStream &InputOutputStream::operator>>(T &x) {
    read(x);
    return *this;
}

template <typename T>
inline InputOutputStream &InputOutputStream::operator<<(T x) {
    print(x);
    return *this;
}
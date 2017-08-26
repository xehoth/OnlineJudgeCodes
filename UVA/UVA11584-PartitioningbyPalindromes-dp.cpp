/**
 * 「UVA 11584」Partitioning by Palindromes 05-06-2017
 * dp
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
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}

namespace Task {

const int MAXN = 1010;

bool isPalindrome[MAXN][MAXN];
char s[MAXN];

inline void solve() {
    using namespace IO;
    register int t;
    for (read(t); t--;) {
        register int n = read(s);
        memset(isPalindrome, 0, sizeof(isPalindrome));
        for (register int i = 0; i < n; i++) {
            for (register int l = i, r = i; l >= 0 && r < n && s[l] == s[r];
                 l--, r++)
                isPalindrome[l + 1][r + 1] = isPalindrome[r + 1][l + 1] = true;
            for (register int l = i, r = i + 1; l >= 0 && r < n && s[l] == s[r];
                 l--, r++)
                isPalindrome[l + 1][r + 1] = isPalindrome[r + 1][l + 1] = true;
        }
        static int f[MAXN];
        for (register int i = 1; i <= n; i++) {
            f[i] = i;
            for (register int j = 1; j <= i; j++)
                if (isPalindrome[i][j]) f[i] = std::min(f[j - 1] + 1, f[i]);
        }
        print(f[n]), print('\n');
    }
}
}

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    Task::solve();
    IO::flush();
    return 0;
}
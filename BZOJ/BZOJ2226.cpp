/*
 * created by xehoth on 13-01-2017
 */
#include <bits/stdc++.h>

typedef long long ll;

#define FAST_IO
#ifdef FAST_IO
const int IN_LEN = 100000, OUT_LEN = 100000;
inline int nextChar() {
    static char buf[IN_LEN], *h, *t;
    if (h == t) {
        t = (h = buf) + fread(buf, 1, IN_LEN, stdin);
        if (h == t) return -1;
    }
    return *h++;
}
template <class T>
inline bool read(T &x) {
    static bool iosig = 0;
    static char c;
    for (iosig = 0, c = nextChar(); !isdigit(c); c = nextChar()) {
        if (c == -1) return false;
        if (c == '-') iosig = 1;
    }
    for (x = 0; isdigit(c); c = nextChar()) x = (x << 1) + (x << 3) + (c ^ '0');
    if (iosig) x = -x;
    return true;
}
template <class T1, class T2>
inline bool read(T1 &x1, T2 &x2) {
    return read(x1), read(x2);
}
template <class T1, class T2, class T3>
inline bool read(T1 &x1, T2 &x2, T3 &x3) {
    return read(x1), read(x2), read(x3);
}
template <class T1, class T2, class T3, class T4>
inline bool read(T1 &x1, T2 &x2, T3 &x3, T4 &x4) {
    return read(x1), read(x2), read(x3), read(x4);
}
inline bool isLineSkippable(const char c) { return c == '\n' || c == '\r'; }
inline bool isWordSkippable(const char c) {
    return c == ' ' || c == '\n' || c == '\r';
}
inline void skip() {
    for (char c = nextChar(); isWordSkippable(c); c = nextChar())
        ;
}
inline void skipLine() {
    for (char c = nextChar(); isLineSkippable(c); c = nextChar())
        ;
}
inline bool read(char *str) {
    char c;
    for (c = nextChar(); isWordSkippable(c); c = nextChar())
        if (c == -1) return false;
    for (; !isWordSkippable(c); c = nextChar()) *str++ = c;
    return true;
}
inline void readLine(char *str) {
    static char c;
    while (isLineSkippable(c = nextChar()))
        ;
    while (!isLineSkippable(c)) *str++ = c = nextChar();
}
inline int nextInt() {
    static int i;
    read(i);
    return i;
}
inline long long nextLong() {
    static long long i;
    read(i);
    return i;
}
inline std::string nextString() {
    static char buf[IN_LEN];
    read(buf);
    return std::string(buf);
}
char obuf[OUT_LEN], *oh = obuf;
inline void writeChar(const char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}
template <class T>
inline void write(T x) {
    static int buf[30], cnt;
    if (!x)
        writeChar(48);
    else {
        if (x < 0) writeChar('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) writeChar(buf[cnt--]);
    }
}
template <class T>
inline void writeln(T x) {
    write(x), writeChar('\n');
}
template <class T1, class T2>
inline void write(T1 x1, T2 x2) {
    write(x1), write(x2);
}
template <class T1, class T2, class T3>
inline void write(T1 x1, T2 x2, T3 x3) {
    write(x1), write(x2), write(x3);
}
template <class T1, class T2, class T3, class T4>
inline void write(T1 x1, T2 x2, T3 x3, T4 x4) {
    write(x1), write(x2), write(x3), write(x4);
}
inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
#endif

const int MAXN = 1000010;
int T, n;

int prime[MAXN], phi[MAXN], pcnt;
bool vis[MAXN];

inline void fastLinearSieveMethod() {
    phi[1] = 1;
    phi[2] = 1;

    ll temp;
    for (int i = 2; i < MAXN; ++i) {
        if (!vis[i]) {
            prime[pcnt] = i;
            phi[i] = i - 1;
            ++pcnt;
        }
        for (int j = 0; j < pcnt && (temp = (ll)prime[j] * i) < MAXN; ++j) {
            vis[temp] = 1;
            if (i % prime[j] == 0) {
                phi[temp] = phi[i] * prime[j];
                break;
            } else
                phi[temp] = phi[i] * (prime[j] - 1);
        }
    }
}

inline ll solve(int n) {
    ll ans = 0ll;
    register int half = (int)(sqrt(n) + 0.01);

    if (half * half == n) ans += (long long)phi[half] * half / 2, half--;
    ans += 1;
    ans += (long long)phi[n] * n / 2;
    for (register int i = 2; i <= half; i++)
        if (n % i == 0)
            ans += (long long)phi[i] * i / 2,
                ans += (long long)phi[n / i] * n / i / 2;

    return ans * n;
}

int main() {
    // freopen("in.in", "r", stdin);
    read(T);
    fastLinearSieveMethod();

    while (T--) {
        read(n);
        writeln(solve(n));
    }
    flush();
    return 0;
}
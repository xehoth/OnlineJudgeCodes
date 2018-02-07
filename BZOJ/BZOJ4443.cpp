#include <bits/stdc++.h>
using namespace std;
#define IO_L 1048576
char _buf[IO_L], *S, *Ts, _buf1[IO_L + 1], *S1 = _buf1, IO_c, IO_signum;
inline char read() {
    if (S == Ts) {
        Ts = (S = _buf) + fread(_buf, 1, IO_L, stdin);
        if (S == Ts)return 0;
    }
    return *S++;
}
inline bool read(int &x) {
    IO_signum = false;
    for (IO_c = read(); IO_c < '0' || IO_c > '9'; IO_c = read()) {
        if (IO_c == -1) return false;
        if (IO_c == '-') IO_signum = true;
    }
    x = 0;
    while (IO_c == '0') IO_c = read();
    for (;; IO_c = read()) {
        if (IO_c < '0' || IO_c > '9') break;
        x = (x << 3) + (x << 1) + (IO_c ^ '0');
    }
    if (IO_signum) x = -x;
    return true;
}
inline void write(char c) {
    if (S1 == _buf1 + IO_L) {
        fwrite(_buf1, 1, IO_L, stdout);
        S1 = _buf1;
    }
    *S1++ = c;
}
inline void flushIO() {
    fwrite(_buf1, 1, S1 - _buf1, stdout);
}
inline void writeInt(int x) {
    stack<char> st;
    while (x > 9)
        st.push(x % 10 ^ '0'), x /= 10;
    write(x^'0');
    while (!st.empty())
        write(st.top()), st.pop();
}
#define in(x,y) read(x),read(y)
#define writeln(x) writeInt(x),write('\n')
#define N 1000001
int a[255][255];
int head[N], nxt[N], lst[N], tim[N], from[N];
int n, m, tot, mx, TI, K;
inline void insert(int x, int y) {
    lst[++tot] = y, nxt[tot] = head[x], head[x] = tot;
}
inline bool Hungary(int x) {
    for (register int i = head[x]; i; i = nxt[i])
        if (tim[lst[i]] ^ TI) {
            tim[lst[i]] = TI;
            if (!from[lst[i]] || Hungary(from[lst[i]])) {
                from[lst[i]] = x;
                return true;
            }
        }
    return 0;
}
inline bool jud(int x) {
    int ans = 0; tot = 0;
    for (int i = 1; i <= n; i++)head[i] = 0;
    for (int i = 1; i <= m; i++)from[i] = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (a[i][j] <= x) insert(i, j);
    for (int i = 1; i <= n; i++)
        TI++, ans += Hungary(i);
    return ans >= K;
}
int main() {
    // freopen("in.in", "r", stdin);
    in(n, m), read(K), K = n - K + 1;
    for (register int i = 1; i <= n; i++)
        for (register int j = 1; j <= m; j++)
            read(a[i][j]), mx = max(mx, a[i][j]);
    register int l = 1, r = mx;
    while (l ^ r) {
        register int mid = l + r >> 1;
        if (jud(mid))r = mid;
        else l = mid + 1;
    }
    cout << l;
    return 0;
}

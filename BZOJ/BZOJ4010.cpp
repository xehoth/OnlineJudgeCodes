#include <bits/stdc++.h>
using namespace std;
#define IO_L 1048576
char _buf[IO_L], *S, *Ts, _buf1[IO_L + 1], *S1 = _buf1, IO_c, IO_signum;
inline char read() {
    if (S == Ts) {
        Ts = (S = _buf) + fread(_buf, 1, IO_L, stdin);
        if (S == Ts) return 0;
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
inline void flushIO() { fwrite(_buf1, 1, S1 - _buf1, stdout); }
inline void writeInt(int x) {
    stack<char> st;
    while (x > 9) st.push(x % 10 ^ '0'), x /= 10;
    write(x ^ '0');
    while (!st.empty()) write(st.top()), st.pop();
}
#define in(x, y) read(x), read(y)
#define writeln(x) writeInt(x), write('\n')
int T, n, m, cnt, top;
int last[100005], d[100005], ans[100005];
priority_queue<int> q;
struct edge {
    int to, next;
} e[100005];
void insert(int u, int v) {
    e[++cnt] = (edge){v, last[u]};
    last[u] = cnt;
}
void solve(int x) {
    q.pop(), ans[++top] = x;
    for (register int i = last[x]; i; i = e[i].next) {
        d[e[i].to]--;
        if (!d[e[i].to]) q.push(e[i].to);
    }
}
int main() {
    // freopen("in.in", "r", stdin);
    read(T);
    while (T--) {
        cnt = top = 0, memset(last, 0, sizeof(last)), memset(d, 0, sizeof(d));
        in(n, m);
        for (register int i = 1, u, v; i <= m; i++)
            in(u, v), insert(v, u), d[u]++;
        for (register int i = 1; i <= n; i++)
            if (!d[i]) q.push(i);
        while (!q.empty()) solve(q.top());
        if (top ^ n)
            puts("Impossible!");
        else {
            for (register int i = n; i; i--)
                printf("%d ", ans[i], i == 1 ? '\n' : ' ');
            putchar('\n');
        }
    }
    return 0;
}
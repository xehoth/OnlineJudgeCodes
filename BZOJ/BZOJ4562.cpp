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
#define N 100005
#define M 200005
vector<int> edge[M];
int indgr[N], outdgr[N], f[N];
inline int dfs(int u) {
    if (f[u] ^ -1) return f[u];
    if (!outdgr[u]) return f[u] = 1;
    f[u] = 0;
    for (register int i = 0; i < edge[u].size(); i++) f[u] += dfs(edge[u][i]);
    return f[u];
}
int n, m, ans;
int main() {
    // freopen("in.in", "r", stdin);
    in(n, m);
    for (register int i = 1, u, v; i <= m; i++)
        in(u, v), indgr[v]++, outdgr[u]++, edge[u].push_back(v);
    memset(f, -1, sizeof(int) * (n + 1));
    for (register int i = 1; i <= n; i++)
        if (!indgr[i] && outdgr[i]) dfs(i), ans += f[i];
    cout << ans;
    return 0;
}
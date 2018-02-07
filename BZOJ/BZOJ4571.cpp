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
#define N 200010
#define M 300000
struct segmentTree {
    int ls, rs, sum;
    segmentTree() : ls(0), rs(0), sum(0) {}
} tr[N * 20];
int root[N], sz;
int n, m, x, b, l, r, a;
inline void modify(int &cur, int pre, int l, int r, int v) {
    cur = ++sz, tr[cur].ls = tr[pre].ls, tr[cur].rs = tr[pre].rs,
    tr[cur].sum = tr[pre].sum + 1;
    if (l == r) return;
    register int mid = l + r >> 1;
    if (v <= mid)
        modify(tr[cur].ls, tr[pre].ls, l, mid, v);
    else
        modify(tr[cur].rs, tr[pre].rs, mid + 1, r, v);
}
inline bool query(int cur, int pre, int l, int r, int ll, int rr) {
    if (l == ll && r == rr) return tr[pre].sum - tr[cur].sum > 0;
    register int mid = l + r >> 1;
    if (rr <= mid)
        return query(tr[cur].ls, tr[pre].ls, l, mid, ll, rr);
    else if (mid < ll)
        return query(tr[cur].rs, tr[pre].rs, mid + 1, r, ll, rr);
    else
        return query(tr[cur].ls, tr[pre].ls, l, mid, ll, mid) |
               query(tr[cur].rs, tr[pre].rs, mid + 1, r, mid + 1, rr);
}
#define in(x, y) read(x), read(y)
#define writeln(x) writeInt(x), write('\n')
int main() {
    // freopen("in.in", "r", stdin);
    in(n, m);
    for (register int i = 1; i <= n; i++)
        read(x), modify(root[i], root[i - 1], 0, M, x);
    for (register int i = 1; i <= m; i++) {
        in(b, x), in(l, r), a = 0;
        for (register int j = 17; ~j; j--) {
            if (b & (1 << j)) {
                register int ll = max(a - x, 0), rr = (a | ((1 << j) - 1)) - x;
                if (rr < 0 || !query(root[l - 1], root[r], 0, M, ll, rr))
                    a ^= (1 << j);
            } else {
                a ^= (1 << j);
                register int ll = max(a - x, 0), rr = (a | ((1 << j) - 1)) - x;
                if (rr < 0 || !query(root[l - 1], root[r], 0, M, ll, rr))
                    a ^= (1 << j);
            }
        }
        writeln(a ^ b);
    }
    flushIO();
    return 0;
}
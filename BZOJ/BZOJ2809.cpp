#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
#define MAX 100010
#define max(x, y) (x ^ ((x ^ y) & -(x < y)))
using namespace std;
#ifndef XEHOTH
#define XEHOTH
#ifndef XEHOTH_IO
#define XEHOTH_IO
typedef long long Long;
#define XEHOTH_IO_SIZE 1048576
char IO_rbuf[XEHOTH_IO_SIZE], *IO_S, *IO_T, IO_c;
bool IO_signum;
inline char freadChar() {
    if (IO_S == IO_T) {
        IO_T = (IO_S = IO_rbuf) + fread(IO_rbuf, 1, XEHOTH_IO_SIZE, stdin);
        if (IO_S == IO_T) return -1;
    }
    return *IO_S++;
}
inline bool bfreadInt(int &x) {
    IO_signum = false;
    for (IO_c = freadChar(); IO_c < '0' || IO_c > '9'; IO_c = freadChar()) {
        if (IO_c == -1) return false;
        if (IO_c == '-') IO_signum = true;
    }
    x = 0;
    while (IO_c == '0') IO_c = freadChar();
    for (;; IO_c = freadChar()) {
        if (IO_c < '0' || IO_c > '9') break;
        x = (x << 3) + (x << 1) + (IO_c ^ '0');
    }
    if (IO_signum) x = -x;
    return true;
}
#endif
#endif
__gnu_pbds::priority_queue<int, std::less<int>, __gnu_pbds::pairing_heap_tag>
    pq[MAX];
int lead[MAX], father[MAX], money[MAX], first[MAX];
Long tot[MAX], num[MAX];
int size, n, m;
Long ans;
struct Edge {
    int to, next;
} edge[MAX << 1];
inline void add(int u, int v) {
    size++;
    edge[size].to = v;
    edge[size].next = first[u];
    first[u] = size;
}
void dfs(int now) {
    if (money[now] < m) ans = max(ans, lead[now]);
    for (int u = first[now]; u ^ 0; u = edge[u].next) {
        if (edge[u].to != father[now]) {
            dfs(edge[u].to);
            pq[now].join(pq[edge[u].to]);
            num[now] += num[edge[u].to];
            tot[now] += tot[edge[u].to];
        }
    }
    while (num[now] > m) {
        num[now] -= pq[now].top();
        tot[now]--;
        pq[now].pop();
    }
    ans = max(ans, tot[now] * lead[now]);
}
int main(int argc, char *argv[]) {
    // freopen("in.in", "r", stdin);
    std::ios::sync_with_stdio(false);
    cin.tie(NULL);
    bfreadInt(n), bfreadInt(m);
    for (register int i = 1; i <= n; i++)
        bfreadInt(father[i]), bfreadInt(money[i]), bfreadInt(lead[i]),
            add(i, father[i]), add(father[i], i), pq[i].push(money[i]),
            tot[i] = 1, num[i] = money[i];
    dfs(1);
    cout << ans;
    return 0;
}

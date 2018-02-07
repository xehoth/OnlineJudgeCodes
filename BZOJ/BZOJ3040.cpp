#include <algorithm>
#include <cstdio>
#include <cstring>
#include <ext/pb_ds/priority_queue.hpp>
#include <iostream>
typedef long long ll;
#define pa pair<ll, int>
#define mp make_pair
using namespace std;
using namespace __gnu_pbds;
typedef __gnu_pbds::priority_queue<pa, greater<pa> > heap;
const int N = 1e6 + 5, M = 1e7 + 5;
const ll INF = 1e15;
inline int read() {
    char c = getchar();
    int x = 0, f = 1;
    while (c < '0' || c > '9') {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    return x * f;
}
int n, m, T, rxa, rxc, rya, ryc, rp, a, b;
int x, y, z;
struct edge {
    int v, w, ne;
} e[M];
int cnt, h[N];
inline void ins(int u, int v, int w) {
    cnt++;
    e[cnt].v = v;
    e[cnt].w = w;
    e[cnt].ne = h[u];
    h[u] = cnt;
}

ll d[N];
heap q;
heap::point_iterator id[N];
void dij() {
    for (int i = 1; i <= n; i++) d[i] = INF;
    d[1] = 0;
    id[1] = q.push(mp(0, 1));
    while (!q.empty()) {
        int u = q.top().second;
        q.pop();  // printf("u %d\n",u);
        for (int i = h[u]; i; i = e[i].ne) {
            int v = e[i].v, w = e[i].w;
            if (d[v] > d[u] + w) {
                d[v] = d[u] + w;
                if (id[v] != 0)
                    q.modify(id[v], mp(d[v], v));
                else
                    id[v] = q.push(mp(d[v], v));
            }
        }
    }
}
int main() {
    // freopen("in.txt","r",stdin);
    n = read();
    m = read();
    T = read();
    rxa = read();
    rxc = read();
    rya = read();
    ryc = read();
    rp = read();
    m = m - T;
    while (T--) {
        x = y = z = 0;
        x = ((ll)x * rxa + rxc) % rp;
        y = ((ll)y * rya + ryc) % rp;
        a = min(x % n + 1, y % n + 1);
        b = max(y % n + 1, y % n + 1);
        ins(a, b, 100000000 - 100 * a);
    }
    while (m--) x = read(), y = read(), z = read(), ins(x, y, z);
    dij();
    printf("%lld", d[n]);
}
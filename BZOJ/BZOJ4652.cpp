#include <bits/stdc++.h>
#include <tr1/unordered_map>

namespace Task {

#define long long long

const int MAXN = 300010;
const int MAXK = 2010;
int prime[MAXN], cnt, n, m, k;
long mu[MAXN];
bool vis[MAXN];

inline void fastLinearSieveMethod(const int n) {
    mu[1] = 1;
    for (register int i = 2; i <= n; i++) {
        if (!vis[i]) prime[cnt++] = i, mu[i] = -1;
        for (register int j = 0, tmp; j < cnt && (tmp = i * prime[j]) <= n;
             j++) {
            vis[tmp] = true;
            if (i % prime[j] == 0) {
                mu[tmp] = 0;
                break;
            } else {
                mu[tmp] = -mu[i];
            }
        }
    }
    for (register int i = 1; i <= n; i++) mu[i] += mu[i - 1];
}

typedef std::tr1::unordered_map<int, long> HashMap;
HashMap map[MAXK];
HashMap::iterator it;

const int sieveBlockSize = 300000;

HashMap buc;

inline long sieveMuMain(int x) {
    if (x <= sieveBlockSize) return mu[x];
    register long &cur = buc[x];
    if (cur) return cur;
    register long ret = 1;
    for (register int i = 2, pos; i <= x; i = pos + 1)
        pos = x / (x / i), ret -= (pos - i + 1) * sieveMuMain(x / i);
    return cur = ret;
}

int f[MAXK], fac[MAXK], facCnt;

inline void initFunctionF() {
    for (register int i = 1; i <= k; i++)
        f[i] = f[i - 1] + (std::__gcd(i, k) == 1);
}

inline long getF(int x) { return (long)(x / k) * f[k] + f[x % k]; }

inline long getG(int x, int y) {
    if (x <= 1) return x;
    if (y == 0) return sieveMuMain(x);
    register long &cur = map[y][x];
    return cur ? cur : cur = getG(x, y - 1) + getG(x / fac[y], y);
}

inline void initFactor() {
    register int now = k;
    for (register int i = 2; i <= k; i++) {
        if (now % i != 0) continue;
        fac[++facCnt] = i;
        while (now % i == 0) now /= i;
        if (now == 1) break;
    }
}

inline void solve() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL), std::cout.tie(NULL);
    std::cin >> n >> m >> k;
    fastLinearSieveMethod(sieveBlockSize);
    initFunctionF(), initFactor();
    register int min = std::min(n, m);
    register long ans = 0;
    for (register int i = 1, pos; i <= min; i = pos + 1) {
        pos = std::min(n / (n / i), m / (m / i));
        ans += (long)getF(m / i) * (n / i) *
               (getG(pos, facCnt) - getG(i - 1, facCnt));
    }
    std::cout << ans;
}
}

int main() {
    Task::solve();
    return 0;
}
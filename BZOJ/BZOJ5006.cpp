#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/hash_policy.hpp>

namespace {

const int MOD = 1000000007;

typedef __gnu_pbds::cc_hash_table<int, long long> HashMap;

struct Two {
    int a, b;
    Two(const int a, const int b) : a(a), b(b) {}
};

struct Four {
    int a, b, c, d, f;

    Four(int a, int b, int c, int d, int f) : a(a), b(b), c(c), d(d), f(f) {}
};

const int MAXN = 15;

std::vector<Two> v1[MAXN];
std::vector<Four> v2[MAXN];

HashMap f[1 << MAXN | 1];

inline void solve() {
    register int n, m;
    std::cin >> n >> m;
    for (register int i = 1, cmd, a, b, c, d; i <= m; i++) {
        std::cin >> cmd >> a >> b;
        a--;
        b--;
        if (cmd) {
            std::cin >> c >> d;
            c--;
            d--;
            (a > c) && (std::swap(a, c), std::swap(b, d), 0);
            v1[a].push_back(Two(a, b));
            v1[c].push_back(Two(c, d));
            if (a != c && b != d) {
                v2[a].push_back(Four(a, b, c, d, cmd == 1 ? 1 : -1));
            }
        } else {
            v1[a].push_back(Two(a, b));
        }
    }
    f[0][0] = 1;
    for (register int i = 0, w; i < (1 << n) - 1; i++) {
        w = __builtin_ctz(~i);
        for (register HashMap::iterator it = f[i].begin(); it != f[i].end();
             ++it) {
            register int st = it->first;
            register long long s = it->second % MOD;
            for (register int j = 0; j < (int)v1[w].size(); j++) {
                const Two &o = v1[w][j];
                if (!(st >> o.b & 1)) f[i + (1 << w)][st + (1 << o.b)] += s;
            }
            for (register int j = 0; j < (int)v2[w].size(); j++) {
                const Four &o = v2[w][j];
                if (!(st >> o.b & 1) && !(st >> o.d & 1) && !(i >> o.c & 1)) {
                    f[i + (1 << w) + (1 << o.c)]
                     [st + (1 << o.b) + (1 << o.d)] += o.f * s;
                }
            }
        }
    }
    std::cout << (f[(1 << n) - 1][(1 << n) - 1] % MOD + MOD) % MOD;
}
}  // namespace

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    solve();
    return 0;
}
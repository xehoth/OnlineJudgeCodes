#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

int f[2][1024][1024];

const int Mod = 1e9 + 7;

inline void up(int& a, int b) {
    a += b;
    a >= Mod ? a -= Mod : (a < 0 ? a += Mod : a);
}

int main() {
    int n;
    scanf("%d", &n);
    int now = 1, last = 0;
    f[last][0][0] = 1;
    for (int i = 1; i <= n; i++, now ^= 1, last ^= 1) {
        int p, t;
        scanf("%d", &p);
        for (int j = 0; j < 1024; j++)
            for (int k = j;; k = (--k) & j) {
                f[now][j][k] = f[last][j][k];
                if (!k) break;
            }
        p = 1023 - (t = p);
        for (int j = 0; j < 1024; j++)
            if ((j | p) ^ j)
                for (int k = j;; k = (--k) & j) {
                    up(f[now][j | p][k & t], f[last][j][k]),
                        up(f[now][j | p][(k & t) | (p ^ (p & j))],
                           -f[last][j][k]);
                    if (!k) break;
                }
    }
    cout << f[last][1023][0];

    return 0;
}
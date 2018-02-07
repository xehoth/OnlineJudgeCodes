#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
using namespace std;

int x, y, d, jj, m, n, xi, i, j;
bool f, ff;
int w[50001];

int main() {
    // freopen("hotel.in","r",stdin);
    // freopen("hotel.out","w",stdout);
    scanf("%d%d", &n, &m);
    for (i = 1; i <= m; i++) {
        scanf("%d", &x);
        if (x == 1) {
            scanf("%d", &y);
            f = false;
            j = 1;
            jj = j + y - 1;
            while (j <= n && jj <= n) {
                ff = false;
                for (d = jj; d >= j; d--)
                    if (w[d] == 1) {
                        j = d + 1;
                        jj = j + y - 1;
                        ff = true;
                        break;
                    }

                if (ff) continue;
                for (d = jj; d >= j; d--) w[d] = 1;
                f = true;
                break;
            }
            if (f)
                printf("%d\n", j);
            else
                printf("0\n");
            ;
        }

        if (x == 2) {
            scanf("%d%d", &xi, &y);
            y = xi + y - 1;
            for (d = xi; d <= y; d++) w[d] = 0;
        }
    }
    return 0;
}

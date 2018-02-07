#include <cstdio>
#define J (i + j >> 1)
#define I (J + 1)
typedef unsigned ll;
const int N = 1e5 + 5;
ll n, m, q, i, j, k, l, s, t, v;
struct node {
    ll s, t;
    node *i, *j;
};
node f[N * 320], *e[N];
node* back = f;
void vary(node*& k, int s, int t, int i = 1, int j = n) {
    if (!k) k = back++;
    k->s += t - s + 1;
    if (s == i && t == j)
        ++k->t;
    else if (t < I)
        vary(k->i, s, t, i, J);
    else if (s > J)
        vary(k->j, s, t, I, j);
    else {
        vary(k->i, s, J, i, J);
        vary(k->j, I, t, I, j);
    }
}
void eval(node* k, int s, int t, int i = 1, int j = n) {
    if (!k) return;
    if (s == i && t == j) {
        l += k->s;
        return;
    }
    l += k->t * (t - s + 1);
    if (t < I)
        eval(k->i, s, t, i, J);
    else if (s > J)
        eval(k->j, s, t, I, j);
    else {
        eval(k->i, s, J, i, J);
        eval(k->j, I, t, I, j);
    }
}
int main() {
    struct {
        operator int() {
            int x;
            scanf("%d", &x);
            return x;
        }
    } it;
    n = it, m = n << 1 ^ 1, q = it;
    while (q--) {
        j = it, s = it, t = it, k = it;
        if (j == 2) {
            for (i = 65536, v = j = 0; i; i >>= 1)
                if (j + i <= m) {
                    l = 0, eval(e[j + i], s, t);
                    if (v + l < k) j += i, v += l;
                }
            printf("%d\n", n - j);
        } else
            for (i = n - k + 1; i <= m; i += i & -i) vary(e[i], s, t);
    }
}
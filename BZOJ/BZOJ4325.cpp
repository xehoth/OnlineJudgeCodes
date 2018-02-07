#include <bits/stdc++.h>
using namespace std;
char ch;
inline void read(int &x) {
    x = 0;
    do
        ch = getchar();
    while (!isdigit(ch));
    while (isdigit(ch)) x = (x << 1) + (x << 3) + (ch ^ '0'), ch = getchar();
}
int T, n, i, x, y, ans, a[15], b[5], A, B, C, D, f[16][16][16][16];
struct Card {
    int a, b, c, d;
    Card() : a(0), b(0), c(0), d(0) {}
    Card(int a, int b, int c, int d) : a(a), b(b), c(c), d(d) {}
    inline Card operator+(const Card &x) {
        return Card(a + x.a, b + x.b, c + x.c, d + x.d);
    }
} g1[4], g2[3], g3[2], g4;
inline void up(int &a, int b) {
    if (a > b) a = b;
}
inline int get(Card x) {
    if (x.a < 0 || x.b < 0 || x.c < 0 || x.d < 0) return 30;
    if (x.a > 15 || x.b > 15 || x.c > 15 || x.d > 15) return 30;
    if (~f[x.a][x.b][x.c][x.d]) return f[x.a][x.b][x.c][x.d];
    int t = 30;
    for (int i = 0; i < 4; i++) up(t, get(x + g1[i]));
    for (int i = 0; i < 3; i++) up(t, get(x + g2[i]));
    for (int i = 0; i < 2; i++) up(t, get(x + g3[i]));
    up(t, get(x + g4));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 2; j++) up(t, get(x + g1[i] + g3[j]));
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 2; j++) up(t, get(x + g2[i] + g3[j]));
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) up(t, get(x + g4 + g1[i] + g1[j]));
    for (int i = 0; i < 3; i++) up(t, get(x + g4 + g2[i]));
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) up(t, get(x + g4 + g2[i] + g2[j]));
    up(t, get(x + g4 + g4));
    return f[x.a][x.b][x.c][x.d] = t + 1;
}
inline int cal() {
    for (int i = 2; i <= 4; i++) b[i] = 0;
    b[1] = a[0];
    for (int i = 1; i <= 13; i++) b[a[i]]++;
    return get(Card(b[1], b[2], b[3], b[4]));
}
void dfs(int x) {
    if (ans && x >= ans) return;
    x++;
    for (int i = 2; i <= 12; i++) {
        if (a[i] >= 3) {
            int j = i;
            while (a[j + 1] >= 3) j++;
            for (int k = j; k > i; k--) {
                for (int o = i; o <= k; o++) a[o] -= 3;
                dfs(x);
                for (int o = i; o <= k; o++) a[o] += 3;
            }
        }
    }
    for (int i = 2; i <= 11; i++) {
        if (a[i] >= 2) {
            int j = i;
            while (a[j + 1] >= 2) j++;
            for (int k = j; k > i + 1; k--) {
                for (int o = i; o <= k; o++) a[o] -= 2;
                dfs(x);
                for (int o = i; o <= k; o++) a[o] += 2;
            }
        }
    }
    for (int i = 2; i <= 9; i++) {
        if (a[i]) {
            int j = i;
            while (a[j + 1]) j++;
            for (int k = j; k > i + 3; k--) {
                for (int o = i; o <= k; o++) a[o]--;
                dfs(x);
                for (int o = i; o <= k; o++) a[o]++;
            }
        }
    }
    int tmp = cal() + x - 1;
    if (!ans || tmp < ans) ans = tmp;
}
int main() {
    // freopen("in.in", "r", stdin);
    for (A = 0; A < 16; A++)
        for (B = 0; B < 16; B++)
            for (C = 0; C < 16; C++)
                for (D = 0; D < 16; D++) f[A][B][C][D] = -1;
    f[0][0][0][0] = 0;
    g1[0] = Card(-1, 0, 0, 0);
    g1[1] = Card(1, -1, 0, 0);
    g1[2] = Card(0, 1, -1, 0);
    g1[3] = Card(0, 0, 1, -1);
    g2[0] = Card(0, -1, 0, 0);
    g2[1] = Card(1, 0, -1, 0);
    g2[2] = Card(0, 1, 0, -1);
    g3[0] = Card(0, 0, -1, 0);
    g3[1] = Card(1, 0, 0, -1);
    g4 = Card(0, 0, 0, -1);
    for (read(T), read(n); T--; printf("%d\n", ans)) {
        for (ans = i = 0; i <= 13; i++) a[i] = 0;
        for (i = 0; i < n; i++) {
            read(x), read(y);
            if (x == 1)
                x = 13;
            else if (x)
                x--;
            a[x]++;
        }
        dfs(0);
        if (a[0] == 2) a[0] = 0, dfs(1);
    }
    return 0;
}

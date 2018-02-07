#include <cstdio>

inline int nextInt() {
    int r = 0;
    int c = getchar_unlocked();

    for (; c < 48; c = getchar_unlocked())
        ;
    for (; c > 47; c = getchar_unlocked()) r = r * 10 + c - 48;

    return r;
}

const int MAXN = 20;

typedef unsigned long long ull;

int n, ans, map[MAXN][MAXN];

inline void search(int x, ull a, ull b, ull c) {
    if (x == n)
        ++ans;
    else {
        ull d = a | b | c;

        for (register int y = 0; y < n; ++y)
            if (!map[x][y] && !((d >> y) & 1))
                search(x + 1, (a | (1ULL << y)), (b | (1ULL << y)) >> 1,
                       (c | (1ULL << y)) << 1);
    }
}

int main() {
    n = nextInt();

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) map[i][j] = nextInt();

    search(0, 0, 0, 0);

    printf("%d\n", ans);
    return 0;
}
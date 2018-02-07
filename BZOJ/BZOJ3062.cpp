#include <cstdio>
#include <algorithm>

const int MAXN = 100000;

int main() {
    static int n, m, s[MAXN + 1], t[MAXN + 1];
    scanf("%d %d", &n, &m);

    long long ans = 0;
    for (int i = 1; i <= n; i++) scanf("%d %d", &s[i], &t[i]), ans += abs(s[i] - t[i]);

    s[0] = m, t[0] = 0;
    std::sort(s, s + n + 1);
    std::sort(t, t + n + 1);

    for (int i = 0; i <= n; i++) ans += abs(s[i] - t[i]);

    printf("%lld\n", ans);

    return 0;
}

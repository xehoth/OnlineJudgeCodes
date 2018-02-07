#include <bits/stdc++.h>
#include <ext/rope>

__gnu_cxx::rope<int> d;
int f[100001], g[100001], p[100001], n;

int main() {
    scanf("%d", &n);
    for (register int i = 1, x; i <= n; i++) scanf("%d", &x), d.insert(x, i);
    register const int *a = d.c_str();
    memset(g, 0x3f, sizeof(int) * n);
    for (register int i = 1; i <= n; i++) p[a[i - 1]] = i;
    for (register int i = 0, k; i < n; i++)
        f[i + 1] = (k = std::lower_bound(g, g + n, a[i]) - g) + 1, g[k] = a[i];
    for (register int i = 1; i <= n; i++)
        printf("%d\n", (f[p[i]] = std::max(f[p[i]], f[p[i - 1]])));
    return 0;
}

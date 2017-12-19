#include <bits/stdc++.h>
int a[100010];
int main() {
    register int n = 100000, q = 100000;
    std::cout << n << '\n';
    for (register int i = 1; i <= n; i++) {
        a[i] = i;
    }
    std::random_shuffle(a + 1, a + n + 1);
    for (register int i = 1; i < n; i++) {
        std::cout << a[i] << ' ';
    }
    std::cout << a[n] << '\n';
    std::cout << q << '\n';
    q /= 4;
    register int l[2] = {1, n / 2}, r[2] = {n / 2, n};
    const int LIMIT = (INT_MAX / n - 1) / 2;
    for (register int i = 1; i <= q; i++) {
        if (l[0] > r[0]) std::swap(l[0], r[0]);
        std::cout << "3 " << (l[0]++) << ' ' << (r[0]--) << '\n';
        if (l[1] > r[1]) std::swap(l[1], r[1]);
        std::cout << "5 " << (l[1]++) << ' ' << (r[1]--) << ' '
                  << i * 1234ll % LIMIT << '\n';
        register int s = i + n / 2, t = n * 3 / 4 - i;
        if (s > t) std::swap(s, t);
        std::cout << "9 " << s << ' ' << t << ' '
                  << i * 123456ll % (INT_MAX / 2) << '\n';
    }
    for (register int i = 1; i <= q; i++) {
        std::cout << "9 " << i << ' ' << n - i + 1 << ' '
                  << i * 123456ll % (INT_MAX / 2) << '\n';
    }
}
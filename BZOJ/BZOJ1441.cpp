#include <bits/stdc++.h>
// 裴蜀定理
int main() {
    register int n, ans = 0;
    scanf("%d", &n);
    for (register int i = 1, x; i <= n; i++)
        scanf("%d", &x), ans = std::__gcd(ans, x);
    return printf("%d", std::abs(ans)), 0;
}
#include <bits/stdc++.h>
int main() {
    int n, k;
    std::cin >> n >> k, std::cout << (n == 1 ? 1 : std::min(k, n - k + 1) << 1);
    return 0;
}

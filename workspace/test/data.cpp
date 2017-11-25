#include <bits/stdc++.h>

const int MOD = 1000000000;

int main() {
    char *t = new char;
    srand(time(0) + (long long)t);
    delete t;
    register int n, m;
    n = rand() % MOD + 1, m = rand() % MOD + 1;
    while (std::__gcd(n, m) != 1) {
        n = rand() % MOD + 1, m = rand() % MOD + 1;
    }
    std::cout << n << ' ' << m << '\n';
    return 0;
}
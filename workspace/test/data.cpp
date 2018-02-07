#include <bits/stdc++.h>

int main() {
    int n = 500;
    std::cout << n << '\n';
    srand(time(0));
    for (int i = 1; i <= 4 * n * (n + 1); i++) {
        std::cout << rand() % 1000001 << '\n';
    }
}
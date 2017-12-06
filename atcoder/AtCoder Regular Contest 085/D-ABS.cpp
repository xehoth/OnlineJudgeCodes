/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 085D」ABS 06-12-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

int main() {
    register int n, z, w, a, b;
    std::cin >> n >> z >> w;
    if (n == 1) {
        std::cin >> a, std::cout << std::abs(a - w);
        return 0;
    }
    for (register int i = 1, t; i <= n - 2; i++) std::cin >> t;
    std::cin >> a >> b;
    std::cout << std::max(std::abs(a - b), std::abs(b - w));
    return 0;
}
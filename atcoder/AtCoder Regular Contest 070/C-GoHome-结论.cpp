/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 070C」Go Home 18-08-2017
 * 结论题
 * @author xehoth
 */
#include <bits/stdc++.h>

int main() {
    register int n = 1, x;
    std::cin >> x;
    while (n * (n + 1) / 2 < x) n++;
    std::cout << n;
    return 0;
}
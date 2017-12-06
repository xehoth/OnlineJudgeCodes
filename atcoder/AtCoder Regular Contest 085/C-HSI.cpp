/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 085C」HSI 06-12-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

int main() {
    register int n, m;
    std::cin >> n >> m;
    std::cout << (1900 * m + 100 * (n - m)) * (1 << m);
    return 0;
}

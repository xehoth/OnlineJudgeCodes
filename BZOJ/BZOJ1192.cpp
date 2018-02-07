/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1192」12-03-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

int main() {
    long long ans;
    scanf("%lld", &ans);
    std::cout << std::floor(log2(ans) + 1);
    return 0;
}
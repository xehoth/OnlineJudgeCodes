/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1072」26-03-2017
 *
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
char s[10];
int len;
int d;
int T;
long long toint(char *s) {
    long long res = 0;
    for (int i = 0; i < len; i++) res *= 10, res += s[i] - '0';
    return res;
}
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%s", s);
        len = strlen(s);
        scanf("%d", &d);
        sort(s, s + len);
        int ans = 0;
        do {
            if (toint(s) % d == 0) ans++;
        } while (next_permutation(s, s + len));
        printf("%d\n", ans);
    }
    return 0;
}

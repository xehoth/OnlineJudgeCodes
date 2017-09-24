/**
 * Copyright (c) 2016, xehoth
 * All rights reserved.
 * 「BZOJ 1296」文本编辑器 15-08-2016
 * rope
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <ext/rope>
#include <iostream>
using namespace std;
using namespace __gnu_cxx;
crope a, b, tmp;
char s[10];
int now, n, len, size;
char str[2000000], rstr[2000000];
int main() {
    scanf("%d", &n);
    while (n--) {
        scanf("%s", s);
        switch (s[0]) {
            case 'M': {
                scanf("%d", &now);
                break;
            }
            case 'P': {
                now--;
                break;
            }
            case 'N': {
                now++;
                break;
            }
            case 'G': {
                putchar(a[now]);
                putchar('\n');
                break;
            }
            case 'I': {
                scanf("%d", &size);
                len = a.length();
                for (int i = 0; i < size; i++) {
                    do {
                        str[i] = getchar();
                    } while (str[i] == '\n');
                    rstr[size - i - 1] = str[i];
                }
                rstr[size] = str[size] = '\0';
                a.insert(now, str);
                b.insert(len - now, rstr);
                break;
            }
            case 'D': {
                scanf("%d", &size);
                len = a.length();
                a.erase(now, size);
                b.erase(len - now - size, size);
                break;
            }
            case 'R': {
                scanf("%d", &size);
                len = a.length();
                tmp = a.substr(now, size);
                a = a.substr(0, now) + b.substr(len - now - size, size) +
                    a.substr(now + size, len - now - size);
                b = b.substr(0, len - now - size) + tmp +
                    b.substr(len - now, now);
                break;
            }
        }
    }
    return 0;
}
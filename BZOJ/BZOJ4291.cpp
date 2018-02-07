/*
 * created by xehoth on 02-03-2017
 */
#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define long long long

char buf[10000050];
char *p = buf;

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = *p++, iosig = false; !isdigit(c); c = *p++)
        if (c == '-') iosig = true;
    for (x = 0; isdigit(c); c = *p++) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
}

int main() {
    fread(buf, 1, 10000050, stdin);
    static int a[1000005];
    register int n;
    read(n);
    if (n == 1) {
        read(a[1]);
        if (a[1] & 1)
            std::cout << "NIESTETY";
        else
            std::cout << a[1];
    } else {
        register long sum = 0;
        for (register int i = 1; i <= n; i++) read(a[i]), sum += a[i];
        if (sum & 1) {
            register int min = INT_MAX;
            for (register int i = 1; i <= n; i++)
                if ((a[i] & 1) && a[i] < min) min = a[i];
            std::cout << sum - min;
        } else {
            std::cout << sum;
        }
    }
    return 0;
}

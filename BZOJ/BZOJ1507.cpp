#include <bits/stdc++.h>
#include <ext/rope>
inline int read() {
    register int x = 0, f = 1;
    char ch = getchar();
    while (!isdigit(ch)) {
        if (ch == '-') f = -1;
        ch = getchar();
    }
    while (isdigit(ch)) {
        x = (x << 1) + (x << 3) + (ch ^ '0');
        ch = getchar();
    }
    return x * f;
}
int main() {
    __gnu_cxx::crope rope;
    int t, now;
    static char ch[3000005];
    t = read();
    char s[10];
    int x;
    while (t--) {
        scanf("%s", s);
        switch (s[0]) {
            case 'M':
                now = read();
                break;
            case 'P':
                now--;
                break;
            case 'N':
                now++;
                break;
            case 'I':
                x = read();
                for (int i = 0; i < x; i++) {
                    ch[i] = getchar();
                    while (ch[i] == '\n') ch[i] = getchar();
                }
                ch[x] = 0;
                rope.insert(now, ch);
                break;
            case 'D':
                x = read();
                rope.erase(now, x);
                break;
            case 'G':
                x = read();
                rope.copy(now, x, ch);
                ch[x] = 0;
                puts(ch);
        }
    }
    return 0;
}
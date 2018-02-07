#include <stdio.h>
char c;
inline void read() {
    // setvbuf(stdin, NULL, _IOFBF, 0);
    fread(&c, 1, 1, stdin);
}

inline void read(int &x) {
    for (read(), x = 0; c >= '0' && c <= '9'; read())
        x = (x + (x << 2) << 1) + (c ^ '0');
}

inline void print(char c) {
    // setvbuf(stdout, NULL, _IOFBF, 0);
    fwrite(&c, 1, 1, stdout);
}

inline void print(int x) {
    static char buf[11], *c;
    if (x == 0) {
        print('0');
    } else {
        for (c = buf; x; x /= 10) *++c = x % 10 | 48;
        while (c != buf) print(*c--);
    }
}

int main() {
    //	freopen("in.in", "r", stdin);
    int x;
    read(x), read(x);
    print(x);
    return 0;
}
/*
 * created by xehoth on 17-04-2017
 */
#include <bits/stdc++.h>

inline char read() {
    static const int IN_LEN = 10000000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        if (c == '-') iosig = true;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

bool d[1000001];
unsigned char val[8][262144 + 1];
int lbound[256 + 1], rbound[256 + 1];
int lc[8], rc[8];

inline void insert(int x) {
    if (d[x]) return;
    d[x] = 1;
    val[1][x >> 3] |= 1 << (x & 7), x >>= 3;
    val[2][x >> 3] |= 1 << (x & 7), x >>= 3;
    val[3][x >> 3] |= 1 << (x & 7), x >>= 3;
    val[4][x >> 3] |= 1 << (x & 7), x >>= 3;
    val[5][x >> 3] |= 1 << (x & 7), x >>= 3;
    val[6][x >> 3] |= 1 << (x & 7), x >>= 3;
    val[7][x >> 3] |= 1 << (x & 7);
}

inline void erase(int x) {
    if (d[x])
        d[x] = 0;
    else
        return;
    if (val[1][x >> 3] ^= 1 << (x & 7)) return;
    x >>= 3;
    if (val[2][x >> 3] ^= 1 << (x & 7)) return;
    x >>= 3;
    if (val[3][x >> 3] ^= 1 << (x & 7)) return;
    x >>= 3;
    if (val[4][x >> 3] ^= 1 << (x & 7)) return;
    x >>= 3;
    if (val[5][x >> 3] ^= 1 << (x & 7)) return;
    x >>= 3;
    if (val[6][x >> 3] ^= 1 << (x & 7)) return;
    x >>= 3;
    if (val[7][x >> 3] ^= 1 << (x & 7)) return;
}

inline int getMin() {
    if (!val[7][0]) return -1;
    register int p = lbound[val[7][0]];
    p = (p << 3) + lbound[val[6][p]], p = (p << 3) + lbound[val[5][p]],
    p = (p << 3) + lbound[val[4][p]], p = (p << 3) + lbound[val[3][p]],
    p = (p << 3) + lbound[val[2][p]];
    return p = (p << 3) + lbound[val[1][p]];
}

inline int getMax() {
    if (!val[7][0]) return -1;
    register int p = rbound[val[7][0]];
    p = (p << 3) + rbound[val[6][p]], p = (p << 3) + rbound[val[5][p]],
    p = (p << 3) + rbound[val[4][p]], p = (p << 3) + rbound[val[3][p]],
    p = (p << 3) + rbound[val[2][p]];
    return p = (p << 3) + rbound[val[1][p]];
}

inline int precursor(int p) {
    if (!val[7][0]) return -1;
    register int s = val[1][p >> 3] & lc[p & 7];
    if (s) return (p ^ (p & 7)) | rbound[s];
    for (register int i = 2; i <= 7; i++) {
        p >>= 3;
        s = val[i][p >> 3] & lc[p & 7];
        if (s) {
            p = (p ^ (p & 7)) | rbound[s];
            for (register int j = i - 1; j; j--)
                p = (p << 3) | rbound[val[j][p]];
            return p;
        }
    }
    return -1;
}

inline int successor(int p) {
    if (!val[7][0]) return -1;
    register int s = val[1][p >> 3] & rc[p & 7];
    if (s) return (p ^ (p & 7)) | lbound[s];
    for (register int i = 2; i <= 7; i++) {
        p >>= 3;
        s = val[i][p >> 3] & rc[p & 7];
        if (s) {
            p = (p ^ (p & 7)) | lbound[s];
            for (register int j = i - 1; j; j--)
                p = (p << 3) | lbound[val[j][p]];
            return p;
        }
    }
    return -1;
}

int n, m, a, b;

int main() {
    // freopen("in.in", "r", stdin);
    for (register int i = 1; i < 256; i++) {
        register int j = 0;
        while (!(i & 1 << j)) j++;
        lbound[i] = j;
        j = 7;
        while (!(i & 1 << j)) j--;
        rbound[i] = j;
    }
    for (register int i = 0; i < 8; i++)
        lc[i] = 255 >> 8 - i, rc[i] = 255 & (255 << i + 1);
    read(n), read(m);
    for (int i = 0; i < m; i++) {
        read(a);
        if (a < 3) {
            read(b);
            if (a == 1)
                insert(b);
            else if (a == 2)
                erase(b);
        } else if (a > 4) {
            read(b);
            if (a == 5)
                print(precursor(b)), print('\n');
            else if (a == 6)
                print(successor(b)), print('\n');
            else if (a == 7)
                print(d[b] && val[1][b >> 3] & 1 << (b & 7) ? 1 : -1),
                    print('\n');
        } else if (a == 3) {
            print(getMin()), print('\n');
        } else if (a == 4) {
            print(getMax()), print('\n');
        }
    }
    flush();
    return 0;
}

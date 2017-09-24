/**
 * Copyright (c) 2016, xehoth
 * All rights reserved.
 * 「BZOJ 1798」维护序列 31-08-2016
 * 线段树
 * @author xehoth
 */
#include <cctype>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
inline int getc() {
    static const int L = 1 << 21;
    static char buf[L], *S = buf, *T = buf;
    if (S == T) {
        T = (S = buf) + fread(buf, 1, L, stdin);
        if (S == T) return EOF;
    }
    return *S++;
}
inline int getint() {
    register int c;
    while (!isdigit(c = getc()))
        ;
    register int tmp = c ^ '0';
    while (isdigit(c = getc())) tmp = (tmp << 3) + (tmp << 1) + (c ^ '0');
    return tmp;
}
#define XEHOTH_IO_SIZE (1 << 21 | 1)
char IO_wbuf[XEHOTH_IO_SIZE], *IO_S1 = IO_wbuf;
inline void fwriteChar(char c) {
    if (IO_S1 == IO_wbuf + XEHOTH_IO_SIZE) {
        fwrite(IO_wbuf, 1, XEHOTH_IO_SIZE, stdout);
        IO_S1 = IO_wbuf;
    }
    *IO_S1++ = c;
}
inline void fwriteCharArray(const char *str, int len) {
    for (register int i = 0; i < len; i++) fwriteChar(str[i]);
}
#define MIN_VALUE 0x80000000
const int sizeTable[] = {9,      99,      999,      9999,      99999,
                         999999, 9999999, 99999999, 999999999, 0x7fffffff};
inline int stringSize(int x) {
    for (int i = 0;; i++)
        if (x <= sizeTable[i]) return i + 1;
}
const char DigitOnes[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4',
    '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4',
    '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4',
    '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
};
const char DigitTens[] = {
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '1', '1', '1',
    '1', '1', '1', '1', '1', '2', '2', '2', '2', '2', '2', '2', '2', '2', '2',
    '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '4', '4', '4', '4', '4',
    '4', '4', '4', '4', '4', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5',
    '6', '6', '6', '6', '6', '6', '6', '6', '6', '6', '7', '7', '7', '7', '7',
    '7', '7', '7', '7', '7', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8',
    '9', '9', '9', '9', '9', '9', '9', '9', '9', '9',
};
const char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
                       '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                       'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                       'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
inline void getChars(int i, int index, char buf[]) {
    int q, r;
    int charPos = index;
    char sign = 0;
    if (i < 0) {
        sign = '-';
        i = -i;
    }
    while (i >= 65536) {
        q = i / 100;
        r = i - ((q << 6) + (q << 5) + (q << 2));
        i = q;
        buf[--charPos] = DigitOnes[r];
        buf[--charPos] = DigitTens[r];
    }
    for (;;) {
        q = ((unsigned int)(i * 52429)) >> (16 + 3);
        r = i - ((q << 3) + (q << 1));
        buf[--charPos] = digits[r];
        i = q;
        if (i == 0) break;
    }
    if (sign != 0) {
        buf[--charPos] = sign;
    }
}
inline void fwriteInt(int i) {
    if (i == MIN_VALUE) fwriteCharArray("-2147483648", 11);
    int size = (i < 0) ? stringSize(-i) + 1 : stringSize(i);
    char buf[size];
    getChars(i, size, buf);
    fwriteCharArray(buf, size);
}
inline void flushIO() { fwrite(IO_wbuf, 1, IO_S1 - IO_wbuf, stdout); }
typedef long long LL;
int mod;
struct Mark {
    int u, v;
    Mark(int _u = -1, int _v = -1) : u(_u), v(_v) {}
    inline bool operator==(const Mark &b) const { return u == b.u && v == b.v; }
    inline bool operator!=(const Mark &b) const { return u != b.u || v != b.v; }
    inline void operator+=(const Mark &b) {
        if (u == -1 && v == -1)
            *this = b;
        else {
            u = (LL)u * b.u % mod;
            v = ((LL)v * b.u + b.v) % mod;
        }
    }
} null(-1, -1);
inline void inc(int &a, int b, int c) {
    a = ((LL)b + c >= mod) ? b - mod + c : b + c;
}
#define N ((131072 << 1) + 10)
int sav[100010];
struct SegmentTree {
    int M, sum[N], size[N];
    Mark c[N];
    inline void build(int _size) {
        for (M = 1; M < (_size + 2); M <<= 1)
            ;
        register int i;
        for (i = 0; i < M; ++i) sum[M + i] = sav[i], size[M + i] = 1;
        for (i = M - 1; i >= 1; --i) {
            inc(sum[i], sum[i << 1], sum[(i << 1) | 1]);
            size[i] = size[i << 1] + size[(i << 1) | 1];
        }
    }
    int stack[25], top;
    inline void make_Mark(int x, Mark _c) {
        c[x] += _c;
        sum[x] = ((LL)sum[x] * _c.u + (LL)_c.v * size[x]) % mod;
    }
    inline void pushdown(int x) {
        if (c[x] != null && x < M) {
            if (x << 1) make_Mark(x << 1, c[x]);
            if ((x << 1) | 1) make_Mark((x << 1) | 1, c[x]);
            c[x] = null;
        }
    }
    inline void update_path(int x) {
        top = 0;
        for (; x; x >>= 1) stack[++top] = x;
        while (top--) pushdown(stack[top]);
    }
    inline int query(int tl, int tr) {
        register int insl = 0, insr = 0, res = 0;
        for (tl = tl + M - 1, tr = tr + M + 1; tl ^ tr ^ 1;
             tl >>= 1, tr >>= 1) {
            if (~tl & 1) {
                if (!insl) update_path(insl = tl ^ 1);
                inc(res, res, sum[tl ^ 1]);
            }
            if (tr & 1) {
                if (!insr) update_path(insr = tr ^ 1);
                inc(res, res, sum[tr ^ 1]);
            }
        }
        return res;
    }
    inline void modify(int tl, int tr, Mark _c) {
        register int insl = 0, insr = 0;
        for (tl = tl + M - 1, tr = tr + M + 1; tl ^ tr ^ 1;
             tl >>= 1, tr >>= 1) {
            if (~tl & 1) {
                if (!insl) update_path(insl = tl ^ 1);
                make_Mark(tl ^ 1, _c);
            }
            if (tr & 1) {
                if (!insr) update_path(insr = tr ^ 1);
                make_Mark(tr ^ 1, _c);
            }
        }
        for (insl = insl >> 1; insl; insl >>= 1)
            inc(sum[insl], sum[insl << 1], sum[(insl << 1) | 1]);
        for (insr = insr >> 1; insr; insr >>= 1)
            inc(sum[insr], sum[insr << 1], sum[(insr << 1) | 1]);
    }
} ZKW;
int n;
int main() {
    n = getint();
    mod = getint();
    register int i;
    for (i = 1; i <= n; ++i) sav[i] = getint(), sav[i] %= mod;
    ZKW.build(n);
    register int ask = getint();
    register int sign, a, b, x;
    while (ask--) {
        sign = getint();
        a = getint();
        b = getint();
        if (sign == 1) {
            x = getint();
            ZKW.modify(a, b, Mark(x % mod, 0));
        } else if (sign == 2) {
            x = getint();
            ZKW.modify(a, b, Mark(1, x % mod));
        } else
            fwriteInt(ZKW.query(a, b)), fwriteChar('\n');
    }
    flushIO();
    return 0;
}
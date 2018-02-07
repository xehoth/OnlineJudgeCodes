#include <bits/stdc++.h>

namespace {

const int OUT_LEN = 100000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

char buf[1 << 15], *S = buf, *T = buf;
inline char getc() {
    if (S == T)
        if (T = (S = buf) + fread(buf, 1, 1 << 15, stdin), S == T) return 0;
    return *S++;
}

char t[1 << 18], ch, op, lk, rk;
int d = 1 << 17, i, j, l, r, x, y, v, flag;
#define Xor(a) (a = 1 - a)

void update(int o, int l, int r) {
    if (x <= l && r <= y) {
        v < 2 ? t[o] = v : Xor(t[o]);
        return;
    }
    int mid = l + r >> 1;
    t[o] == 0 || t[o] == 1 ? t[o << 1] = t[o << 1 | 1] = t[o],
                                    t[o] = -1 : t[o] == 2 ? Xor(t[o << 1]),
                                    Xor(t[o << 1 | 1]), t[o] = -1 : 1;
    if (x <= mid) update(o << 1, l, mid);
    if (mid < y) update(o << 1 | 1, mid + 1, r);
}

inline void set(int xx, int l, int r) {
    if (l > r) return;
    x = l, y = r, v = xx, update(1, 0, d - 1);
}

inline void getCmd() {
    while (op = getc(),
           op != 'U' && op != 'D' && op != 'S' && op != 'C' && op != 'I')
        if (op == 0) return;
}

inline void solve() {
    while (getCmd(), op != 0) {
        getc(), lk = getc(), l = r = 0;
        while (ch = getc(), ch != ',') l = l * 10 + ch - 48;
        while (ch = getc(), ch > 47 && ch < 58) r = r * 10 + ch - 48;
        rk = ch, l <<= 1, r <<= 1, l += (lk == '('), r -= (rk == ')');
        if (op == 'U') set(1, l, r);
        if (op == 'I') set(0, 0, l - 1), set(0, r + 1, d - 1);
        if (op == 'D') set(0, l, r);
        if (op == 'C') set(0, 0, l - 1), set(0, r + 1, d - 1), set(2, l, r);
        if (op == 'S') set(2, l, r);
    }
    for (i = 1; i < d; i++)
        t[i] == 0 || t[i] == 1 ? t[i << 1] = t[i << 1 | 1] = t[i],
                                        t[i] = -1 : t[i] == 2 ? Xor(t[i << 1]),
                                        Xor(t[i << 1 | 1]), t[i] = -1 : 1;
    for (i = 0; i < d; i++)
        if (t[i + d] == 1) {
            flag = 1;
            for (j = i; t[j + d] == 1 && j < d; j++)
                ;
            i & 1 ? (print('('), print(i >> 1), print(','))
                  : (print('['), print(i >> 1), print(',')),
                j & 1 ? (print(j >> 1), print(']'), print(' '))
                      : (print(j >> 1), print(')'), print(' ')),
                i = j - 1;
        }
    if (!flag)
        print("empty set\n");
    else
        print('\n');
    flush();
}
}  // namespace

int main() {
    solve();
    return 0;
}
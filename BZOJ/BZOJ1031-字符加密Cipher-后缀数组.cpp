/**
 * Copyright (c) 2016, xehoth
 * All rights reserved.
 * 「BZOJ 1031」字符加密Cipher 27-12-2016
 * 后缀数组
 * @author xehoth
 */
#include <bits/stdc++.h>
#define FAST_IO
#ifdef FAST_IO
const int IN_LEN = 1000000, OUT_LEN = 1000000;
inline int nextChar() {
    static char buf[IN_LEN], *h, *t;
    if (h == t) {
        t = (h = buf) + fread(buf, 1, IN_LEN, stdin);
        if (h == t) return -1;
    }
    return *h++;
}
inline bool isWordSkippable(const char c) {
    return c == ' ' || c == '\n' || c == '\r';
}
inline bool read(char *str) {
    char c;
    for (c = nextChar(); isWordSkippable(c); c = nextChar())
        if (c == -1) return false;
    for (; !isWordSkippable(c); c = nextChar()) *str++ = c;
    return true;
}
char obuf[OUT_LEN], *oh = obuf;
inline void writeChar(const char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}
inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
#endif
template <size_t size>
struct SuffixArray {
    bool t[size << 1];
    int b[size], b1[size];
    int sa[size], rk[size], ht[size];
    inline bool islms(const int i, const bool *t) {
        return i > 0 && t[i] && !t[i - 1];
    }
    template <class T>
    inline void sort(T s, int *sa, const int len, const int sigma, const int sz,
                     bool *t, int *b, int *cb, int *p) {
        memset(b, 0, sizeof(int) * sigma);
        memset(sa, -1, sizeof(int) * len);
        for (register int i = 0; i < len; i++) b[s[i]]++;
        cb[0] = b[0];
        for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
        for (register int i = sz - 1; i >= 0; i--) sa[--cb[s[p[i]]]] = p[i];
        for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i - 1];
        for (register int i = 0; i < len; i++)
            if (sa[i] > 0 && !t[sa[i] - 1]) sa[cb[s[sa[i] - 1]]++] = sa[i] - 1;
        cb[0] = b[0];
        for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
        for (register int i = len - 1; i >= 0; i--)
            if (sa[i] > 0 && t[sa[i] - 1]) sa[--cb[s[sa[i] - 1]]] = sa[i] - 1;
    }
    template <class T>
    inline void sais(T s, int *sa, int len, bool *t, int *b, int *b1,
                     int sigma) {
        int i, j, sz = 0, cnt = 0, p = -1, x, *cb = b + sigma;
        t[len - 1] = 1;
        for (i = len - 2; i >= 0; i--)
            t[i] = s[i] < s[i + 1] || (s[i] == s[i + 1] && t[i + 1]);
        for (i = 1; i < len; i++)
            if (t[i] && !t[i - 1]) b1[sz++] = i;
        sort(s, sa, len, sigma, sz, t, b, cb, b1);
        for (i = sz = 0; i < len; i++)
            if (islms(sa[i], t)) sa[sz++] = sa[i];
        for (i = sz; i < len; i++) sa[i] = -1;
        for (i = 0; i < sz; i++) {
            x = sa[i];
            for (j = 0; j < len; j++) {
                if (p == -1 || s[x + j] != s[p + j] || t[x + j] != t[p + j]) {
                    cnt++, p = x;
                    break;
                } else if (j > 0 && (islms(x + j, t) || islms(p + j, t)))
                    break;
            }
            x = (~x & 1 ? x >> 1 : x - 1 >> 1), sa[sz + x] = cnt - 1;
        }
        for (i = j = len - 1; i >= sz; i--)
            if (sa[i] >= 0) sa[j--] = sa[i];
        int *s1 = sa + len - sz, *b2 = b1 + sz;
        if (cnt < sz)
            sais(s1, sa, sz, t + len, b, b1 + sz, cnt);
        else
            for (i = 0; i < sz; i++) sa[s1[i]] = i;
        for (i = 0; i < sz; i++) b2[i] = b1[sa[i]];
        sort(s, sa, len, sigma, sz, t, b, cb, b2);
    }
    inline void getHeight(const char *s, const int len, const int *sa) {
        for (int i = 0, k = 0; i < len; i++) {
            if (rk[i] == 0)
                k = 0;
            else {
                if (k > 0) k--;
                int j = sa[rk[i] - 1];
                while (i + k < len && j + k < len && s[i + k] == s[j + k]) k++;
            }
            ht[rk[i]] = k;
        }
    }
    template <class T>
    inline void init(T s, const int len, const int sigma) {
        sais(s, sa, len, t, b, b1, sigma);
        for (register int i = 1; i < len; i++) rk[sa[i]] = i;
        getHeight(s, len, sa);
    }
};
const int MAXN = 200000 + 10;
char s[MAXN];
SuffixArray<MAXN> sf;
int main() {
    read(s);
    register int len = strlen(s);
    memcpy(s + len, s, sizeof(char) * len);
    sf.init(s, len << 1 | 1, 123);
    len <<= 1;
    for (register int i = 1; i <= len; i++)
        if (sf.sa[i]<len>> 1) writeChar(s[sf.sa[i] + (len >> 1) - 1]);
    flush();
    return 0;
}

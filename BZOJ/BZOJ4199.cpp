#include <bits/stdc++.h>
#define FAST_IO
#ifdef FAST_IO
const int IN_LEN = 100010, OUT_LEN = 1000000;
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
template <class T>
inline bool read(T &x) {
    static bool iosig = 0;
    static char c;
    for (iosig = 0, c = nextChar(); !isdigit(c); c = nextChar()) {
        if (c == -1) return false;
        if (c == '-') iosig = 1;
    }
    for (x = 0; isdigit(c); c = nextChar()) x = (x << 1) + (x << 3) + (c ^ '0');
    if (iosig) x = -x;
    return true;
}
char obuf[OUT_LEN], *oh = obuf;
inline void writeChar(const char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}
template <class T>
inline void write(T x) {
    static int buf[30], cnt;
    if (!x)
        writeChar(48);
    else {
        if (x < 0) writeChar('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) writeChar(buf[cnt--]);
    }
}
inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
#endif
#include <algorithm>
#include <cstdio>
#include <cstring>
const int UCHAR_SIZE = 256;
const int MINBUCKETSIZE = 256;
typedef int sais_index_type;
typedef int sais_bool_type;
const int SAIS_LMSSORT2_LIMIT = 0x3fffffff;
void getCounts(const void *T, sais_index_type *C, sais_index_type n,
               sais_index_type k, int cs) {
    sais_index_type i;
    for (i = 0; i < k; ++i) C[i] = 0;
    for (i = 0; i < n; ++i)
        ++C[(cs == sizeof(sais_index_type) ? ((sais_index_type *)T)[(i)]
                                           : ((unsigned char *)T)[(i)])];
}
void getBuckets(const sais_index_type *C, sais_index_type *B, sais_index_type k,
                sais_bool_type end) {
    sais_index_type i, sum = 0;
    if (end)
        for (i = 0; i < k; ++i) {
            sum += C[i];
            B[i] = sum;
        }
    else
        for (i = 0; i < k; ++i) {
            sum += C[i];
            B[i] = sum - C[i];
        }
}
void LMSsort1(const void *T, sais_index_type *SA, sais_index_type *C,
              sais_index_type *B, sais_index_type n, sais_index_type k,
              int cs) {
    sais_index_type *b, i, j;
    sais_index_type c0, c1;
    if (C == B) getCounts(T, C, n, k, cs);
    getBuckets(C, B, k, 0);
    j = n - 1;
    b = SA +
        B[c1 = (cs == sizeof(sais_index_type) ? ((sais_index_type *)T)[(j)]
                                              : ((unsigned char *)T)[(j)])];
    --j;
    *b++ = ((cs == sizeof(sais_index_type) ? ((sais_index_type *)T)[(j)]
                                           : ((unsigned char *)T)[(j)]) < c1)
               ? ~j
               : j;
    for (i = 0; i < n; ++i) {
        if (0 < (j = SA[i])) {
            if ((c0 = (cs == sizeof(sais_index_type)
                           ? ((sais_index_type *)T)[(j)]
                           : ((unsigned char *)T)[(j)])) != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            --j;
            *b++ = ((cs == sizeof(sais_index_type)
                         ? ((sais_index_type *)T)[(j)]
                         : ((unsigned char *)T)[(j)]) < c1)
                       ? ~j
                       : j;
            SA[i] = 0;
        } else if (j < 0)
            SA[i] = ~j;
    }
    if (C == B) getCounts(T, C, n, k, cs);
    getBuckets(C, B, k, 1);
    for (i = n - 1, b = SA + B[c1 = 0]; 0 <= i; --i) {
        if (0 < (j = SA[i])) {
            if ((c0 = (cs == sizeof(sais_index_type)
                           ? ((sais_index_type *)T)[(j)]
                           : ((unsigned char *)T)[(j)])) != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            --j;
            *--b = ((cs == sizeof(sais_index_type)
                         ? ((sais_index_type *)T)[(j)]
                         : ((unsigned char *)T)[(j)]) > c1)
                       ? ~(j + 1)
                       : j;
            SA[i] = 0;
        }
    }
}
sais_index_type LMSpostproc1(const void *T, sais_index_type *SA,
                             sais_index_type n, sais_index_type m, int cs) {
    sais_index_type i, j, p, q, plen, qlen, name;
    sais_index_type c0, c1;
    sais_bool_type diff;
    for (i = 0; (p = SA[i]) < 0; ++i) SA[i] = ~p;
    if (i < m) {
        for (j = i, ++i;; ++i) {
            if ((p = SA[i]) < 0) {
                SA[j++] = ~p;
                SA[i] = 0;
                if (j == m) break;
            }
        }
    }
    i = n - 1;
    j = n - 1;
    c0 = (cs == sizeof(sais_index_type) ? ((sais_index_type *)T)[(n - 1)]
                                        : ((unsigned char *)T)[(n - 1)]);
    do {
        c1 = c0;
    } while ((0 <= --i) && ((c0 = (cs == sizeof(sais_index_type)
                                       ? ((sais_index_type *)T)[(i)]
                                       : ((unsigned char *)T)[(i)])) >= c1));
    for (; 0 <= i;) {
        do {
            c1 = c0;
        } while ((0 <= --i) &&
                 ((c0 = (cs == sizeof(sais_index_type)
                             ? ((sais_index_type *)T)[(i)]
                             : ((unsigned char *)T)[(i)])) <= c1));
        if (0 <= i) {
            SA[m + ((i + 1) >> 1)] = j - i;
            j = i + 1;
            do {
                c1 = c0;
            } while ((0 <= --i) &&
                     ((c0 = (cs == sizeof(sais_index_type)
                                 ? ((sais_index_type *)T)[(i)]
                                 : ((unsigned char *)T)[(i)])) >= c1));
        }
    }
    for (i = 0, name = 0, q = n, qlen = 0; i < m; ++i) {
        p = SA[i], plen = SA[m + (p >> 1)], diff = 1;
        if ((plen == qlen) && ((q + plen) < n)) {
            for (j = 0; (j < plen) && ((cs == sizeof(sais_index_type)
                                            ? ((sais_index_type *)T)[(p + j)]
                                            : ((unsigned char *)T)[(p + j)]) ==
                                       (cs == sizeof(sais_index_type)
                                            ? ((sais_index_type *)T)[(q + j)]
                                            : ((unsigned char *)T)[(q + j)]));
                 ++j)
                ;
            if (j == plen) diff = 0;
        }
        if (diff != 0) ++name, q = p, qlen = plen;
        SA[m + (p >> 1)] = name;
    }
    return name;
}
void LMSsort2(const void *T, sais_index_type *SA, sais_index_type *C,
              sais_index_type *B, sais_index_type *D, sais_index_type n,
              sais_index_type k, int cs) {
    sais_index_type *b, i, j, t, d;
    sais_index_type c0, c1;
    getBuckets(C, B, k, 0);
    j = n - 1;
    b = SA +
        B[c1 = (cs == sizeof(sais_index_type) ? ((sais_index_type *)T)[(j)]
                                              : ((unsigned char *)T)[(j)])];
    --j;
    t = ((cs == sizeof(sais_index_type) ? ((sais_index_type *)T)[(j)]
                                        : ((unsigned char *)T)[(j)]) < c1);
    j += n;
    *b++ = (t & 1) ? ~j : j;
    for (i = 0, d = 0; i < n; ++i) {
        if (0 < (j = SA[i])) {
            if (n <= j) d += 1, j -= n;
            if ((c0 = (cs == sizeof(sais_index_type)
                           ? ((sais_index_type *)T)[(j)]
                           : ((unsigned char *)T)[(j)])) != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            --j;
            t = c0;
            t = (t << 1) | ((cs == sizeof(sais_index_type)
                                 ? ((sais_index_type *)T)[(j)]
                                 : ((unsigned char *)T)[(j)]) < c1);
            if (D[t] != d) j += n, D[t] = d;
            *b++ = (t & 1) ? ~j : j;
            SA[i] = 0;
        } else if (j < 0)
            SA[i] = ~j;
    }
    for (i = n - 1; 0 <= i; --i) {
        if (0 < SA[i]) {
            if (SA[i] < n) {
                SA[i] += n;
                for (j = i - 1; SA[j] < n; --j)
                    ;
                SA[j] -= n;
                i = j;
            }
        }
    }
    getBuckets(C, B, k, 1);
    for (i = n - 1, d += 1, b = SA + B[c1 = 0]; 0 <= i; --i) {
        if (0 < (j = SA[i])) {
            if (n <= j) d += 1, j -= n;
            if ((c0 = (cs == sizeof(sais_index_type)
                           ? ((sais_index_type *)T)[(j)]
                           : ((unsigned char *)T)[(j)])) != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            --j;
            t = c0;
            t = (t << 1) | ((cs == sizeof(sais_index_type)
                                 ? ((sais_index_type *)T)[(j)]
                                 : ((unsigned char *)T)[(j)]) > c1);
            if (D[t] != d) j += n, D[t] = d;
            *--b = (t & 1) ? ~(j + 1) : j;
            SA[i] = 0;
        }
    }
}
sais_index_type LMSpostproc2(sais_index_type *SA, sais_index_type n,
                             sais_index_type m) {
    sais_index_type i, j, d, name;
    for (i = 0, name = 0; (j = SA[i]) < 0; ++i) {
        j = ~j;
        if (n <= j) name += 1;
        SA[i] = j;
    }
    if (i < m) {
        for (d = i, ++i;; ++i) {
            if ((j = SA[i]) < 0) {
                j = ~j;
                if (n <= j) name += 1;
                SA[d++] = j;
                SA[i] = 0;
                if (d == m) break;
            }
        }
    }
    if (name < m) {
        for (i = m - 1, d = name + 1; 0 <= i; --i) {
            if (n <= (j = SA[i])) j -= n, --d;
            SA[m + (j >> 1)] = d;
        }
    } else {
        for (i = 0; i < m; ++i) {
            if (n <= (j = SA[i])) j -= n, SA[i] = j;
        }
    }
    return name;
}
void induceSA(const void *T, sais_index_type *SA, sais_index_type *C,
              sais_index_type *B, sais_index_type n, sais_index_type k,
              int cs) {
    sais_index_type *b, i, j;
    sais_index_type c0, c1;
    if (C == B) getCounts(T, C, n, k, cs);
    getBuckets(C, B, k, 0);
    j = n - 1;
    b = SA +
        B[c1 = (cs == sizeof(sais_index_type) ? ((sais_index_type *)T)[(j)]
                                              : ((unsigned char *)T)[(j)])];
    *b++ = ((0 < j) && ((cs == sizeof(sais_index_type)
                             ? ((sais_index_type *)T)[(j - 1)]
                             : ((unsigned char *)T)[(j - 1)]) < c1))
               ? ~j
               : j;
    for (i = 0; i < n; ++i) {
        j = SA[i], SA[i] = ~j;
        if (0 < j) {
            --j;
            if ((c0 = (cs == sizeof(sais_index_type)
                           ? ((sais_index_type *)T)[(j)]
                           : ((unsigned char *)T)[(j)])) != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            *b++ = ((0 < j) && ((cs == sizeof(sais_index_type)
                                     ? ((sais_index_type *)T)[(j - 1)]
                                     : ((unsigned char *)T)[(j - 1)]) < c1))
                       ? ~j
                       : j;
        }
    }
    if (C == B) getCounts(T, C, n, k, cs);
    getBuckets(C, B, k, 1);
    for (i = n - 1, b = SA + B[c1 = 0]; 0 <= i; --i) {
        if (0 < (j = SA[i])) {
            --j;
            if ((c0 = (cs == sizeof(sais_index_type)
                           ? ((sais_index_type *)T)[(j)]
                           : ((unsigned char *)T)[(j)])) != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            *--b = ((j == 0) || ((cs == sizeof(sais_index_type)
                                      ? ((sais_index_type *)T)[(j - 1)]
                                      : ((unsigned char *)T)[(j - 1)]) > c1))
                       ? ~j
                       : j;
        } else
            SA[i] = ~j;
    }
}
sais_index_type computeBWT(const void *T, sais_index_type *SA,
                           sais_index_type *C, sais_index_type *B,
                           sais_index_type n, sais_index_type k, int cs) {
    sais_index_type *b, i, j, pidx = -1;
    sais_index_type c0, c1;
    if (C == B) getCounts(T, C, n, k, cs);
    getBuckets(C, B, k, 0);
    j = n - 1;
    b = SA +
        B[c1 = (cs == sizeof(sais_index_type) ? ((sais_index_type *)T)[(j)]
                                              : ((unsigned char *)T)[(j)])];
    *b++ = ((0 < j) && ((cs == sizeof(sais_index_type)
                             ? ((sais_index_type *)T)[(j - 1)]
                             : ((unsigned char *)T)[(j - 1)]) < c1))
               ? ~j
               : j;
    for (i = 0; i < n; ++i) {
        if (0 < (j = SA[i])) {
            --j;
            SA[i] =
                ~((sais_index_type)(c0 = (cs == sizeof(sais_index_type)
                                              ? ((sais_index_type *)T)[(j)]
                                              : ((unsigned char *)T)[(j)])));
            if (c0 != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            *b++ = ((0 < j) && ((cs == sizeof(sais_index_type)
                                     ? ((sais_index_type *)T)[(j - 1)]
                                     : ((unsigned char *)T)[(j - 1)]) < c1))
                       ? ~j
                       : j;
        } else if (j != 0)
            SA[i] = ~j;
    }
    if (C == B) getCounts(T, C, n, k, cs);
    getBuckets(C, B, k, 1);
    for (i = n - 1, b = SA + B[c1 = 0]; 0 <= i; --i) {
        if (0 < (j = SA[i])) {
            --j;
            SA[i] = (c0 = (cs == sizeof(sais_index_type)
                               ? ((sais_index_type *)T)[(j)]
                               : ((unsigned char *)T)[(j)]));
            if (c0 != c1) {
                B[c1] = b - SA;
                b = SA + B[c1 = c0];
            }
            *--b =
                ((0 < j) && ((cs == sizeof(sais_index_type)
                                  ? ((sais_index_type *)T)[(j - 1)]
                                  : ((unsigned char *)T)[(j - 1)]) > c1))
                    ? ~((sais_index_type)(cs == sizeof(sais_index_type)
                                              ? ((sais_index_type *)T)[(j - 1)]
                                              : ((unsigned char *)T)[(j - 1)]))
                    : j;
        } else if (j != 0)
            SA[i] = ~j;
        else
            pidx = i;
    }
    return pidx;
}
sais_index_type sais_main(const void *T, sais_index_type *SA,
                          sais_index_type fs, sais_index_type n,
                          sais_index_type k, int cs, sais_bool_type isbwt) {
    sais_index_type *C, *B, *D, *RA, *b;
    sais_index_type i, j, m, p, q, t, name, pidx = 0, newfs;
    sais_index_type c0, c1;
    unsigned int flags;
    if (k <= MINBUCKETSIZE) {
        if ((C = ((sais_index_type *)malloc((k) * sizeof(sais_index_type)))) ==
            NULL)
            return -2;
        if (k <= fs) {
            B = SA + (n + fs - k);
            flags = 1;
        } else {
            if ((B = ((sais_index_type *)malloc(
                     (k) * sizeof(sais_index_type)))) == NULL) {
                free(C);
                return -2;
            }
            flags = 3;
        }
    } else if (k <= fs) {
        C = SA + (n + fs - k);
        if (k <= (fs - k)) {
            B = C - k;
            flags = 0;
        } else if (k <= (MINBUCKETSIZE * 4)) {
            if ((B = ((sais_index_type *)malloc(
                     (k) * sizeof(sais_index_type)))) == NULL)
                return -2;
            flags = 2;
        } else {
            B = C;
            flags = 8;
        }
    } else {
        if ((C = B = ((sais_index_type *)malloc(
                 (k) * sizeof(sais_index_type)))) == NULL)
            return -2;
        flags = 4 | 8;
    }
    if ((n <= SAIS_LMSSORT2_LIMIT) && (2 <= (n / k))) {
        if (flags & 1)
            flags |= ((k * 2) <= (fs - k)) ? 32 : 16;
        else if ((flags == 0) && ((k * 2) <= (fs - k * 2)))
            flags |= 32;
    }
    getCounts(T, C, n, k, cs);
    getBuckets(C, B, k, 1);
    for (i = 0; i < n; ++i) SA[i] = 0;
    b = &t;
    i = n - 1;
    j = n;
    m = 0;
    c0 = (cs == sizeof(sais_index_type) ? ((sais_index_type *)T)[(n - 1)]
                                        : ((unsigned char *)T)[(n - 1)]);
    do {
        c1 = c0;
    } while ((0 <= --i) && ((c0 = (cs == sizeof(sais_index_type)
                                       ? ((sais_index_type *)T)[(i)]
                                       : ((unsigned char *)T)[(i)])) >= c1));
    for (; 0 <= i;) {
        do {
            c1 = c0;
        } while ((0 <= --i) &&
                 ((c0 = (cs == sizeof(sais_index_type)
                             ? ((sais_index_type *)T)[(i)]
                             : ((unsigned char *)T)[(i)])) <= c1));
        if (0 <= i) {
            *b = j;
            b = SA + --B[c1];
            j = i;
            ++m;
            do {
                c1 = c0;
            } while ((0 <= --i) &&
                     ((c0 = (cs == sizeof(sais_index_type)
                                 ? ((sais_index_type *)T)[(i)]
                                 : ((unsigned char *)T)[(i)])) >= c1));
        }
    }
    if (1 < m) {
        if (flags & (16 | 32)) {
            if (flags & 16) {
                if ((D = ((sais_index_type *)malloc(
                         (k * 2) * sizeof(sais_index_type)))) == NULL) {
                    if (flags & (1 | 4)) free(C);
                    if (flags & 2) free(B);
                    return -2;
                }
            } else
                D = B - k * 2;
            ++B[(cs == sizeof(sais_index_type)
                     ? ((sais_index_type *)T)[(j + 1)]
                     : ((unsigned char *)T)[(j + 1)])];
            for (i = 0, j = 0; i < k; ++i) {
                j += C[i];
                if (B[i] != j) SA[B[i]] += n;
                D[i] = D[i + k] = 0;
            }
            LMSsort2(T, SA, C, B, D, n, k, cs);
            name = LMSpostproc2(SA, n, m);
            if (flags & 16) free(D);
        } else {
            LMSsort1(T, SA, C, B, n, k, cs);
            name = LMSpostproc1(T, SA, n, m, cs);
        }
    } else if (m == 1) {
        *b = j + 1;
        name = 1;
    } else
        name = 0;
    if (name < m) {
        if (flags & 4) free(C);
        if (flags & 2) free(B);
        newfs = (n + fs) - (m * 2);
        if ((flags & (1 | 4 | 8)) == 0) {
            if ((k + name) <= newfs)
                newfs -= k;
            else
                flags |= 8;
        }
        RA = SA + m + newfs;
        for (i = m + (n >> 1) - 1, j = m - 1; m <= i; --i) {
            if (SA[i] != 0) RA[j--] = SA[i] - 1;
        }
        if (sais_main(RA, SA, newfs, m, name, sizeof(sais_index_type), 0) !=
            0) {
            if (flags & 1) free(C);
            return -2;
        }
        i = n - 1;
        j = m - 1;
        c0 = (cs == sizeof(sais_index_type) ? ((sais_index_type *)T)[(n - 1)]
                                            : ((unsigned char *)T)[(n - 1)]);
        do {
            c1 = c0;
        } while ((0 <= --i) &&
                 ((c0 = (cs == sizeof(sais_index_type)
                             ? ((sais_index_type *)T)[(i)]
                             : ((unsigned char *)T)[(i)])) >= c1));
        for (; 0 <= i;) {
            do {
                c1 = c0;
            } while ((0 <= --i) &&
                     ((c0 = (cs == sizeof(sais_index_type)
                                 ? ((sais_index_type *)T)[(i)]
                                 : ((unsigned char *)T)[(i)])) <= c1));
            if (0 <= i) {
                RA[j--] = i + 1;
                do {
                    c1 = c0;
                } while ((0 <= --i) &&
                         ((c0 = (cs == sizeof(sais_index_type)
                                     ? ((sais_index_type *)T)[(i)]
                                     : ((unsigned char *)T)[(i)])) >= c1));
            }
        }
        for (i = 0; i < m; ++i) SA[i] = RA[SA[i]];
        if (flags & 4) {
            if ((C = B = ((int *)malloc((k) * sizeof(int)))) == NULL) return -2;
        }
        if (flags & 2) {
            if ((B = ((int *)malloc((k) * sizeof(int)))) == NULL) {
                if (flags & 1) free(C);
                return -2;
            }
        }
    }
    if (flags & 8) getCounts(T, C, n, k, cs);
    if (1 < m) {
        getBuckets(C, B, k, 1);
        i = m - 1, j = n, p = SA[m - 1],
        c1 = (cs == sizeof(sais_index_type) ? ((sais_index_type *)T)[(p)]
                                            : ((unsigned char *)T)[(p)]);
        do {
            q = B[c0 = c1];
            while (q < j) SA[--j] = 0;
            do {
                SA[--j] = p;
                if (--i < 0) break;
                p = SA[i];
            } while ((c1 = (cs == sizeof(sais_index_type)
                                ? ((sais_index_type *)T)[(p)]
                                : ((unsigned char *)T)[(p)])) == c0);
        } while (0 <= i);
        while (0 < j) SA[--j] = 0;
    }
    if (isbwt == 0)
        induceSA(T, SA, C, B, n, k, cs);
    else
        pidx = computeBWT(T, SA, C, B, n, k, cs);
    if (flags & (1 | 4)) free(C);
    if (flags & 2) free(B);
    return pidx;
}
int sais(const unsigned char *T, int *SA, int n) {
    if ((T == NULL) || (SA == NULL) || (n < 0)) return -1;
    if (n <= 1) {
        if (n == 1) SA[0] = 0;
        return 0;
    }
    return sais_main(T, SA, 0, n, UCHAR_SIZE, sizeof(unsigned char), 0);
}
int sais_int(const int *T, int *SA, int n, int k) {
    if ((T == NULL) || (SA == NULL) || (n < 0) || (k <= 0)) return -1;
    if (n <= 1) {
        if (n == 1) SA[0] = 0;
        return 0;
    }
    return sais_main(T, SA, 0, n, k, sizeof(int), 0);
}
using namespace std;
const int N = 3e5 + 5;
typedef int arr1[N];
typedef long long arr2[N];
arr1 sa, r, h;
struct buf {
    char z[1 << 25], *s;
    buf() : s(z) { fread(z, 1, sizeof z, stdin); }
    inline void pre(char *v) {
        while (*s < 48) ++s;
        while (*s > 32) *v++ = *s++;
        *v = 0;
    }
    operator int() {
        int x = 0, y = 0;
        while (*s < 48) y = *s++;
        while (*s > 32) x = x * 10 + *s++ - 48;
        if (y == 45) return -x;
        return x;
    }
} it;
int main() {
    static char s[N];
    static arr1 a, t;
    register int n = it;
    it.pre(s);
    for (register int i = 0; i < n; ++i) a[i] = it;
    sais((unsigned char *)s, sa, n + 1);
    for (register int i = 1; i <= n; ++i) r[sa[i]] = i;
    for (register int i = 0, j = 0; i < n; ++i) {
        if (j) --j;
        while (s[i + j] == s[sa[r[i] - 1] + j]) ++j;
        h[r[i]] = j;
    }
    static arr2 f1, f2, f3, e1, e2;
    for (register int i = 1; i <= n; ++i) f1[i] = 1, f2[i] = f3[i] = a[sa[i]];
    for (register int i = 0; i < n; ++i) e2[i] = -1e18;
    t[0] = 1;
    for (register int i = 0, j = 2; j <= n + 1; ++j) {
        for (; i && h[j] <= h[t[i]]; --i) {
            int u = t[i], v = t[i - 1];
            e1[h[u]] += f1[u] * f1[v];
            e2[h[u]] = max(max(max(max(e2[h[u]], f2[u] * f2[v]), f2[u] * f3[v]),
                               f3[u] * f2[v]),
                           f3[u] * f3[v]);
            f1[v] += f1[u];
            f2[v] = min(f2[v], f2[u]);
            f3[v] = max(f3[v], f3[u]);
        }
        t[++i] = j;
    }
    for (register int i = n - 2; ~i; --i) {
        e1[i] += e1[i + 1];
        e2[i] = max(e2[i], e2[i + 1]);
    }
    for (register int i = 0; i < n; ++i)
        write(e1[i]), writeChar(' '), write(!!e1[i] * e2[i]), writeChar('\n');
    flush();
}

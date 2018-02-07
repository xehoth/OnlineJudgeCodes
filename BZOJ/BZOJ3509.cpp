/*
 * created by xehoth on 18-03-2017
 * 不能并行?
 * CPU 并发!(好虚啊)
 * 假设不卡 CPU 并发...
 */
#include <bits/stdc++.h>

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
inline bool read(T &x) {
    static bool iosig;
    static char c;
    for (iosig = false, c = read(); !isdigit(c); c = read()) {
        if (c == '-') iosig = true;
        if (c == -1) return false;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
    return true;
}

const int OUT_LEN = 10000000;
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

typedef unsigned long long ull;
typedef unsigned int uint;
#define long long long

const int MAXN = 100000;
const int MAX_VAL = 30000;
uint v[MAXN], bucL[MAX_VAL + 1], bucR[MAX_VAL + 1];
// #pragma GCC comment(linker, "-fopenmp")
// #include <parallel/algorithm>
// 并行
// namespace Parallel {

// __gnu_parallel::shared_t ans, n, i, tmp;

// inline void parallelSolve1() {
// n--, n >>= 1;
// for (i = 1; i < n; i++) {
// register int last = v[i - 1], cur = v[i];
// if (last < minL) minL = last;
// else if (last > maxL) maxL = last;

// bucL[last]++;
// bucR[MAX_VAL - cur]--;

// register int bufx = cur << 1, low = std::max(minL, bufx - MAX_VAL),
// high = std::min(maxL, bufx - 1);
// register uint tmp = 0, *p1 = bucL + low, *pr = bucL + high - 14,
// *p2 = bucR + MAX_VAL - bufx + low;
// while (p1 <= pr) {
// tmp += (*p1) * (*p2) + (*(p1 + 1)) * (*(p2 + 1)) + (*(p1 + 2)) *
// (*(p2 + 2)) + (*(p1 + 3)) * (*(p2 + 3)) + (*(p1 + 4)) * (*(p2 + 4))
// + (*(p1 + 5)) * (*(p2 + 5)) + (*(p1 + 6)) * (*(p2 + 6)) + (*(p1 + 7))
// * (*(p2 + 7)) + (*(p1 + 8)) * (*(p2 + 8)) + (*(p1 + 9)) * (*(p2 + 9))
// + (*(p1 + 10)) * (*(p2 + 10)) + (*(p1 + 11)) * (*(p2 + 11))
// + (*(p1 + 12)) * (*(p2 + 12)) + (*(p1 + 13)) * (*(p2 + 13))
// + (*(p1 + 14)) * (*(p2 + 14));

// p1 += 15, p2 += 15;
// }
// while (p1 <= bucL + high) tmp += (*(p1++)) * (*(p2++));
// ans += tmp;
// }
// }

// inline void parallelSolve2() {
// n--;
// for (i = n >> 1; i < n; i++) {
// register int last = v[i - 1], cur = v[i];
// if (last < minL) minL = last;
// else if (last > maxL) maxL = last;

// bucL[last]++;
// bucR[MAX_VAL - cur]--;

// register int bufx = cur << 1, low = std::max(minL, bufx - MAX_VAL),
// high = std::min(maxL, bufx - 1);
// register uint tmp = 0, *p1 = bucL + low, *pr = bucL + high - 14,
// *p2 = bucR + MAX_VAL - bufx + low;
// while (p1 <= pr) {
// tmp += (*p1) * (*p2) + (*(p1 + 1)) * (*(p2 + 1)) + (*(p1 + 2)) *
// (*(p2 + 2)) + (*(p1 + 3)) * (*(p2 + 3)) + (*(p1 + 4)) * (*(p2 + 4))
// + (*(p1 + 5)) * (*(p2 + 5)) + (*(p1 + 6)) * (*(p2 + 6)) + (*(p1 + 7))
// * (*(p2 + 7)) + (*(p1 + 8)) * (*(p2 + 8)) + (*(p1 + 9)) * (*(p2 + 9))
// + (*(p1 + 10)) * (*(p2 + 10)) + (*(p1 + 11)) * (*(p2 + 11))
// + (*(p1 + 12)) * (*(p2 + 12)) + (*(p1 + 13)) * (*(p2 + 13))
// + (*(p1 + 14)) * (*(p2 + 14));

// p1 += 15, p2 += 15;
// }
// while (p1 <= bucL + high) tmp += (*(p1++)) * (*(p2++));
// ans += tmp;
// }
// }

// inline void parallelSolveFather() {
// for (i = 0; i < n; i++)
// read(tmp), bucR[MAX_VAL - (v[i] = tmp)]++;

// bucR[MAX_VAL - v[0]]--;
// register int minL = v[0], maxL = v[0];

// register long ans = 0;
// }
// inline void parallelSolve() {
// parallelSolveFather();
// __gnu_parallel::thread_t t1 = __gnu_parallel::create_thread(2,
// parallelSolve1);

// __gnu_parallel::thread_t t2 = __gnu_parallel::create_thread(2,
// parallelSolve2);

// print(ans);
// }
// } //end

namespace Concurrent {

/*concurrent*/
inline void concurrentSolve() {
    // real register %esp %ebp %eax
    register int i, n, tmp;
    read(n);
    for (i = 0; i < n; i++) read(tmp), bucR[MAX_VAL - (v[i] = tmp)]++;

    bucR[MAX_VAL - v[0]]--;
    register int minL = v[0], maxL = v[0];

    register long ans = 0;

    n--;
    for (i = 1; i < n; i++) {
        register int last = v[i - 1], cur = v[i];
        if (last < minL)
            minL = last;
        else if (last > maxL)
            maxL = last;

        bucL[last]++;
        bucR[MAX_VAL - cur]--;

        register int bufx = cur << 1, low = std::max(minL, bufx - MAX_VAL),
                     high = std::min(maxL, bufx - 1);
        /*CPU 并发优化*/
        register uint tmp = 0, *p1 = bucL + low, *pr = bucL + high - 14,
                      *p2 = bucR + MAX_VAL - bufx + low;
        /*循环展开 + 刺激并发*/
        // 0x0041e203 <+723>:   imul   -0x8(%eax),%ebx
        // 0x0041e207 <+727>:   add    %ebx,%ebp
        // 0x0041e209 <+729>:   mov    -0xc(%edx),%ebx
        // 0x0041e20c <+732>:   imul   -0xc(%eax),%ebx
        // 0x0041e210 <+736>:   add    %ebx,%ebp
        // 0x0041e212 <+738>:   mov    -0x4(%edx),%ebx
        // 0x0041e215 <+741>:   imul   -0x4(%eax),%ebx
        // 0x0041e219 <+745>:   add    %ebp,%ebx
        // 0x0041e21b <+747>:   add    %ebx,%ecx
        // 0x0041e21d <+749>:   cmp    %eax,%esi
        // 0x0041e21f <+751>:   jae    0x41e190
        // <Concurrent::concurrentSolve()+608>
        while (p1 <= pr) {
            tmp += (*p1) * (*p2) + (*(p1 + 1)) * (*(p2 + 1)) +
                   (*(p1 + 2)) * (*(p2 + 2)) + (*(p1 + 3)) * (*(p2 + 3)) +
                   (*(p1 + 4)) * (*(p2 + 4)) + (*(p1 + 5)) * (*(p2 + 5)) +
                   (*(p1 + 6)) * (*(p2 + 6)) + (*(p1 + 7)) * (*(p2 + 7)) +
                   (*(p1 + 8)) * (*(p2 + 8)) + (*(p1 + 9)) * (*(p2 + 9)) +
                   (*(p1 + 10)) * (*(p2 + 10)) + (*(p1 + 11)) * (*(p2 + 11)) +
                   (*(p1 + 12)) * (*(p2 + 12)) + (*(p1 + 13)) * (*(p2 + 13)) +
                   (*(p1 + 14)) * (*(p2 + 14));

            p1 += 15, p2 += 15;
        }
        while (p1 <= bucL + high) tmp += (*(p1++)) * (*(p2++));
        ans += tmp;
    }

    print(ans);
}

}  // namespace Concurrent

int main() {
#ifndef ONLINE_JUDGE
// freopen("in.in", "r", stdin);
#endif
    Concurrent::concurrentSolve();
    flush();
    return 0;
}

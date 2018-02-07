/*******************************************************************************
 * Copyright (c) 2016-2017, xehoth
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name xehoth, nor the names of its contributors may be used
 *       to endorse or promote products derived from this software without
 *       specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY XEHOTH AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEHOTH AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
#ifndef DBG

#define NDEBUG

#endif

#include <bits/stdc++.h>

namespace IO {
/**
 * T1 28-05-2017
 * 贪心
 * O(n \log n)
 * @author xehoth
 */
inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = '\0';
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = '\0';
    return s;
}

inline void read(double &x) {
    static char buf[30];
    read(buf), x = atof(buf);
}

template <typename T1, typename T2>
inline void read(T1 &a, T2 &b) {
    read(a), read(b);
}

template <typename T1, typename T2, typename T3>
inline void read(T1 &a, T2 &b, T3 &c) {
    read(a), read(b), read(c);
}

template <typename T1, typename T2, typename T3, typename T4>
inline void read(T1 &a, T2 &b, T3 &c, T4 &d) {
    read(a), read(b), read(c), read(d);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline void read(T1 &a, T2 &b, T3 &c, T4 &d, T5 &e) {
    read(a), read(b), read(c), read(d), read(e);
}

const int OUT_LEN = 1000000;

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

template <typename T>
inline void println(T x) {
    print(x), print('\n');
}

template <typename T1, typename T2>
inline void print(T1 a, T2 b) {
    print(a), print(b);
}

template <typename T1, typename T2, typename T3>
inline void print(T1 a, T2 b, T3 c) {
    print(a), print(b), print(c);
}

template <typename T1, typename T2, typename T3, typename T4>
inline void print(T1 a, T2 b, T3 c, T4 d) {
    print(a), print(b), print(c), print(d);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline void print(T1 a, T2 b, T3 c, T4 d, T5 e) {
    print(a), print(b), print(c), print(d), print(e);
}

template <typename T1, typename T2>
inline void println(T1 a, T2 b) {
    print(a), print(b), print('\n');
}

template <typename T1, typename T2, typename T3>
inline void println(T1 a, T2 b, T3 c) {
    print(a), print(b), print(c), print('\n');
}

template <typename T1, typename T2, typename T3, typename T4>
inline void println(T1 a, T2 b, T3 c, T4 d) {
    print(a), print(b), print(c), print(d), print('\n');
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline void println(T1 a, T2 b, T3 c, T4 d, T5 e) {
    print(a), print(b), print(c), print(d), print(e), print('\n');
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace Task {

typedef std::pair<int, int> Pair;

inline void solve() {
    using namespace IO;
    register int n;
    read(n);
    static std::deque<Pair> q(n);
    for (register int i = 0; i < n; i++) read(q[i].second, q[i].first);
    std::sort(q.begin(), q.end());
    register int ans = 0;
    while (q.size() > 1) {
        ans = std::max(ans, q.front().first + q.back().first);
        register int tmp = std::min(q.front().second, q.back().second);
        q.front().second -= tmp;
        if (!q.front().second) q.pop_front();
        q.back().second -= tmp;
        if (!q.back().second) q.pop_back();
    }
    if (q.size() == 1) ans = std::max(ans, q.front().first + q.back().first);
    print(ans);
}
}  // namespace Task

int main() {
#ifndef ONLINE_JUDGE
#ifdef DBG
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    freopen("debug.log", "w", stderr);
#else
    freopen("pairup.in", "r", stdin);
    freopen("pairup.out", "w", stdout);
#endif
#endif
    Task::solve();
    IO::flush();
    return 0;
}
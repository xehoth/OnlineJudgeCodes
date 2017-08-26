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
#include <bits/stdc++.h>
/**
 * 「SDOI2017」硬币游戏 10-05-2017
 * @author xehoth 
 */
namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template<class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read())
        x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1);
    if (c == -1) {
        *buf = '\0';
        return -1;
    }
    do buf[s++] = c; while (c = read(), !isspace(c) && c != -1);
    buf[s] = '\0';
    return s;
}
}

namespace Task {

const int MAXN = 300 + 3, MAXM = 300 + 3;

inline void gauss(int n, double a[][MAXN]) {
    for (register int i = 0, idx; i < n; ++i) {
        idx = i;
        for (register int j = i + 1; j < n; j++)
            if (fabs(a[idx][i]) < fabs(a[j][i])) idx = j;
        if (idx != i) 
            for (register int j = i; j <= n; j++) 
                std::swap(a[idx][j], a[i][j]);
        for (register int j = 0; j < n; j++) {
            if (j != i) {
                register double t = a[j][i] / a[i][i];
                for (register int k = i; k <= n; k++) a[j][k] -= t * a[i][k];
            }
        }
    }
    for (register int i = 0; i < n; i++) a[i][n] /= a[i][i], a[i][i] = 1;
}

int n, m;
char str[MAXN][MAXM];
int f[MAXN][MAXM];
double pow[MAXM];

inline double calc(int a, int b) {
    register int j = 0;
    for (register int i = 0; i < m; i++) {
        while (j && str[b][i] != str[a][j]) j = f[a][j];
        if (str[b][i] == str[a][j]) j++;
    }
    if (a == b) j = f[a][j];
    register double res = 0;
    while (j) res += pow[m - j], j = f[a][j];
    return res;
}

inline void solve() {
    using namespace IO;
    read(n), read(m);
    for (register int i = 0; i < n; i++) {
        read(str[i]);
        for (register int j = 1, k; j < m; j++) {
            k = f[i][j];
            while (k && str[i][j] != str[i][k]) k = f[i][k];
            f[i][j + 1] = str[i][j] == str[i][k] ? k + 1 : 0;
        }
    }
    pow[0] = 1;

    for (register int i = 1; i <= m; i++) pow[i] = pow[i - 1] * 0.5;

    static double a[MAXN][MAXN];

    for (register int i = 0; i < n; i++)
        for (register int j = 0; j < n; j++)
            a[i][j] = calc(i, j);

    for (register int i = 0; i < n; i++) a[i][n] = -pow[m], a[i][i]++;

    for (register int i = 0; i < n; ++i) a[n][i] = 1;
    a[n][n + 1] = 1;

    gauss(n + 1, a);

    for (register int i = 0; i < n; ++i) printf("%.10lf\n", a[i][n + 1]);
}

}

int main() {
    freopen("game.in", "r", stdin);
    freopen("game.out", "w", stdout);
    Task::solve();
    return 0;
}
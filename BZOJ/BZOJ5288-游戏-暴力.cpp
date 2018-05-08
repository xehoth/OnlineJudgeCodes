/**
 * Copyright (c) 2016-2018, xehoth
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * 「BZOJ 5288」游戏 08-05-2018
 * 暴力
 * @author xehoth
 */
#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
    char *buf, *p;
    int size;
    char *oh, obuf[SIZE];

    InputOutputStream() : oh(obuf) {
        int fd = fileno(stdin);
        struct stat sb;
        fstat(fd, &sb);
        size = sb.st_size;
        buf = (char *)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
        p = buf;
    }
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read() { return *p++; }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        for (c = read(); !isdigit(c);) c = read();
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        return *this;
    }

    inline void print(char c) {
        if (oh == obuf + SIZE) {
            fwrite(obuf, 1, SIZE, stdout);
            oh = obuf;
        }
        *oh++ = c;
    }

    inline InputOutputStream &operator<<(const char *s) {
        for (; *s; s++) print(*s);
        return *this;
    }
} io;

const int MAXN = 1000000 + 9;

int n, m, p, a[MAXN], l[MAXN], r[MAXN];

inline bool valid(int pos, int i) { return l[i] <= pos && r[i] >= pos; }

inline void force() {
    io >> n >> m >> p;
    for (int i = 1, x, y; i <= m; i++) {
        io >> x >> y;
        a[x] = y;
    }
    for (int i = 1; i <= n; i++) l[i] = r[i] = i;
    for (int i = n; i; i--)
        while (r[i] < n && (!a[r[i]] || valid(a[r[i]], i))) r[i] = r[r[i] + 1];
    for (int i = 1; i <= n; i++) {
        for (bool updated = true; updated;) {
            updated = false;
            if (l[i] > 1 && (!a[l[i] - 1] || valid(a[l[i] - 1], i))) {
                l[i] = l[l[i] - 1];
                updated = true;
            }
            if (r[i] < n && (!a[r[i]] || valid(a[r[i]], i))) {
                r[i] = r[r[i] + 1];
                updated = true;
            }
        }
    }
    for (int s, t; p--;) {
        io >> s >> t;
        io << (valid(t, s) ? "YES\n" : "NO\n");
    }
}

int main() {
#ifdef _DEBUG
    clock_t start = clock();
#endif
    force();
#ifdef _DEBUG
    clock_t end = clock();
    fprintf(stderr, "\ntime: %.3fs\n", (end - start) / (double)CLOCKS_PER_SEC);
#endif
    return 0;
}
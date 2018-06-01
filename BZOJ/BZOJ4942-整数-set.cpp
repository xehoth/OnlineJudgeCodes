/**
 * Copyright (c) 2018, xehoth
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
 * 「BZOJ 4942」整数 22-05-2018
 * set
 * @author xehoth
 */
#include <bits/stdc++.h>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        if (iosig) x = -x;
        return *this;
    }

    inline InputOutputStream &operator<<(char c) {
        if (oh == obuf + SIZE) {
            fwrite(obuf, 1, SIZE, stdout);
            oh = obuf;
        }
        *oh++ = c;
        return *this;
    }

    inline void skipInt() {
        int t;
        this->operator>>(t);
    }
} io;

const int MAXN = 30000000;

int a[MAXN + 1];
bool vis[MAXN + 1];

const int POOL_SIZE = 256 * 1024 * 1024;

char pool[POOL_SIZE];

void *operator new(size_t size) {
    static char *s = pool + POOL_SIZE;
    return s -= size;
}

void operator delete(void *) {}
void operator delete(void *, size_t) {}

int main() {
    int n;
    io >> n;
    io.skipInt();
    io.skipInt();
    io.skipInt();
    static std::set<int> s;
    s.insert(s.end(), -1);
    s.insert(s.end(), 1 << 30);
    for (int cmd; n--;) {
        io >> cmd;
        switch (cmd) {
            case 1: {
                int i, k;
                io >> i >> k;
                std::set<int>::iterator it = s.lower_bound(k), tmp;
                for (a[k] += i; std::abs(a[k]) > 1; k++) {
                    a[k + 1] += a[k] / 2;
                    a[k] %= 2;
                    if (vis[k] && !a[k]) {
                        while (*it != k) ++it;
                        vis[k] = 0;
                        tmp = it++;
                        s.erase(tmp);
                    }
                    if (!vis[k] && a[k]) {
                        vis[k] = 1;
                        it = s.insert(it, k);
                    }
                }
                if (vis[k] && !a[k]) {
                    while (*it != k) ++it;
                    vis[k] = 0;
                    s.erase(it);
                }
                if (!vis[k] && a[k]) {
                    vis[k] = 1;
                    s.insert(it, k);
                }
                break;
            }
            case 2: {
                int k;
                io >> k;
                std::set<int>::iterator it = s.lower_bound(k);
                int a = *it, b = *--it;
                int ans = (a == k && ::a[k] != 0);
                if (b != -1 && ::a[b] == -1) ans ^= 1;
                io << "01"[ans] << '\n';
                break;
            }
        }
    }
    return 0;
}
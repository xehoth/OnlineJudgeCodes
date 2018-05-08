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
 * 「BZOJ 5300」九连环 04-05-2018
 * 高精度
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 30105 + 100;

struct BigInteger {
    static const long long DIGIT = 1000000000;
    static const int DIGIT_LEN = 9;
    static const int LEN = MAXN / DIGIT_LEN * 2 + 9;

    long long a[LEN];
    int len;

    BigInteger() : a(), len(1) {}

    void clear() {
        memset(a, 0, sizeof(long long) * (len + 1));
        len = 1;
    }

    void fix() {
        while (len > 1 && !a[len]) len--;
    }

    void multiply(const BigInteger &a, const BigInteger &b, BigInteger &c) {
        c.clear();
        for (int i = 1, pos; i <= a.len; i++) {
            for (int j = 1; j <= b.len; j++) {
                pos = i + j - 1;
                c.a[pos] += a.a[i] * b.a[j];
                c.a[pos + 1] += c.a[pos] / DIGIT;
                c.a[pos] %= DIGIT;
            }
        }
        c.len = a.len + b.len;
        c.fix();
    }

    void divide(const BigInteger &a, int b, BigInteger &c) {
        c.clear();
        long long t = 0;
        for (int i = len; i; i--) {
            c.a[i] = (a.a[i] + t) / b;
            t = ((a.a[i] + t) % b) * DIGIT;
        }
        c.len = len;
        c.fix();
    }

    static BigInteger tmp;

    void print() {
        printf("%d", (int)a[len]);
        for (int i = len - 1; i; i--) printf("%09d", (int)a[i]);
    }

    void operator*=(const BigInteger &b) {
        multiply(*this, b, tmp);
        *this = tmp;
    }

    void operator/=(int b) {
        divide(*this, b, tmp);
        *this = tmp;
    }
} a, b;

BigInteger BigInteger::tmp;

BigInteger f[31];

int main() {
    int m;
    std::cin >> m;
    a.a[1] = 2;
    a.len = 1;
    f[0] = a;
    for (int i = 1; (1 << i) <= 100001; i++) {
        a *= a;
        f[i] = a;
    }
    while (m--) {
        b.clear();
        b.a[1] = 1;
        int n;
        std::cin >> n;
        int idx = n + 1;
        for (int i = 0; (1 << i) <= idx; i++) {
            if (idx >> i & 1) {
                b *= f[i];
            }
        }
        b /= 3;
        b.print();
        putchar('\n');
    }
    return 0;
}
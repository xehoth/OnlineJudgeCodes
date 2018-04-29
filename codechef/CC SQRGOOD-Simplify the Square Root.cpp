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
 * 「CC SQRGOOD」Simplify the Square Root 29-04-2018
 * 杜教筛 + 二分
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 50000000;
const double PI = acos(-1);

int p[MAXN / 8], mu[MAXN + 9], mu2[MAXN + 2], cnt;
bool vis[MAXN + 9];

inline void init(int n) {
    mu[1] = mu2[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (!vis[i]) {
            p[cnt++] = i;
            mu[i] = -1;
        }

        for (int j = 0, k; j < cnt && (k = i * p[j]) <= n; j++) {
            vis[k] = true;
            if (i % p[j] == 0) {
                mu[k] = 0;
                break;
            } else {
                mu[k] = -mu[i];
            }
        }
    }
    for (int i = 2; i <= n; i++) {
        mu2[i] = mu2[i - 1] + (mu[i] ? 1 : 0);
        mu[i] += mu[i - 1];
    }
}

std::unordered_map<int, int> h;

int getMu(int n) {
    if (n <= MAXN) return mu[n];
    if (h.count(n)) return h[n];
    int &ret = h[n];
    ret = 1;
    for (int i = 2, pos; i <= n; i = pos + 1) {
        pos = n / (n / i);
        ret -= getMu(n / i) * (pos - i + 1);
    }
    return ret;
}

inline long long get(long long n) {
    if (n <= MAXN) return mu2[n];
    long long ret = 0, last = 0, tmp, t, i = 1;
    for (; i * i * i <= n; i++) {
        ret += (n / (i * i)) * ((tmp = mu[i]) - last);
        last = tmp;
    }
    t = n / (i * i);
    for (ret -= t * last; t; t--) ret += getMu(sqrt(n / t));
    return ret;
}

int main() {
    long long n;
    init(MAXN);
    std::cin >> n;
    long long l = n / (1 - 6 / PI / PI), r, step = 1, o = l;
    if (l - get(l) >= n) {
        r = l;
        for (;; step <<= 1) {
            l = o - step;
            if (l <= 0) {
                l = 1;
                break;
            }
            if (l - get(l) < n) {
                break;
            }
            r = l;
        }
    } else {
        for (;; step <<= 1) {
            r = o + step;
            if (r - get(r) >= n) {
                break;
            }
            l = r;
        }
    }
    l--;
    r++;
    for (long long mid; r - l > 1;) {
        mid = (l + r) >> 1;
        if (mid - get(mid) >= n) {
            r = mid;
        } else {
            l = mid;
        }
    }
    std::cout << r;
    return 0;
}

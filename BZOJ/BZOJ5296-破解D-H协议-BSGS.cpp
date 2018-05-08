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
 * 「BZOJ 5296」破解D-H协议 03-05-2018
 * BSGS
 * @author xehoth
 */
#include <bits/stdc++.h>

template <class T = int, size_t HASH_SIZE = 99991, size_t MAXN = 46349>
struct HashMap {
    struct Edge {
        T key, val;
        Edge *next;

        inline Edge *init(const T &key, const T &val, Edge *next) {
            this->key = key;
            this->val = val;
            this->next = next;
            return this;
        }
    };

    Edge *head[HASH_SIZE], edge[MAXN], *cur;

    HashMap() : cur(edge) {}

    inline void clear() {
        cur = edge;
        memset(head, 0, sizeof(head));
    }

    inline void put(const T &key, const T &val) {
        int index = key % HASH_SIZE;
        Edge *p = get(key);
        if (p)
            p->val = val;
        else
            head[index] = (++cur)->init(key, val, head[index]);
    }

    inline Edge *get(const T &key) {
        for (Edge *p = head[key % HASH_SIZE]; p; p = p->next)
            if (p->key == key) return p;
        return NULL;
    }
};

HashMap<> h;

inline int modPow(int a, int b, int mod) {
    int ret = 1;
    for (; b; b >>= 1, a = (long long)a * a % mod)
        if (b & 1) ret = (long long)ret * a % mod;
    return ret;
}

inline int bsgs(int a, int b, int c) {
    int s = sqrt(c), p = 1;
    h.clear();
    for (int i = 0; i < s; i++) {
        if (p == b) return i;
        h.put((long long)p * b % c, i);
        p = (long long)p * a % c;
    }
    int q = p;
    for (int i = s; i - s + 1 <= c - 1; i += s) {
        HashMap<>::Edge *it = h.get(q);
        if (it != NULL) return i - it->val;
        q = (long long)q * p % c;
    }
    return -1;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    int g, P, n;
    std::cin >> g >> P >> n;
    for (int A, B; n--;) {
        std::cin >> A >> B;
        int b = bsgs(g, B, P);
        std::cout << modPow(A, b, P) << std::endl;
    }
    return 0;
}

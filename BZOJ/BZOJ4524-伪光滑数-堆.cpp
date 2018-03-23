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
 * 「BZOJ 4524」伪光滑数 22-03-2018
 * 堆
 * @author xehoth
 */
#include <cstdio>
#include <iostream>
#include <queue>

const int PRIME[] = {2,  3,  5,  7,   11,  13,  17,  19,  23, 29, 31,
                     37, 41, 43, 47,  53,  59,  61,  67,  71, 73, 79,
                     83, 89, 97, 101, 103, 107, 109, 113, 127};
const int MAXP = 31;

struct Node {
    long long val;
    int maxP, idx, ex;

    Node(long long val, int maxP, int idx, int ex)
        : val(val), maxP(maxP), idx(idx), ex(ex) {}

    inline bool operator<(const Node &p) const { return val < p.val; }
};

class PriorityQueue : public std::priority_queue<Node> {
   private:
    typedef std::priority_queue<Node> super;

   public:
    inline void reserve(int n) { super::c.reserve(n); }

    inline void push_back(const Node &x) { super::c.push_back(x); }

    inline void init() {
        std::make_heap(super::c.begin(), super::c.end(), super::comp);
    }
} q;

long long n;
int k;

int main() {
    std::cin >> n >> k;
    q.reserve(k * 2);
    for (int i = 0; i < MAXP; i++) {
        long long x = PRIME[i];
        for (int j = 1; x <= n; j++, x *= PRIME[i])
            q.push_back(Node(x, PRIME[i], j, i - 1));
    }
    for (q.init(); k--;) {
        Node d = q.top();
        q.pop();
        if (k == 0) {
            std::cout << d.val;
            return 0;
        }
        if (d.idx > 1) {
            for (int i = 0; i <= d.ex; i++) {
                q.push(Node(d.val / d.maxP * PRIME[i], d.maxP, d.idx - 1, i));
            }
        }
    }
    return 0;
}
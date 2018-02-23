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
 * 「POJ 2002」Squares 23-02-2018
 * 计算几何-正方形 Hash
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

std::vector<std::pair<int, int> > p;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    for (int n; std::cin >> n && n;) {
        p.resize(n);
        for (int i = 0; i < n; i++) std::cin >> p[i].first >> p[i].second;
        std::sort(p.begin(), p.end());
        int ans = 0;
        for (int i = 0, dx, dy; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                dx = p[i].first - p[j].first;
                dy = p[i].second - p[j].second;
                ans += std::binary_search(
                           p.begin(), p.end(),
                           std::make_pair(p[i].first - dy, p[i].second + dx)) &&
                       std::binary_search(
                           p.begin(), p.end(),
                           std::make_pair(p[j].first - dy, p[j].second + dx));
                ans += std::binary_search(
                           p.begin(), p.end(),
                           std::make_pair(p[i].first + dy, p[i].second - dx)) &&
                       std::binary_search(
                           p.begin(), p.end(),
                           std::make_pair(p[j].first + dy, p[j].second - dx));
            }
        }
        std::cout << ans / 4 << '\n';
    }
    return 0;
}
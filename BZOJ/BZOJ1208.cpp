/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1208」01-10-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MOD = 1000000;
const int INF = 0x7fffffff;

int t, n, ans;
std::set<int> st;

inline void query(int x) {
    std::set<int>::iterator l = --st.lower_bound(x), r = st.lower_bound(x);
    if (x - *l <= *r - x && *l != -INF)
        ans += x - *l, st.erase(l);
    else
        ans += *r - x, st.erase(r);
    ans %= MOD;
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    st.insert(INF), st.insert(-INF);
    std::cin >> n;
    for (int i = 1, a, b; i <= n; i++) {
        std::cin >> a >> b;
        if (st.size() == 2)
            t = a, st.insert(b);
        else if (a == t)
            st.insert(b);
        else
            query(b);
    }
    std::cout << ans;
    return 0;
}
/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 071C」怪文書 20-08-2017
 * dp
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n;
    std::cin >> n;
    std::streambuf *ob = std::cout.rdbuf(), *ib = std::cin.rdbuf();
    static int buc[26], f[26];
    memset(f, 127, sizeof(f));
    register char c;
    for (register int i = 0; i < n; i++) {
        memset(buc, 0, sizeof(buc));
        while (c = ib->sbumpc(), !isalpha(c))
            ;
        while (isalpha(c)) buc[c - 'a']++, c = ib->sbumpc();
        for (register int i = 0; i < 26; i++) f[i] = std::min(f[i], buc[i]);
    }
    for (register int i = 0; i < 26; i++)
        for (; f[i]--;) ob->sputc('a' + i);
}
}

int main() {
    Task::solve();
    return 0;
}

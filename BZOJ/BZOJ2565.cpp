#include <bits/stdc++.h>
using namespace std;
const int MAXN = 1e5 + 10;
#define SIGMA 26
struct PalindromicTree {
    int next[MAXN][SIGMA];
    int fail[MAXN], cnt[MAXN], len[MAXN], num[MAXN], s[MAXN];
    int n, p, last;
    inline int newNode(int l) {
        return memset(p[next], 0, sizeof(p[next])), p[cnt] = p[num] = 0,
                                                    p[len] = l, p++;
    }
    inline void init() {
        last = n = p = 0, newNode(0), newNode(-1), s[0] = -1, fail[0] = 1,
        fail[1] = 0;
    }
    inline int getFail(int x) {
        while (s[n - x[len] - 1] != s[n]) x = x[fail];
        return x;
    }
    inline int extend(int c) {
        c -= 'a', s[++n] = c;
        register int cur = getFail(last);
        if (!(cur[next])[c]) {
            register int now = newNode(cur[len] + 2);
            now[fail] = (getFail(cur[fail])[next])[c], (cur[next])[c] = now,
            now[num] = fail[now][num] + 1;
        }
        last = (cur[next])[c], last[cnt]++;
        return last;
    }
    inline void count() {
        for (register int i = p - 1; i >= 0; i--) i[fail][cnt] += i[cnt];
    }
};
PalindromicTree T;
int len[MAXN];
char s[MAXN];
int main() {
    while (scanf("%s", s + 1) == 1) {
        T.init();
        int n = strlen(s + 1);
        for (int i = n; i >= 1; --i) len[i] = T.len[T.extend(s[i])];
        int ans = 0;
        T.init();
        for (int i = 1; i < n; ++i)
            ans = max(ans, len[i + 1] + T.len[T.extend(s[i])]);
        printf("%d\n", ans);
    }
    return 0;
}

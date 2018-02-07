#include <cstdio>
#include <cstring>

const int maxn(3e5 + 7);

int p[maxn], l[maxn], c[maxn], ch[26][maxn], cnt, lst;

int main() {
    static char s[maxn];
    static long long ans(0), tmp;

    gets(s + 1);

    l[1] = -1;
    p[0] = p[1] = 1;

    cnt = 1, lst = 0;

    for (register int i = 1, v, k; s[i]; ++i) {
        static int &u = lst;
        while (s[i - l[u] - 1] ^ s[i]) u = p[u];
        if (!ch[s[i] - 'a'][u]) {
            v = ++cnt;
            k = p[u];
            l[v] = l[u] + 2;
            while (s[i - l[k] - 1] ^ s[i]) k = p[k];
            p[v] = ch[s[i] - 'a'][k];
            ch[s[i] - 'a'][u] = v;
        }
        u = ch[s[i] - 'a'][u];
        ++c[u];
    }

    for (register int i = cnt; i; --i) {
        tmp = 1ll * c[i] * l[i];
        c[p[i]] += c[i];
        if (tmp > ans) ans = tmp;
    }

    printf("%lld\n", ans);

    return 0;
}

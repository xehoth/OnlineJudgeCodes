#include <bits/stdc++.h>
using namespace std;
char ch;
inline void read(int &x) {
    x = 0;
    do ch = getchar(); while (!isdigit(ch));
    while (isdigit(ch)) x = (x << 1) + (x << 3) + (ch ^ '0'), ch = getchar();
}
const int MAXN = 100;
const int MAXLEN = 10000 + 1;
const int MAXM = 1000000;
const int MOD1 = 21893;
const int MOD2 = 18341629;
inline int parse(const char *s, const int mod) {
    int res = 0, sgn = 1;
    if (*s == '-') s++, sgn = -1;
    for (const char *p = s; *p; p++) res = ((res << 1) + (res << 3) + (*p ^ '0')) % mod;
    return res * sgn;
}
int main() {
#ifndef ONLINE_JUDGE
	freopen("in.in", "r", stdin);
#endif
	static char s[MAXN + 1][MAXLEN + 1];
	int n, m;
	scanf("%d %d", &n, &m);
    for (int i = 0; i <= n; i++)
        scanf("%s", s[i]);
    static int a[MAXN + 1];
    for (int i = 0; i <= n; i++) a[i] = parse(s[i], MOD1);
    std::list<int> roots;
	for (int i = 1; i < MOD1; i++) {
        long long pow = 1, val = 0;
        for (int j = 0; j <= n; j++) {
            (val += a[j] * pow) %= MOD1;
            pow = pow * i % MOD1;
        }
        if (val == 0) {
            for (int j = i; j <= m; j += MOD1) roots.push_back(j);
        }
    }
    for (int j = 0; j <= n; j++) a[j] = parse(s[j], MOD2);
    for (std::list<int>::iterator it = roots.begin(); it != roots.end(); ) {
        long long pow = 1, val = 0;
        for (int j = 0; j <= n; j++) {
            (val += a[j] * pow) %= MOD2;
            pow = pow * *it % MOD2;
        }
        if (val != 0) it = roots.erase(it);
        else it++;
    }
    cout << roots.size() << "\n";
    roots.sort();
    for (std::list<int>::iterator it = roots.begin(); it != roots.end(); it++)
    	cout << *it << "\n";
	return 0;
}
/**
 * Copyright (c) 2016, xehoth
 * All rights reserved.
 * 「TJOI 2013」单词 26-07-2016
 * AC 自动机
 * @author xehoth
 */
#include <bits/stdc++.h>
#define MAX_N 1050050
using namespace std;
int n, len, l, tot, head = 0, tail = 1;
char ss[MAX_N], s[MAX_N];
int first[MAX_N], next[MAX_N], ans[MAX_N], fail[MAX_N], f[MAX_N][27];
int st, tag[MAX_N], line[MAX_N];

void solve(int v, int x) {
    if (x == l) {
        next[++st] = first[v];
        first[v] = st;
        return;
    }
    if (!f[v][s[x + 1] - 'a']) f[v][s[x + 1] - 'a'] = ++tot;
    solve(f[v][s[x + 1] - 'a'], x + 1);
}

void ac_bfs() {
    line[1] = 0;
    while (head ^ tail) {
        head++;
        if (line[head])
            for (int j = 0; j < 26; j++)
                if (f[line[head]][j])
                    fail[f[line[head]][j]] = f[fail[line[head]]][j];
        for (int j = 0; j < 26; j++)
            if (!f[line[head]][j])
                f[line[head]][j] = f[fail[line[head]]][j];
            else
                line[++tail] = f[line[head]][j];
    }
}

int main() {
    // freopen("in.in", "r", stdin);
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> n;
    for (register int i = 1; i <= n; i++) {
        cin >> (s + 1);
        l = strlen(s + 1);
        solve(0, 0);
        ss[++len] = 'a' + 26;
        for (register int j = 1; j <= l; j++) ss[++len] = s[j];
    }
    ac_bfs();
    int pos = 0, now = 0;
    for (; pos < len; pos++) tag[now = f[now][ss[pos + 1] - 'a']]++;
    for (register int i = tail; i; i--) {
        tag[fail[line[i]]] += tag[line[i]];
        for (int k = first[line[i]]; k; k = next[k]) ans[k] = tag[line[i]];
    }
    for (register int i = 1; i <= n; i++) cout << ans[i] << "\n";
    return 0;
}
#include <bits/stdc++.h>

using namespace std;

const int iol = 1024 * 1024;

char buf[iol], *s, *t, ioc;
bool iosig;

inline char read() {
    if (s == t) {
        t = (s = buf) + fread(buf, 1, iol, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
inline bool read(T& x) {
    iosig = false;
    for (ioc = read(); !isdigit(ioc); ioc = read()) {
        if (ioc == -1) return false;
        if (ioc == '-') iosig = true;
    }
    x = 0;
    while (ioc == '0') ioc = read();
    for (; isdigit(ioc); ioc = read()) x = (x << 1) + (x << 3) + (ioc ^ '0');
    s--;
    if (iosig) x = -x;
    return true;
}

vector<int> edge[10100];
bool vis[2002000];
int match[2002000];

stack<int> st;

bool dfs(int u) {
    for (register int i = 0; i < edge[u].size(); i++) {
        register int v = edge[u][i];
        if (!vis[v]) {
            vis[v] = true;
            st.push(v);
            if (!match[v] || dfs(match[v])) {
                match[v] = u;
                return true;
            }
        }
    }
    return false;
}

inline int hungary(int n) {
    register int ans = 0;
    for (register int i = 1; i <= n; i++) {
        memset(vis, 0, sizeof(vis));
        if (dfs(i)) ans++;
    }
    return ans;
}

int n;
int main() {
    read(n);
    for (register int i = 0, u, v; i < n; i++) {
        read(u), read(v);
        edge[u].push_back(i);
        edge[v].push_back(i);
    }

    register int i;

    for (i = 1; i <= 10000; i++) {
        while (!st.empty()) {
            match[st.top()] = false;
            st.pop();
        }
        if (!dfs(i)) break;
    }
    cout << i - 1;
    return 0;
}
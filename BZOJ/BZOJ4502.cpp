/*
 * created by xehoth on 26-02-2017
 */
#include <bits/stdc++.h>

#define long long long

const int MAXN = 10010;
const int MAXM = 33;

typedef char byte;

struct Node {
    Node *next[26], *fail;
    int deep;
    byte f[26];
} * root, pool[MAXN * MAXM], *cur = pool;

int maxLen;

inline void insert(const char *s) {
    register int len = strlen(s), index;
    Node *p = root;
    ::maxLen = std::max(::maxLen, len);
    for (register int i = 0; i < len; i++) {
        index = s[i] - 'a';
        if (!p->next[index]) p->next[index] = cur++;
        p->f[index] = 1, p = p->next[index];
    }
}

long f[MAXM][MAXM * MAXN];

inline void build() {
    Node *p, *next;
    std::queue<Node *> q;
    q.push(root);
    while (!q.empty()) {
        p = q.front(), q.pop();
        for (register int i = 0; i < 26; i++) {
            if (p->next[i]) {
                next = p->fail;
                while (next && !next->next[i]) next = next->fail;
                p->next[i]->fail = (next ? next->next[i] : root);
                p->next[i]->deep = p->deep + 1, q.push(p->next[i]);
            }
        }
        for (register int i = 0; i < 26; i++) {
            if (!p->next[i]) {
                next = p;
                while (next && !next->next[i]) next = next->fail;
                p->next[i] = root;
                if (next) p->next[i] = next->next[i];
            }
        }
    }
}

inline void query() {
    long ans = 0;
    for (Node *p = pool + 1; p != cur; p++)
        if (p->fail != root) ans++;
    for (Node *p = pool + 1; p != cur; p++) {
        for (register int i = 0; i < 26; i++) {
            if (!p->f[i] && p->next[i] != root) {
                f[1][p->next[i] - pool]++;
            }
        }
    }
    for (register int i = 1; i <= maxLen; i++) {
        for (register int j = 1; j < cur - pool; j++) {
            if (f[i][j]) {
                ans += f[i][j];
                Node *now = pool + j;
                for (register int k = 0; k < 26; k++) {
                    if (now->f[k] || now->next[k]->deep >= i + 1)
                        f[i + 1][now->next[k] - pool] += f[i][j];
                }
            }
        }
    }
    std::cout << ans;
}

int main() {
    register int n;
    scanf("%d", &n);
    root = cur++;

    for (register int i = 1; i <= n; i++) {
        static char str[MAXM];
        scanf("%s", str), insert(str);
    }
    build();
    query();
    return 0;
}

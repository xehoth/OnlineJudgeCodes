/**
 * Copyright (c) 2016, xehoth
 * All rights reserved.
 * 「BZOJ 1095」20-12-2016
 *
 * @author xehoth
 */
#include <bits/stdc++.h>
static const int IO_LEN = 65536 / 2;
inline char read() {
    static char buf[IO_LEN], *ioh, *iot;
    if (iot == ioh) {
        iot = (ioh = buf) + fread(buf, 1, IO_LEN, stdin);
        if (iot == ioh) return -1;
    }
    return *ioh++;
}
inline void read(int &x) {
    static char ioc;
    for (ioc = read(); !isdigit(ioc); ioc = read())
        ;
    for (x = 0; isdigit(ioc); ioc = read())
        x = (x << 1) + (x << 3) + (ioc ^ '0');
}
char _buf1[IO_LEN + 1], *S1 = _buf1;
inline void fwriteChar(char c) {
    if (S1 == _buf1 + IO_LEN) {
        fwrite(_buf1, 1, IO_LEN, stdout);
        S1 = _buf1;
    }
    *S1++ = c;
}
inline void flushIO() { fwrite(_buf1, 1, S1 - _buf1, stdout); }
inline void fwriteInt(int x) {
    if (x > 9) fwriteInt(x / 10);
    fwriteChar(x % 10 ^ '0');
}
const int MAXN = 100005;
struct PriorityQueue {
    std::priority_queue<int> heap, deleteMark;
    inline void insert(const int x) { heap.push(x); }
    inline void erase(const int x) { deleteMark.push(x); }
    inline void pop() {
        while (deleteMark.size() && heap.top() == deleteMark.top())
            heap.pop(), deleteMark.pop();
        heap.pop();
    }
    inline int top() {
        while (deleteMark.size() && heap.top() == deleteMark.top())
            heap.pop(), deleteMark.pop();
        return heap.top();
    }
    inline int secondTop() {
        register int tmp = top();
        pop();
        register int ret = top();
        insert(tmp);
        return ret;
    }
    inline int size() { return heap.size() - deleteMark.size(); }
} s1[MAXN], s2[MAXN], ans;
struct Edge {
    int to, next;
    bool valid;
} edge[MAXN << 1];
int head[MAXN], tot = 1;
int n, m, cnt;
int father[MAXN];
bool status[MAXN];
int logTwo[MAXN << 1], dpt[MAXN], pos[MAXN], dp[MAXN << 1][20], T;
inline void addEdge(const int x, const int y) {
    edge[++tot].to = y;
    edge[tot].next = head[x];
    head[x] = tot;
}
inline int getSize(const int x, const int from) {
    register int i, ret = 1;
    for (i = head[x]; i; i = edge[i].next) {
        if (edge[i].valid || edge[i].to == from) continue;
        ret += getSize(edge[i].to, x);
    }
    return ret;
}
inline int getCentreOfGravity(const int x, const int from, const int size,
                              int &cg) {
    register int i, ret = 1, flag = true;
    for (i = head[x]; i; i = edge[i].next) {
        if (edge[i].valid || edge[i].to == from) continue;
        register int tmp = getCentreOfGravity(edge[i].to, x, size, cg);
        if (tmp << 1 > size) flag = false;
        ret += tmp;
    }
    if (size - ret << 1 > size) flag = false;
    if (flag) cg = x;
    return ret;
}
inline void dfs(const int x, const int from, const int dpt, PriorityQueue &s) {
    s.insert(dpt);
    for (register int i = head[x]; i; i = edge[i].next) {
        if (edge[i].valid || edge[i].to == from) continue;
        dfs(edge[i].to, x, dpt + 1, s);
    }
}
inline void insert(PriorityQueue &s) {
    if (s.size() >= 2) {
        register int tmp = s.top() + s.secondTop();
        ans.insert(tmp);
    }
}
inline void erase(PriorityQueue &s) {
    if (s.size() >= 2) {
        register int tmp = s.top() + s.secondTop();
        ans.erase(tmp);
    }
}
inline int solve(int x) {
    register int i, size = getSize(x, 0), cg;
    getCentreOfGravity(x, 0, size, cg);
    s2[cg].insert(0);
    for (i = head[cg]; i; i = edge[i].next) {
        if (!edge[i].valid) {
            edge[i].valid = edge[i ^ 1].valid = true;
            PriorityQueue s;
            dfs(edge[i].to, 0, 1, s);
            register int tmp = solve(edge[i].to);
            father[tmp] = cg;
            s1[tmp] = s;
            s2[cg].insert(s1[tmp].top());
        }
    }
    insert(s2[cg]);
    return cg;
}
inline void dfs(int x, int from) {
    dp[pos[x] = ++T][0] = dpt[x] = dpt[from] + 1;
    for (register int i = head[x]; i; i = edge[i].next) {
        if (edge[i].to != from) {
            dfs(edge[i].to, x);
            dp[++T][0] = dpt[x];
        }
    }
}
inline int lcaDis(int x, int y) {
    x = pos[x];
    y = pos[y];
    if (x > y) std::swap(x, y);
    int L = logTwo[y - x + 1];
    return std::min(dp[x][L], dp[y - (1 << L) + 1][L]);
}
inline int distance(int x, int y) { return dpt[x] + dpt[y] - 2 * lcaDis(x, y); }
inline void turnOn(int x) {
    register int i;
    erase(s2[x]);
    s2[x].insert(0);
    insert(s2[x]);
    for (i = x; father[i]; i = father[i]) {
        erase(s2[father[i]]);
        if (s1[i].size()) s2[father[i]].erase(s1[i].top());
        s1[i].insert(distance(father[i], x));
        if (s1[i].size()) s2[father[i]].insert(s1[i].top());
        insert(s2[father[i]]);
    }
}
inline void turnOff(int x) {
    int i;
    erase(s2[x]);
    s2[x].erase(0);
    insert(s2[x]);
    for (i = x; father[i]; i = father[i]) {
        erase(s2[father[i]]);
        if (s1[i].size()) s2[father[i]].erase(s1[i].top());
        s1[i].erase(distance(father[i], x));
        if (s1[i].size()) s2[father[i]].insert(s1[i].top());
        insert(s2[father[i]]);
    }
}
int main() {
    register int i, j, x, y;
    static char p;
    read(n);
    cnt = n;
    for (i = 1; i < n; i++) {
        read(x), read(y);
        addEdge(x, y);
        addEdge(y, x);
    }
    solve(1);
    dfs(1, 0);
    for (i = 2; i <= T; i++) logTwo[i] = logTwo[i >> 1] + 1;
    for (j = 1; j <= logTwo[T]; j++)
        for (i = 1; i + (1 << j) - 1 <= T; i++)
            dp[i][j] = std::min(dp[i][j - 1], dp[i + (1 << j - 1)][j - 1]);
    for (i = 1; i <= n; i++) status[i] = true;
    read(m);
    for (i = 1; i <= m; i++) {
        p = read();
        while (!isalpha(p)) p = read();
        if (p == 'G') {
            if (cnt <= 1)
                fwriteInt(cnt - 1), fwriteChar('\n');
            else
                fwriteInt(ans.top()), fwriteChar('\n');
        } else {
            read(x);
            if (status[x] == true) {
                --cnt;
                status[x] = false;
                turnOff(x);
            } else {
                ++cnt;
                status[x] = true;
                turnOn(x);
            }
        }
    }
    flushIO();
    return 0;
}
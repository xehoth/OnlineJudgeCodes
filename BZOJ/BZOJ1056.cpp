/**
 * Copyright (c) 2016, xehoth
 * All rights reserved.
 * 「BZOJ 1056」26-12-2016
 *
 * @author xehoth
 */
#include <bits/stdc++.h>
using std::min;
using std::pair;
using std::string;
typedef unsigned long long ull;
const int MAXN = 260000;
struct SizeBalanceTree {
    struct Node {
        int v, s, t, id;
        Node *ch[2];
        inline void init(int _v = 0, int _s = 0, int _t = 0, int _id = 0,
                         Node *p = NULL) {
            ch[0] = ch[1] = p, v = _v, s = _s, t = _t, id = _id;
        }
        inline void pushUp() { s = ch[0]->s + ch[1]->s + 1; }
        inline int cmp(int _v, int _t) const {
            if (v == _v) return t == _t ? -1 : _t > t;
            return v > _v;
        }
    };
    Node stack[MAXN];
    Node *root, *null, *tail;
    Node *store[MAXN];
    int top;
    inline void init() {
        tail = &stack[0], null = tail++, null->init(), root = null, top = 0;
    }
    inline Node *newNode(int v, int t, int id) {
        Node *p = null;
        if (top)
            p = store[--top];
        else
            p = tail++;
        p->init(v, 1, t, id, null);
        return p;
    }
    inline void rotate(Node *&x, int d) {
        Node *k = x->ch[!d];
        x->ch[!d] = k->ch[d], k->ch[d] = x, k->s = x->s, x->pushUp(), x = k;
    }
    inline void maintain(Node *&x, int d) {
        if (x->ch[d] == null) return;
        if (x->ch[d]->ch[d]->s > x->ch[!d]->s)
            rotate(x, !d);
        else if (x->ch[d]->ch[!d]->s > x->ch[!d]->s)
            rotate(x->ch[d], d), rotate(x, !d);
        else
            return;
        maintain(x, 0), maintain(x, 1);
    }
    inline void insert(Node *&x, int v, int t, int id) {
        if (x == null) {
            x = newNode(v, t, id);
            return;
        } else {
            x->s++;
            int d = x->cmp(v, t);
            insert(x->ch[d], v, t, id), x->pushUp(), maintain(x, d);
        }
    }
    inline void erase(Node *&x, int v, int t) {
        if (x == null) return;
        x->s--;
        int d = x->cmp(v, t);
        if (-1 == d) {
            if (!x->ch[0]->s || !x->ch[1]->s) {
                store[top++] = x, x = x->ch[0]->s ? x->ch[0] : x->ch[1];
            } else {
                Node *ret = x->ch[1];
                for (; ret->ch[0] != null; ret = ret->ch[0])
                    ;
                x->v = ret->v, x->t = ret->t, x->id = ret->id;
                erase(x->ch[1], ret->v, ret->t);
            }
        } else {
            erase(x->ch[d], v, t);
        }
        if (x != null) x->pushUp();
    }
    inline int select(Node *x, int v, int t) {
        int k = 0, cur = 0;
        for (; x != null;) {
            int d = x->cmp(v, t);
            k = x->ch[0]->s;
            if (-1 == d)
                break;
            else if (!d)
                x = x->ch[0];
            else
                cur += k + 1, x = x->ch[1];
        }
        return cur + k + 1;
    }
    inline int rank(Node *x, int k) {
        for (; x != null;) {
            int t = x->ch[0]->s;
            if (k == t + 1)
                break;
            else if (k <= t)
                x = x->ch[0];
            else
                k -= t + 1, x = x->ch[1];
        }
        return x->id;
    }
    inline void insert(int v, int t, int id) { insert(root, v, t, id); }
    inline void erase(int v, int t) { erase(root, v, t); }
    inline int select(int v, int t) { return select(root, v, t); }
    inline int rank(int k) { return rank(root, k); }
} sbt;
#define BASE 133
#define MOD 299997
#define MAXN 500000
int now[MAXN], _time[MAXN];
struct HashSet {
    int head[MAXN];
    int tot, next[MAXN];
    ull hash[MAXN];
    char src[MAXN][12];
    inline ull getHash(char *s) {
        ull ret = 0;
        while (*s != '\0') ret = ret * BASE + *s++;
        return ret;
    }
    inline int insert(char *s) {
        ull _hash = getHash(s);
        int x = _hash % MOD;
        for (int i = head[x]; i; i = next[i])
            if (hash[i] == _hash) return i;
        next[++tot] = head[x], hash[tot] = _hash, head[x] = tot;
        strcpy(src[tot], s);
        return tot;
    }
} map;
int main() {
    int n, v;
    sbt.init();
    char s1[100], buf[100];
    scanf("%d\n", &n);
    for (int i = 1; i <= n; i++) {
        gets(buf);
        if (buf[0] == '+') {
            sscanf(&buf[1], "%s %d", s1, &v);
            int x = map.insert(s1);
            if (now[x]) sbt.erase(now[x], _time[x]);
            now[x] = v, _time[x] = i;
            sbt.insert(now[x], _time[x], x);
        } else if (buf[0] == '?' && isalpha(buf[1])) {
            sscanf(&buf[1], "%s", s1);
            int x = map.insert(s1);
            printf("%d\n", sbt.select(now[x], _time[x]));
        } else {
            int ed;
            sscanf(&buf[1], "%d", &v);
            ed = min(v + 9, map.tot);
            for (int j = v; j <= ed; j++) {
                printf("%s%c", map.src[sbt.rank(j)], j != ed ? ' ' : '\n');
            }
        }
    }
    return 0;
}

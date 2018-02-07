#include <bits/stdc++.h>

#define long long long

const double alpha = 0.8;
const int MAXN = 100005, MAXH = 80;

struct Node {
    Node *lc, *rc;
    int rank, key, size;
    inline void maintain() {
        size = 1;
        if (lc) size += lc->size;
        if (rc) size += rc->size;
    }
} data[MAXN * MAXH * 2], *pool[MAXN * MAXH * 2], **top = pool;

inline void clear() {
    for (register int i = MAXN * MAXH * 2 - 1; i >= 0; i--) *top++ = data + i;
}

inline Node *newNode(int x = 0) {
    Node *cur = *--top;
    cur->lc = cur->rc = NULL;
    cur->rank = rand(), cur->key = x;
    cur->size = 1;
    return cur;
}

inline void erase(Node *&cur) {
    if (!cur) return;
    erase(cur->lc), erase(cur->rc);
    *top++ = cur, cur = NULL;
}

inline void insert(Node *&cur, int x) {
    if (!cur) {
        cur = newNode(x);
        return;
    }
    if (x < cur->key) {
        insert(cur->lc, x);
        if (cur->lc->rank > cur->rank) {
            Node *x = cur->lc, *y = cur;
            y->lc = x->rc, x->rc = y;
            y->maintain(), cur = x;
        }
    } else {
        insert(cur->rc, x);
        if (cur->rc->rank > cur->rank) {
            Node *x = cur->rc, *y = cur;
            y->rc = x->lc, x->lc = y;
            y->maintain(), cur = x;
        }
    }
    cur->maintain();
}

inline int rank(Node *cur, int x) {
    if (!cur) return 0;
    if (x < cur->key)
        return rank(cur->lc, x);
    else
        return (cur->lc ? cur->lc->size : 0) + 1 + rank(cur->rc, x);
}

int n;
long lastAns;

struct Edge {
    struct ScapegoatNode *scape;
    Edge *next;
    int w;
} edge[MAXN << 1], *curEdge = edge;

struct ScapegoatNode {
    Edge *son;
    ScapegoatNode *fa;
    int size, level;
    int rc, dist[MAXH];
    Node *s, *t;
    int sz;
} root[MAXN];

inline void addedge(ScapegoatNode *u, ScapegoatNode *v, int w) {
    *curEdge = (Edge){v, u->son, w}, u->son = curEdge++;
    *curEdge = (Edge){u, v->son, w}, v->son = curEdge++;
}

inline int dfs(ScapegoatNode *u, ScapegoatNode *from, int level, int d,
               Node *&t) {
    if (u->level < level) return 0;
    u->sz = 1, u->level = level + 1, u->dist[level] = d;
    insert(t, d - u->rc);
    for (Edge *e = u->son; e; e = e->next)
        if (e->scape != from) u->sz += dfs(e->scape, u, level, d + e->w, t);
    return u->sz;
}

inline ScapegoatNode *getScapegoatRoot(ScapegoatNode *u, int level) {
    while (true) {
        ScapegoatNode *v = NULL;
        for (Edge *e = u->son; e; e = e->next) {
            if (e->scape->level >= level && e->scape->sz * 2 > u->sz) {
                v = e->scape;
                break;
            }
        }
        if (!v) break;
        register int tmp = u->sz;
        u->sz -= v->sz, v->sz = tmp, u = v;
    }
    return u;
}

Node *tmpNode;

inline void build(ScapegoatNode *u, ScapegoatNode *fa, int size, int level,
                  Node *t) {
    erase(u->s);
    dfs(u, NULL, level, 0, u->s);
    erase(u->s);
    u = getScapegoatRoot(u, level);
    u->fa = fa, u->size = size;
    erase(u->s);
    dfs(u, NULL, level, 0, u->s);
    if (u->t != tmpNode) erase(u->t);
    u->t = t, u->level = level;
    for (Edge *e = u->son; e; e = e->next) {
        if (e->scape->level >= level) {
            ScapegoatNode *v = e->scape;
            Node *tmp = NULL;
            dfs(v, u, level + 1, e->w, tmp);
            build(v, u, v->sz, level + 1, tmp);
        }
    }
}

const int IN_LEN = 1000000, OUT_LEN = 1000000;
inline int nextChar() {
    static char buf[IN_LEN], *h, *t;
    if (h == t) {
        t = (h = buf) + fread(buf, 1, IN_LEN, stdin);
        if (h == t) return -1;
    }
    return *h++;
}

template <class T>
inline bool read(T &x) {
    static bool iosig = 0;
    static char c;
    for (iosig = 0, c = nextChar(); !isdigit(c); c = nextChar()) {
        if (c == -1) return false;
        if (c == '-') iosig = 1;
    }
    for (x = 0; isdigit(c); c = nextChar()) x = (x << 1) + (x << 3) + (c ^ '0');
    if (iosig) x = -x;
    return true;
}

char obuf[OUT_LEN], *oh = obuf;
inline void writeChar(const char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
inline void write(T x) {
    static int buf[30], cnt;
    if (!x)
        writeChar(48);
    else {
        if (x < 0) writeChar('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) writeChar(buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

int main() {
    read(n), clear(), read(n);
    read(root[1].rc), read(root[1].rc), read(root[1].rc);
    root[1].size = 1, root[1].level = 0, root[1].dist[0] = 0, root[1].s = NULL;

    insert(root[1].s, -root[1].rc);
    root[1].t = NULL;

    writeChar('0'), writeChar('\n');

    for (ScapegoatNode *u = root + 2; u <= root + n; u++) {
        register int f, w, rc;
        read(f), read(w), read(rc);
        f ^= lastAns % int(1e9);
        ScapegoatNode *v = root + f;

        addedge(u, v, w);
        u->fa = v, u->size = 0, u->level = v->level + 1, u->rc = rc;
        for (register int j = 0; j < u->level; j++) u->dist[j] = v->dist[j] + w;

        u->dist[u->level] = 0, u->s = NULL, u->t = NULL;
        ScapegoatNode *toreBuild = NULL;

        for (v = u; v; v = v->fa) {
            v->size++;
            lastAns += rank(v->s, u->rc - u->dist[v->level]);
            insert(v->s, u->dist[v->level] - u->rc);
            if (v->fa) {
                lastAns -= rank(v->t, u->rc - u->dist[v->level - 1]);
                insert(v->t, u->dist[v->level - 1] - u->rc);
                if (v->size > (v->fa->size + 1) * alpha) toreBuild = v->fa;
            }
        }
        if (toreBuild) {
            tmpNode = toreBuild->t;
            build(toreBuild, toreBuild->fa, toreBuild->size, toreBuild->level,
                  toreBuild->t);
        }
        write(lastAns), writeChar('\n');
    }

    flush();
    return 0;
}

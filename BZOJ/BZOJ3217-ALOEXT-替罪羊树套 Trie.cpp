/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3217」ALOEXT 21-09-2017
 * 替罪羊树套 Trie
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
}

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = 0;
    return s;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

    ~InputOutputStream() { flush(); }
} io;
}

namespace PersistentTrie {

const int MAX_LOG = 20;
const int MAXN = 200010;

struct Node {
    Node *c[2];
    int v;

    inline void *operator new(size_t);
    inline void operator delete(void *);

    Node(int);
    Node();
} pool[MAXN * MAX_LOG * 6 + 1], *cur = pool + 1, *null = pool,
                                *bin[MAXN * MAX_LOG * 6 + 1];

int binTop;

inline void *Node::operator new(size_t) {
    return binTop ? bin[--binTop] : cur++;
}

inline void Node::operator delete(void *p) {
    if ((Node *)p == null) return;
    bin[binTop++] = (Node *)p;
    delete ((Node *)p)->c[0];
    ((Node *)p)->c[0] = null;
    delete ((Node *)p)->c[1];
    ((Node *)p)->c[1] = null;
}

Node::Node() { c[0] = c[1] = null, v = 0; }

Node::Node(int v) : v(v) { c[0] = c[1] = null; }

struct PersistentTrie {
    Node *root;

    inline void insert(Node *&p, int val, int dep = MAX_LOG - 1) {
        if (p == null) p = new Node();
        p->v++;
        if (dep < 0) return;
        insert(p->c[val >> dep & 1], val, dep - 1);
    }

    inline void insert(int val) { insert(root, val); }

    inline void erase(Node *&p, int val, int dep = MAX_LOG - 1) {
        p->v--;
        if (p->v == 0) {
            delete p;
            p = null;
            return;
        }
        if (dep < 0) return;
        erase(p->c[val >> dep & 1], val, dep - 1);
    }

    inline void erase(int val) { erase(root, val); }

    inline Node *merge(Node *u, Node *v) {
        if (u == null && v == null) return null;
        Node *p = new Node(u->v + v->v);
        p->c[0] = merge(u->c[0], v->c[0]);
        p->c[1] = merge(u->c[1], v->c[1]);
        return p;
    }

    inline Node *merge(PersistentTrie &t) { return merge(root, t.root); }

    PersistentTrie() : root(null) {}
};
}

namespace ScapeGoatTree {

struct Query {
    int v;
    PersistentTrie::Node *p;

    Query(int v = 0, PersistentTrie::Node *p = PersistentTrie::null)
        : v(v), p(p) {}
};

using PersistentTrie::MAX_LOG;
using PersistentTrie::PersistentTrie;

const int MAXN = 200010;
const double ALPHA = 0.8;

struct Node {
    Node *c[2];
    int v, size, cover, del;
    PersistentTrie trie;

    inline void *operator new(size_t);
    inline void operator delete(void *);

    Node();
    Node(int);

    inline bool check() {
        return std::max(c[0]->cover, c[1]->cover) > ALPHA * cover;
    }

    inline void maintain() {
        size = c[0]->size + c[1]->size + (!del);
        cover = c[0]->cover + c[1]->cover + 1;
    }
} pool[MAXN + 1], *null = pool, *cur = pool + 1, *bin[MAXN + 1];

int binTop;

inline void *Node::operator new(size_t) {
    return binTop ? bin[--binTop] : cur++;
}

inline void Node::operator delete(void *p) {
    if ((Node *)p == null) return;
#ifdef DBG
    std::cerr << "delete " << (Node *)p - pool << " " << binTop << std::endl;
#endif
    bin[binTop++] = (Node *)p;
    delete ((Node *)p)->c[0];
    ((Node *)p)->c[0] = null;
    delete ((Node *)p)->c[1];
    ((Node *)p)->c[1] = null;
    delete ((Node *)p)->trie.root;
    ((Node *)p)->trie.root = ::PersistentTrie::null;
}

Node::Node() : trie() { c[0] = c[1] = null, v = 0, size = cover = del = 0; }

Node::Node(int v) : v(v), size(1), cover(1), del(0), trie() {
    c[0] = c[1] = null;
}

struct ScapeGoatTree {
    Node *root;
    std::vector<int> v;

    inline Node **insert(Node *&p, int pos, int val) {
        if (p == null) {
            p = new Node(val);
            p->trie.insert(val);
            return &null;
        } else {
            p->size++, p->cover++;
            p->trie.insert(val);
            Node **res;
            if (pos <= p->c[0]->size + (!p->del))
                res = insert(p->c[0], pos, val);
            else
                res = insert(p->c[1], pos - p->c[0]->size - (!p->del), val);
            if (p->check()) res = &p;
            return res;
        }
    }

    inline void travel(Node *p, std::vector<int> &v) {
        if (p == null) return;
        travel(p->c[0], v), v.push_back(p->del ? -1 : p->v), travel(p->c[1], v);
    }

    inline void divide(Node *&p, std::vector<int> &v, int l, int r) {
        register int mid = l + r >> 1;
        p = new Node(v[mid]);
        if (p->v == -1) p->del = true;
        if (l < mid) divide(p->c[0], v, l, mid - 1);
        if (mid < r) divide(p->c[1], v, mid + 1, r);
        p->maintain();
        p->trie.root = p->c[0]->trie.merge(p->c[1]->trie);
        if (!p->del) p->trie.insert(p->v);
    }

    inline void rebuild(Node *&p) {
        v.clear(), v.push_back(-1), travel(p, v);
        delete p, p = null;
        divide(p, v, 1, v.size() - 1);
    }

    inline void insert(int pos, int val) {
        Node **p = insert(root, pos + 1, val);
        if (*p != null) rebuild(*p);
    }

    inline int modify(Node *p, int pos, int val) {
        register int ret = 0;
        if (!p->del && pos == p->c[0]->size + 1) {
            ret = p->v, p->v = val;
            p->trie.insert(val);
            p->trie.erase(ret);
            return ret;
        }
        if (pos <= p->c[0]->size)
            ret = modify(p->c[0], pos, val);
        else
            ret = modify(p->c[1], pos - p->c[0]->size - (!p->del), val);
        p->trie.insert(val), p->trie.erase(ret);
        return ret;
    }

    inline int erase(Node *p, int pos) {
        p->size--;
        if (!p->del && pos == p->c[0]->size + 1) {
            p->del = true;
            p->trie.erase(p->v);
            return p->v;
        }
        register int ret = 0;
        if (pos <= p->c[0]->size)
            ret = erase(p->c[0], pos);
        else
            ret = erase(p->c[1], pos - p->c[0]->size - (!p->del));
        p->trie.erase(ret);
        return ret;
    }

    inline void modify(int pos, int val) { modify(root, pos + 1, val); }

    inline void erase(int pos) { erase(root, pos + 1); }

    Query q[MAXN + 1], buc[MAXN + 1];
    int qcnt;

    inline void dfs(Node *p, int pos) {
        if (pos == 0) return;
        if (pos == p->c[0]->size) {
            q[++qcnt] = Query(1, p->c[0]->trie.root);
        } else if (pos < p->c[0]->size) {
            dfs(p->c[0], pos);
        } else {
            q[++qcnt] = Query(1, p->trie.root);
            q[++qcnt] = Query(-1, p->c[1]->trie.root);
            dfs(p->c[1], pos - p->c[0]->size - (!p->del));
        }
    }

    inline int query(int l, int r) {
        l++, r++, qcnt = 0;
        dfs(root, l - 1);
        for (register int i = 1; i <= qcnt; i++) q[i].v = -q[i].v;
        dfs(root, r);
        memcpy(buc + 1, q + 1, qcnt * sizeof(Query));
        register int ret = 0, k = 2;
        register int totl = 0, totr = 0;
        register int L = 0, R = (1 << MAX_LOG) - 1;
        for (register int dep = MAX_LOG - 1; dep >= 0; dep--) {
            totl = totr = 0;
            for (register int i = 1; i <= qcnt; i++) {
                totr += q[i].v * q[i].p->c[1]->v;
            }
            if (totr < k) {
                k -= (int)totr, R -= 1 << dep;
                for (register int i = 1; i <= qcnt; i++) q[i].p = q[i].p->c[0];
            } else {
                ret += 1 << dep, L += 1 << dep;
                for (register int i = 1; i <= qcnt; i++) q[i].p = q[i].p->c[1];
            }
        }
        memcpy(q + 1, buc + 1, qcnt * sizeof(Query));
        register int ans = 0;
        for (register int dep = MAX_LOG - 1; dep >= 0; dep--) {
            totl = 0, totr = 0;
            for (register int i = 1; i <= qcnt; i++) {
                totl += q[i].v * q[i].p->c[0]->v;
                totr += q[i].v * q[i].p->c[1]->v;
            }
            if (!totl || (totr && ((ret >> dep) & 1) == 0)) {
                ans += 1 << dep;
                for (register int i = 1; i <= qcnt; i++) q[i].p = q[i].p->c[1];
            } else {
                for (register int i = 1; i <= qcnt; i++) q[i].p = q[i].p->c[0];
            }
        }
#ifdef DBG
        std::cerr << "now ans = " << ans << " query ans = " << ret
                  << " xor ans = " << (ans ^ ret) << std::endl;
#endif
        return ans ^ ret;
    }

    inline void init(const int n) {
        using IO::io;
        v.resize(n + 1);
        v[0] = -1;
        for (register int i = 1; i <= n; i++) io >> v[i];
        divide(root, v, 1, v.size() - 1);
    }
};
}

namespace {

ScapeGoatTree::ScapeGoatTree task;
const int MOD = 1048576;

inline void solve() {
    using IO::io;
    register int n, m;
    io >> n >> m;
    task.init(n);
    register int lastans = 0, n0 = n;
    register char cmd;
    for (register int i = 1, a, b; i <= m; i++) {
        io >> cmd;
        switch (cmd) {
            case 'I':
                io >> a >> b;
                a = (a + lastans) % n0;
                b = (b + lastans) % MOD;
                task.insert(a, b);
                n0++;
                break;
            case 'D':
                io >> a;
                a = (a + lastans) % n0;
                task.erase(a);
                n0--;
                break;
            case 'C':
                io >> a >> b;
                a = (a + lastans) % n0;
                b = (b + lastans) % MOD;
                task.modify(a, b);
                break;
            case 'F':
                io >> a >> b;
                a = (a + lastans) % n0;
                b = (b + lastans) % n0;
                io << (lastans = task.query(a, b)) << '\n';
                break;
        }
    }
}
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif

    solve();
    return 0;
}
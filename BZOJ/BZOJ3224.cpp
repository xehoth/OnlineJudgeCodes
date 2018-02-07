#include <bits/stdc++.h>

namespace IO {

const int IN_LEN = 1000000;
char ibuf[IN_LEN], *ih = ibuf;

template <typename T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = *ih++, iosig = false; !isdigit(c); c = *ih++)
        c == '-' ? iosig = true : 0;
    for (x = 0; isdigit(c); c = *ih++) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 1024 * 1024 - 1;
char obuf[OUT_LEN], *oh = obuf;

template <typename T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        *oh++ = '0';
    } else {
        x < 0 ? (*oh++ = '-', x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) *oh++ = buf[cnt--];
    }
}
}  // namespace IO

const int MAXN = 100001;

struct Node {
    int data, s, c;
    bool color;
    Node *fa, *ch[2];

    Node() {}
    Node(const int v, const bool color, const int i, register Node *p)
        : data(v), color(color), s(i), c(i), fa(p) {
        ch[0] = ch[1] = p;
    }

    inline void maintain() { s = ch[0]->s + ch[1]->s + c; }

    inline void pushDown() {
        for (Node *x = this; x->s; x = x->fa) x->s--;
    }

    inline void *operator new(size_t);

    inline void operator delete(void *);

    inline int cmp(const int &v) const { return data == v ? -1 : v > data; }
} stack[MAXN], *tail = stack, *store[MAXN];

int top;

inline void *Node::operator new(size_t) { return !top ? tail++ : store[--top]; }

inline void Node::operator delete(void *p) { store[top++] = (Node *)p; }

using namespace IO;

struct RedBlackTree {
    Node *root, *null;

    inline void init() { root = null = new Node(0, 0, 0, NULL); }

    inline void rotate(register Node *&x, const bool &d) {
        register Node *y = x->ch[!d];
        x->ch[!d] = y->ch[d];
        (y->ch[d]->s) ? (y->ch[d]->fa = x) : 0;
        y->fa = x->fa;
        (!x->fa->s) ? (root = y) : (x->fa->ch[x->fa->ch[0] != x] = y);
        y->ch[d] = x, x->fa = y, y->s = x->s, x->maintain();
    }

    inline void insert(const int &v) {
        register Node *x = root, *y = null;
        while (x->s) {
            x->s++, y = x;
            register int d = x->cmp(v);
            if (-1 == d) {
                x->c++;
                return;
            }
            x = x->ch[d];
        }
        x = new Node(v, 1, 1, null);
        (y->s) ? (y->ch[v > y->data] = x) : (root = x);
        x->fa = y, insertFix(x);
    }

    inline void insertFix(register Node *&x) {
        while (x->fa->color) {
            Node *par = x->fa, *Gp = par->fa;
            register bool d = par == Gp->ch[0];
            Node *uncle = Gp->ch[d];
            if (uncle->color) {
                par->color = uncle->color = 0;
                Gp->color = 1;
                x = Gp;
            } else if (x == par->ch[d]) {
                rotate(x = par, !d);
            } else {
                Gp->color = 1;
                par->color = 0;
                rotate(Gp, d);
            }
        }
        root->color = 0;
    }

    inline Node *find(Node *x, int data) {
        while (x->s && x->data != data) x = x->ch[x->data < data];
        return x;
    }

    inline void eraseeteFix(register Node *&x) {
        while (x != root && !x->color) {
            register bool d = x == x->fa->ch[0];
            Node *par = x->fa, *sibling = par->ch[d];
            if (sibling->color) {
                sibling->color = 0;
                par->color = 1;
                rotate(x->fa, !d);
                sibling = par->ch[d];
            } else if (!sibling->ch[0]->color && !sibling->ch[1]->color) {
                sibling->color = 1, x = par;
            } else {
                if (!sibling->ch[d]->color) {
                    sibling->ch[!d]->color = 0;
                    sibling->color = 1;
                    rotate(sibling, d);
                    sibling = par->ch[d];
                }
                sibling->color = par->color;
                sibling->ch[d]->color = par->color = 0;
                rotate(par, !d);
                break;
            }
        }
        x->color = 0;
    }

    inline void erase(const int &data) {
        register Node *z = find(root, data);
        if (!z->s) return;
        if (z->c > 1) {
            z->c--;
            z->pushDown();
            return;
        }
        register Node *y = z, *x = null;
        if (z->ch[0]->s && z->ch[1]->s) {
            y = z->ch[1];
            while (y->ch[0]->s) y = y->ch[0];
        }
        x = y->ch[!y->ch[0]->s];
        x->fa = y->fa;
        if (!y->fa->s)
            root = x;
        else
            y->fa->ch[y->fa->ch[1] == y] = x;
        if (z != y) z->data = y->data, z->c = y->c;
        y->fa->pushDown();
        for (Node *k = y->fa; y->c > 1 && k->s && k != z; k = k->fa)
            k->s -= y->c - 1;
        if (!y->color) eraseeteFix(x);
        delete y;
    }

    inline void select(register int k) {
        register int t;
        register Node *x = root;
        for (; x->s;) {
            t = x->ch[0]->s;
            if (k <= t)
                x = x->ch[0];
            else if (t + 1 <= k && k <= t + x->c)
                break;
            else
                k -= t + x->c, x = x->ch[1];
        }
        print(x->data), *oh++ = '\n';
    }

    inline void rank(const int &v) {
        register int t, cur = 0;
        register Node *x = root;
        for (; x->s;) {
            t = x->ch[0]->s;
            if (v == x->data)
                break;
            else if (v < x->data)
                x = x->ch[0];
            else
                cur += t + x->c, x = x->ch[1];
        }
        print(cur + t + 1), *oh++ = '\n';
    }

    inline void succ(const int &v) {
        register int ret = 0;
        Node *x = root;
        while (x->s)
            (x->data > v) ? (ret = x->data, x = x->ch[0]) : (x = x->ch[1]);
        print(ret), *oh++ = '\n';
    }

    inline void pred(const int &v) {
        register int ret = 0;
        register Node *x = root;
        while (x->s)
            (x->data < v) ? (ret = x->data, x = x->ch[1]) : (x = x->ch[0]);
        print(ret), *oh++ = '\n';
    }
} rbt;

int main() {
    // freopen("in.in", "r", stdin);
    fread(ibuf, 1, IN_LEN, stdin);
    rbt.init();
    register int n, op, v;
    read(n);
    while (n--) {
        read(op), read(v);
        switch (op) {
            case 1:
                rbt.insert(v);
                break;
            case 2:
                rbt.erase(v);
                break;
            case 3:
                rbt.rank(v);
                break;
            case 4:
                rbt.select(v);
                break;
            case 5:
                rbt.pred(v);
                break;
            case 6:
                rbt.succ(v);
                break;
        }
    }
    fwrite(obuf, 1, oh - obuf, stdout);
    return 0;
}
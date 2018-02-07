#include <bits/stdc++.h>

int id[70001], pos[270], limit, ans, n, tmp[35001], x, y, val, m;
char op;

struct BlockNode {
    int d[70001], cnt[270];
    inline void insert(const int &x) { d[x]++, cnt[id[x]]++; }
    inline void erase(const int &x) { d[x]--, cnt[id[x]]--; }
} S;

struct Block {
    std::vector<int> v;
    BlockNode T;
    Block() {}
};

std::list<Block> list;
typedef std::list<Block>::iterator ListIterator;
typedef std::vector<int>::iterator VectorIterator;
typedef std::pair<ListIterator, VectorIterator> Pair;

inline Pair find(const int &p) {
    register int cnt = 0;
    ListIterator i = list.begin();
    for (; i != list.end(); i++) {
        cnt += i->v.size();
        if (cnt >= p) {
            cnt -= i->v.size();
            for (VectorIterator j = i->v.begin(); j != i->v.end(); j++)
                if ((++cnt) == p) return std::make_pair(i, j);
        }
    }
    i--;
    return std::make_pair(i, i->v.end());
}

inline void insert(const int &p, const int &V) {
    Pair bel = find(p);
    bel.first->v.insert(bel.second, V);
    for (ListIterator i = bel.first; i != list.end(); i++) i->T.insert(V);
    if (bel.first->v.size() > limit) {
        ListIterator near = bel.first;
        near++;
        register int last = bel.first->v.back();
        if (near->v.size() > limit || near == list.end()) {
            near = list.insert(near, Block());
            near->T = bel.first->T;
        }
        near->v.insert((*near).v.begin(), last);
        bel.first->T.erase(last);
        bel.first->v.pop_back();
    }
}

inline void update(const int &p, const int &V) {
    Pair bel = find(p);
    for (ListIterator i = bel.first; i != list.end(); i++)
        i->T.erase(*bel.second), i->T.insert(V);
    (*bel.second) = V;
}

inline int select(const int &L, const int &R, const int &K) {
    Pair P1 = find(L), P2 = find(R);
    register int cnt = 0, res;
    if (P1.first == P2.first) {
        for (VectorIterator i = P1.second; i <= P2.second; i++) S.insert(*i);
        for (register int i = 1;; i++) {
            cnt += S.cnt[i];
            if (cnt >= K) {
                cnt -= S.cnt[i];
                for (register int j = pos[i];; j++) {
                    cnt += S.d[j];
                    if (cnt >= K) {
                        res = j;
                        goto OUT2;
                    }
                }
            }
        }
    OUT2:
        for (VectorIterator i = P1.second; i <= P2.second; i++) S.erase(*i);
        return res;
    }
    for (VectorIterator i = P1.second; i != P1.first->v.end(); i++)
        S.insert(*i);
    for (VectorIterator i = P2.first->v.begin(); i <= P2.second; i++)
        S.insert(*i);
    ListIterator LB = P1.first, RB = P2.first;
    --RB;
    for (register int i = 1;; i++) {
        cnt += (RB->T.cnt[i] - LB->T.cnt[i] + S.cnt[i]);
        if (cnt >= K) {
            cnt -= (RB->T.cnt[i] - LB->T.cnt[i] + S.cnt[i]);
            for (register int j = pos[i];; j++) {
                cnt += (RB->T.d[j] - LB->T.d[j] + S.d[j]);
                if (cnt >= K) {
                    res = j;
                    goto OUT1;
                }
            }
        }
    }
OUT1:
    for (VectorIterator i = P1.second; i != P1.first->v.end(); i++) S.erase(*i);
    for (VectorIterator i = P2.first->v.begin(); i <= P2.second; i++)
        S.erase(*i);
    return res;
}

inline void initBlockNode() {
    register int tot = 1, sz = 265;
    if (!sz) sz = 1;
    for (; tot * sz <= 70000; tot++) {
        pos[tot] = (tot - 1) * sz;
        register int R = tot * sz - 1;
        for (register int i = pos[tot]; i <= R; i++) id[i] = tot;
    }
    pos[tot] = (tot - 1) * sz;
    for (register int i = pos[tot]; i <= 70000; i++) id[i] = tot;
}

inline void initBlock() {
    register int sz = std::max((sqrt(n) + 1) * 4 / 3, 1.0), tot = 1;
    if (!sz) sz = 1;
    limit = sqrt(n + 35000);
    for (; tot * sz < n; tot++) {
        ListIterator end = list.insert(list.end(), Block());
        end->v.assign(tmp + (tot - 1) * sz + 1, tmp + tot * sz + 1);
    }
    ListIterator end = list.insert(list.end(), Block());
    end->v.assign(tmp + (tot - 1) * sz + 1, tmp + n + 1);
}

inline void init() {
    ListIterator i = list.begin();
    for (VectorIterator j = i->v.begin(); j != i->v.end(); j++) i->T.insert(*j);
    i++;
    for (ListIterator front = list.begin(); i != list.end(); i++, front++) {
        i->T = front->T;
        for (VectorIterator j = i->v.begin(); j != i->v.end(); j++)
            i->T.insert(*j);
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
    read(n);
    for (register int i = 1; i <= n; i++) read(tmp[i]);
    initBlock();
    initBlockNode();
    init();
    read(m);
    for (; m--;) {
        op = nextChar(), read(x);
        x ^= ans;
        if (op == 'Q') {
            read(y), read(val);
            y ^= ans;
            val ^= ans;
            write(ans = select(x, y, val)), writeChar('\n');
        } else if (op == 'M') {
            read(val);
            val ^= ans;
            update(x, val);
        } else {
            read(val);
            val ^= ans;
            insert(x, val);
        }
    }
    flush();
    return 0;
}
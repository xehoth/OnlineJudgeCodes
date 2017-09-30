/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3337」ORZJRY I 30-09-2017
 * 块状链表
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
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
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
}  // end IO

namespace {

#define long long long
const int MAXN = 100000;
int blockCount;
int blockSize;

typedef std::vector<int> Vector;

struct Block {
    Vector num, sorted;
    int delta, cover;
    bool rev;
    long sum;

    Block() : delta(0), cover(0), rev(false), sum(0) {}

    inline void pushDown() {
        if (rev) std::reverse(num.begin(), num.end()), rev = false;
        if (delta) {
            for (register int i = 0; i < num.size(); i++)
                num[i] += delta, sorted[i] += delta;
            sum += (long)delta * num.size();
            delta = 0;
        }
        if (cover) {
            for (register int i = 0; i < num.size(); i++)
                num[i] = sorted[i] = cover;
            sum = (long)cover * num.size();
            cover = 0;
        }
    }

    inline long getSum() {
        return cover ? (long)cover * num.size()
                     : sum + (long)delta * num.size();
    }

    inline long getMax() { return cover ? cover : sorted.back() + (long)delta; }

    inline long getMin() { return cover ? cover : sorted[0] + (long)delta; }
};

typedef std::list<Block> List;
typedef List::iterator Iterator;
List d;

inline void build(const int *a, const int n) {
    blockSize = std::max((sqrt(n) + 1) * 4 / 3, 1.0);
    blockCount = ceil((double)n / blockSize);
    d.resize(blockCount);
    register Iterator it = d.begin();
    for (register int i = 0, j = 0; i < blockCount; i++, it++) {
        j += blockSize;
        it->num.assign(a + j - blockSize, a + std::min(j, n));
        it->sorted.assign(a + j - blockSize, a + std::min(j, n));
        for (Vector::iterator v = it->num.begin(); v != it->num.end(); v++)
            it->sum += *v;
        std::sort(it->sorted.begin(), it->sorted.end());
    }
}

using IO::io;

inline void find(int cur, Iterator &it, int &pos) {
    for (it = d.begin(); (int)(cur - it->num.size()) >= 0 && it != d.end();)
        cur -= it->num.size(), it++;
    pos = cur;
    it->pushDown();
}

inline void split(Iterator cur, int pos) {
    if (pos == cur->num.size()) return;
    Iterator it = cur;
    it = d.insert(++it, Block());
    cur->pushDown();
    it->num.assign(cur->num.begin() + pos, cur->num.end());
    cur->num.erase(cur->num.begin() + pos, cur->num.end());
    cur->sorted.erase(cur->sorted.begin() + pos, cur->sorted.end());
    it->sorted.assign(it->num.begin(), it->num.end());
    for (Vector::iterator v = it->num.begin(); v != it->num.end(); v++)
        it->sum += *v;
    cur->sum -= it->sum;
    cur->sorted.assign(cur->num.begin(), cur->num.end());
    std::sort(it->sorted.begin(), it->sorted.end());
    std::sort(cur->sorted.begin(), cur->sorted.end());
}

inline void merge(Iterator cur, Iterator suc) {
    cur->pushDown();
    suc->pushDown();
    cur->num.insert(cur->num.end(), suc->num.begin(), suc->num.end());
    cur->sorted.insert(cur->sorted.end(), suc->sorted.begin(),
                       suc->sorted.end());
    cur->sum += suc->sum;
    std::sort(cur->sorted.begin(), cur->sorted.end());
    d.erase(suc);
}

inline void maintain(Iterator cur) {
    for (register Iterator tmp; cur != d.end(); cur++) {
        if (cur->num.size() > blockSize * 2) {
            split(cur, cur->num.size() / 2);
        } else if (cur->num.size() < blockSize / 2) {
            tmp = cur, tmp++;
            if (tmp == d.end())
                break;
            else if (tmp->num.size() + cur->num.size() <= blockSize * 2)
                merge(cur, tmp);
        }
    }
}

inline void insert(int x, int v) {
    register Iterator cur;
    register int pos;
    find(--x, cur, pos);
    cur->num.insert(cur->num.begin() + pos + 1, v);
    cur->sorted.insert(
        std::lower_bound(cur->sorted.begin(), cur->sorted.end(), v), v);
    cur->sum += v;
    maintain(cur);
}

inline void erase(int x) {
    register Iterator cur;
    register int pos, v;
    find(--x, cur, pos);
    cur->sum -= cur->num[pos];
    cur->sorted.erase(std::lower_bound(cur->sorted.begin(), cur->sorted.end(),
                                       cur->num[pos]));
    cur->num.erase(cur->num.begin() + pos);
    maintain(cur);
}

inline void reverse(int l, int r) {
    l--, r--;
    std::list<Block> tmp;
    Iterator L, R, p;
    register int posL, posR;
    find(l - 1, L, posL);
    if (posL + 1 > 0) split(L, posL + 1), L++;
    find(r, R, posR);
    split(R, posR + 1);
    tmp.splice(tmp.begin(), d, L, ++R);
    for (Iterator v = tmp.begin(); v != tmp.end(); v++) v->rev ^= 1;
    tmp.reverse();
    L = R;
    if (L != d.begin()) L--;
    d.splice(R, tmp);
    maintain(L);
}

inline void revolve(int l, int r, int k) {
    l--, r--;
    std::list<Block> tmp;
    Iterator L, R, p, K;
    register int posL, posR, posK;
    find(l - 1, L, posL);
    if (posL + 1 > 0) split(L, posL + 1), L++;
    find(r - k, K, posK);
    if (posK + 1 > 0) split(K, posK + 1), K++;
    find(r, R, posR);
    split(R, posR + 1);
    tmp.splice(tmp.begin(), d, K, ++R);
    p = L;
    if (p != d.begin())
        p--;
    else
        p = d.end();
    d.splice(L, tmp);
    maintain(p == d.end() ? d.begin() : p);
}

inline void add(int l, int r, int v) {
    l--, r--;
    Iterator L, R;
    register int posL, posR;
    find(l, L, posL), find(r, R, posR);
    if (L == R) {
        L->pushDown();
        for (register int i = posL; i <= posR; i++) L->num[i] += v;
        L->sum += (posR - posL + 1ll) * v;
        L->sorted.assign(L->num.begin(), L->num.end());
        std::sort(L->sorted.begin(), L->sorted.end());
    } else {
        if (posL > 0) {
            L->pushDown();
            for (register int i = posL; i < L->num.size(); i++) L->num[i] += v;
            L->sum += (L->num.size() - posL) * (long)v;
            L->sorted.assign(L->num.begin(), L->num.end());
            std::sort(L->sorted.begin(), L->sorted.end());
            L++;
        }
        for (; L != R; L++) {
            if (L->cover)
                L->cover += v;
            else
                L->delta += v;
        }
        R->pushDown();
        for (register int i = 0; i <= posR; i++) R->num[i] += v;
        R->sum += (posR + 1) * v;
        R->sorted.assign(R->num.begin(), R->num.end());
        std::sort(R->sorted.begin(), R->sorted.end());
    }
}

inline void cover(int l, int r, int v) {
    l--, r--;
    Iterator L, R;
    register int posL, posR;
    find(l, L, posL), find(r, R, posR);
    if (L == R) {
        L->pushDown();
        for (register int i = posL; i <= posR; i++)
            L->sum += v - L->num[i], L->num[i] = v;
        L->sorted.assign(L->num.begin(), L->num.end());
        std::sort(L->sorted.begin(), L->sorted.end());
    } else {
        if (posL > 0) {
            L->pushDown();
            for (register int i = posL; i < L->num.size(); i++)
                L->sum += v - L->num[i], L->num[i] = v;
            L->sorted.assign(L->num.begin(), L->num.end());
            std::sort(L->sorted.begin(), L->sorted.end());
            L++;
        }
        for (; L != R; L++) {
            if (L->delta) L->delta = 0;
            L->cover = v;
        }
        R->pushDown();
        for (register int i = 0; i <= posR; i++)
            R->sum += v - R->num[i], R->num[i] = v;
        R->sorted.assign(R->num.begin(), R->num.end());
        std::sort(R->sorted.begin(), R->sorted.end());
    }
}

inline long querySum(int l, int r) {
    l--, r--;
    Iterator L, R;
    register int posL, posR;
    register long ret = 0;
    find(l, L, posL), find(r, R, posR);
    if (L == R) {
        L->pushDown();
        for (register int i = posL; i <= posR; i++) ret += L->num[i];
        return ret;
    } else {
        if (posL > 0) {
            L->pushDown();
            for (register int i = posL; i < L->num.size(); i++)
                ret += L->num[i];
            L++;
        }
        for (; L != R; L++) {
            ret += L->getSum();
        }
        R->pushDown();
        for (register int i = 0; i <= posR; i++) ret += R->num[i];
        return ret;
    }
}

inline long queryDiff(int l, int r) {
    l--, r--;
    Iterator L, R;
    register int posL, posR;
    register long min = LLONG_MAX, max = LLONG_MIN;
    find(l, L, posL), find(r, R, posR);
    if (L == R) {
        L->pushDown();
        for (register int i = posL; i <= posR; i++) {
            min = std::min(min, (long)L->num[i]);
            max = std::max(max, (long)L->num[i]);
        }
        return max - min;
    } else {
        if (posL > 0) {
            L->pushDown();
            for (register int i = posL; i < L->num.size(); i++) {
                min = std::min(min, (long)L->num[i]);
                max = std::max(max, (long)L->num[i]);
            }
            L++;
        }
        for (; L != R; L++) {
            min = std::min(min, L->getMin());
            max = std::max(max, L->getMax());
        }
        R->pushDown();
        for (register int i = 0; i <= posR; i++) {
            min = std::min(min, (long)L->num[i]);
            max = std::max(max, (long)L->num[i]);
        }
        return max - min;
    }
}

inline long queryAbs(int l, int r, int v) {
    register long ret = LLONG_MAX;
    l--, r--;
    Iterator L, R;
    register int posL, posR;
    find(l, L, posL), find(r, R, posR);
    if (L == R) {
        L->pushDown();
        for (register int i = posL; i <= posR; i++)
            ret = std::min(ret, llabs(L->num[i] - v));
        return ret;
    } else {
        if (posL > 0) {
            L->pushDown();
            for (register int i = posL; i < L->num.size(); i++)
                ret = std::min(ret, llabs(L->num[i] - v));
            L++;
        }
        register int tmp, pos;
        for (; L != R; L++) {
            if (L->cover) {
                ret = std::min(ret, llabs(L->cover - v));
                continue;
            }
            tmp = v - L->delta;
            pos = std::lower_bound(L->sorted.begin(), L->sorted.end(), tmp) -
                  L->sorted.begin();
            if (pos < L->sorted.size())
                ret = std::min(ret, (long)L->sorted[pos] - tmp);
            if (pos) ret = std::min(ret, (long)tmp - L->sorted[pos - 1]);
        }
        R->pushDown();
        for (register int i = 0; i <= posR; i++)
            ret = std::min(ret, llabs(R->num[i] - v));
        return ret;
    }
}

inline long queryRank(int l, int r, int v) {
    l--, r--;
    Iterator L, R;
    register int posL, posR;
    register int ret = 0;
    find(l, L, posL), find(r, R, posR);
    if (L == R) {
        L->pushDown();
        for (register int i = posL; i <= posR; i++) ret += (L->num[i] < v);
        return ret;
    } else {
        if (posL > 0) {
            L->pushDown();
            for (register int i = posL; i < L->num.size(); i++)
                ret += (L->num[i] < v);
            L++;
        }
        register int tmp;
        for (; L != R; L++) {
            if (L->cover) {
                if (L->cover < v) ret += L->num.size();
                continue;
            }
            tmp = v - L->delta;
            ret += std::lower_bound(L->sorted.begin(), L->sorted.end(), tmp) -
                   L->sorted.begin();
        }
        R->pushDown();
        for (register int i = 0; i <= posR; i++) ret += (R->num[i] < v);
        return ret;
    }
}

inline int divide(std::vector<int> &val, int l, int r, int x) {
    l--;
    for (register int mid; r - l > 1;) {
        mid = l + r >> 1;
        if (val[mid] <= x)
            l = mid;
        else
            r = mid;
    }
    return l;
}

inline int check(Iterator L, Iterator R, int val) {
    Iterator ptr = L;
    register int cnt = 0, pos, tmp;
    if (R != d.end()) R++;
    do {
        if (ptr->cover) {
            if (ptr->cover <= val) cnt += ptr->sorted.size();
            ptr++;
            continue;
        }
        tmp = val - ptr->delta;
        pos = divide(ptr->sorted, 0, ptr->sorted.size(), tmp);
        cnt += pos + 1;
        ptr++;
    } while (ptr != R);
    return cnt;
}

inline long getGlobalMax() {
    register long max = LLONG_MIN;
    for (Iterator p = d.begin(); p != d.end(); p++)
        max = std::max(max, p->getMax());
    return max;
}

inline long querySelect(int left, int right, int k) {
    left--, right--;
    Iterator L, R, ptr;
    register int posL, posR, pos;
    register long l, r, mid;
    find(left - 1, L, posL);
    if (posL + 1 > 0) split(L, posL + 1), L++;
    find(right, R, posR);
    split(R, posR + 1);
    l = 0;
    r = getGlobalMax();
    while (l != r) {
        mid = (l + r) >> 1;
        if (check(L, R, mid) >= k)
            r = mid;
        else
            l = mid + 1;
    }
    maintain(L);
    return l;
}

#ifdef DBG
// for gdb
void debugPrint(const int x) { std::cerr << x << ' '; }

void showNum(Iterator it) {
    it->pushDown();
    std::for_each(it->num.begin(), it->num.end(), debugPrint);
    std::endl(std::cerr);
}

void showSorted(Iterator it) {
    it->pushDown();
    std::for_each(it->sorted.begin(), it->sorted.end(), debugPrint);
    std::endl(std::cerr);
}

void showAll(Iterator it) { showNum(it), showSorted(it); }

void showBlockListNum() {
    std::cerr << "showBlockListNum begin" << std::endl;
    for (Iterator it = d.begin(); it != d.end(); it++) showNum(it);
    std::cerr << "showBlockListNum done" << std::endl << std::endl;
}

void showBlockListSorted() {
    std::cerr << "showBlockListSorted begin" << std::endl;
    for (Iterator it = d.begin(); it != d.end(); it++) showSorted(it);
    std::cerr << "showBlockListSorted done" << std::endl << std::endl;
}

void showBlockListAll() {
    std::cerr << "showBlockListAll begin" << std::endl;
    for (Iterator it = d.begin(); it != d.end(); it++) showAll(it);
    std::cerr << "showBlockListAll done" << std::endl << std::endl;
}

Iterator getBlock(int k) {
    Iterator it = d.begin();
    for (register int i = 0; i < k; i++) it->pushDown(), it++;
    return it;
}
#endif

inline void solve() {
    register int n, m;
    io >> n;
    static int a[MAXN];
    for (register int i = 0; i < n; i++) io >> a[i];
    build(a, n);
    io >> m;
    for (register int cmd, x, y, v, k; m--;) {
        io >> cmd;
        switch (cmd) {
            case 1: {
                io >> x >> v;
                insert(x, v);
                break;
            }
            case 2: {
                io >> x;
                erase(x);
                break;
            }
            case 3: {
                io >> x >> y;
                reverse(x, y);
                break;
            }
            case 4: {
                io >> x >> y >> k;
                revolve(x, y, k);
                break;
            }
            case 5: {
                io >> x >> y >> v;
                add(x, y, v);
                break;
            }
            case 6: {
                io >> x >> y >> v;
                cover(x, y, v);
                break;
            }
            case 7: {
                io >> x >> y;
                io << querySum(x, y) << '\n';
                break;
            }
            case 8: {
                io >> x >> y;
                io << queryDiff(x, y) << '\n';
                break;
            }
            case 9: {
                io >> x >> y >> v;
                io << queryAbs(x, y, v) << '\n';
                break;
            }
            case 10: {
                io >> x >> y >> k;
                io << querySelect(x, y, k) << '\n';
                break;
            }
            case 11: {
                io >> x >> y >> v;
                io << queryRank(x, y, v) << '\n';
                break;
            }
        }
    }
}

#undef long
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    solve();
    return 0;
}

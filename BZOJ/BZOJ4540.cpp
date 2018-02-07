#include <bits/stdc++.h>

using namespace std;

#define final __attribute__((always_inline))
const int MAXN = 100000;
const int MAXLOGN = 17;
const int MAXM = 100000;

#define iol 1048576
char buf1[iol], *s, *t, ioc;
bool iosig;
final char read() {
    if (s == t) {
        t = (s = buf1) + fread(buf1, 1, iol, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
final bool read(T& x) {
    iosig = false;
    for (ioc = read(); !isdigit(ioc); ioc = read()) {
        if (ioc == -1) return false;
        if (ioc == '-') iosig = true;
    }
    x = 0;
    while (ioc == '0') ioc = read();
    for (;; ioc = read()) {
        if (!isdigit(ioc)) break;
        x = (x << 1) + (x << 3) + (ioc ^ '0');
    }
    if (iosig) x = -x;
    return true;
}

#define in(x, y) read(x), read(y)

struct Element {
    int val;
    Element *left, *right;
    long long sumLeft, sumRight;
    final bool operator<(const Element& x) const { return val < x.val; }
    final bool operator<=(const Element& x) const { return val <= x.val; }
} a[MAXN];

int n, m, logTable[MAXN + 1];
Element* st[MAXN][MAXLOGN + 1];
long long ans[MAXN];

int blockSize;
struct Query {
    int l, r, blockPos;
    long long* ans;
    final bool operator<(const Query& x) const {
        if (blockPos == x.blockPos) return r < x.r;
        return blockPos < x.blockPos;
    }
} Q[MAXM];

final Element* min(Element* a, Element* b) {
    if (!a) return b;
    if (!b) return a;
    return *a < *b ? a : b;
}

final void sparseTable() {
    for (register int i = 0; i < n - 1; i++) st[i][0] = min(&a[i], &a[i + 1]);
    st[n - 1][0] = &a[n - 1];
    for (register int j = 1; (1 << j) <= n; j++)
        for (register int i = 0; i < n; i++)
            if (i + (1 << (j - 1)) < n)
                st[i][j] = min(st[i][j - 1], st[i + (1 << (j - 1))][j - 1]);
    logTable[0] = -1;
    for (register int i = 1; i <= n; i++) logTable[i] = logTable[i >> 1] + 1;
}

final Element* rmq(const int l, const int r) {
    if (l == r)
        return &a[l];
    else {
        register int t = logTable[r - l];
        return min(st[l][t], st[r - (1 << t)][t]);
    }
}

final void prepare() {
    stack<Element*> s;
    s.push(&a[0]);
    for (register int i = 1; i < n; i++) {
        while (!s.empty() && a[i] <= *s.top()) s.pop();
        if (!s.empty())
            a[i].left = s.top();
        else
            a[i].left = NULL;
        s.push(&a[i]);
    }
    for (register int i = 0; i < n; i++) {
        Element* x = &a[i];
        if (x->left == NULL)
            x->sumLeft = 0;
        else
            x->sumLeft = x->left->sumLeft +
                         (x - x->left) * static_cast<long long>(x->val);
    }
    s.push(&a[n - 1]);

    for (int i = n - 2; i >= 0; i--) {
        while (!s.empty() && a[i] <= *s.top()) s.pop();
        if (!s.empty())
            a[i].right = s.top();
        else
            a[i].right = NULL;
        s.push(&a[i]);
    }

    for (register int i = n - 1; i >= 0; i--) {
        Element* x = &a[i];
        if (x->right == NULL)
            x->sumRight = 0;
        else
            x->sumRight = x->right->sumRight +
                          (x->right - x) * static_cast<long long>(x->val);
    }

    sparseTable();
    sort(Q, Q + m);
}

final long long expandRight(const int l, const int r) {
    Element* pos = rmq(l, r);
    return (pos - &a[l] + 1) * static_cast<long long>(pos->val) + a[r].sumLeft -
           pos->sumLeft;
}

final long long expandLeft(const int l, const int r) {
    Element* pos = rmq(l, r);
    return (&a[r] - pos + 1) * static_cast<long long>(pos->val) +
           a[l].sumRight - pos->sumRight;
}

final void mo() {
    register int l = 0, r = 0;
    long long ans = a[0].val;
    for (register int i = 0; i < m; i++) {
        const Query& q = Q[i];
        while (r < q.r) r++, ans += expandRight(l, r);
        while (l > q.l) l--, ans += expandLeft(l, r);
        while (r > q.r) ans -= expandRight(l, r), r--;
        while (l < q.l) ans -= expandLeft(l, r), l++;
        *q.ans = ans;
    }
}

final int main() {
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
#endif

    in(n, m);
    blockSize = floor(sqrt(n));
    for (register int i = 0; i < n; i++) read(a[i].val);
    for (register int i = 0; i < m; i++) {
        in(Q[i].l, Q[i].r);
        Q[i].l--, Q[i].r--;
        Q[i].blockPos = Q[i].l / blockSize;
        Q[i].ans = &ans[i];
    }
    prepare();
    mo();
    for (register int i = 0; i < m; i++) cout << ans[i] << "\n";

    return 0;
}

#include <iostream>
#include <cstdio>
#include <cctype>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };

    char ibuf[SIZE], obuf[SIZE], *s, *t, *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    InputOutputStream &operator>>(int &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ 48);
        if (iosig) x = -x;
        return *this;
    }

    void print(char c) {
        if (oh == obuf + SIZE) {
            fwrite(obuf, 1, SIZE, stdout);
            oh = obuf;
        }
        *oh++ = c;
    }

    void print(long long x) {
        static int buf[21], cnt;
        if (x != 0) {
            if (x < 0) {
                print('-');
                x = -x;
            }
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
            while (cnt) print((char)buf[cnt--]);
        } else {
            print('0');
        }
    }

    template <typename T>
    InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 100000;

int n, m, pos[MAXN + 3];

struct Node {
    Node *lc, *rc;
    int cnt;
};

const int MAXM = 90 * MAXN * sizeof(Node);
char memPool[MAXM];

void *operator new(size_t size) {
    static char *p = memPool + MAXM;
    return p -= size;
}

Node *d[MAXN + 3];

void insert(Node *&p, int l, int r, int x) {
    if (!p) p = new Node;
    p->cnt++;
    if (l == r) return;
    int mid = (l + r) >> 1;
    if (x <= mid)
        insert(p->lc, l, mid, x);
    else
        insert(p->rc, mid + 1, r, x);
}

void erase(Node *p, int l, int r, int x) {
    if (!p) return;
    p->cnt--;
    if (l == r) return;
    int mid = (l + r) >> 1;
    if (x <= mid)
        erase(p->lc, l, mid, x);
    else
        erase(p->rc, mid + 1, r, x);
}

int query(Node *p, int l, int r, int s, int t) {
    if (!p) return 0;
    if (s <= l && t >= r) return p->cnt;
    int mid = (l + r) >> 1, ret = 0;
    if (s <= mid) ret += query(p->lc, l, mid, s, t);
    if (t > mid) ret += query(p->rc, mid + 1, r, s, t);
    return ret;
}

void insert(int k, int x) {
    for (; k <= n; k += k & -k) insert(d[k], 1, n, x);
}

void erase(int k, int x) {
    for (; k <= n; k += k & -k) erase(d[k], 1, n, x);
}

long long query(int k, int l, int r) {
    long long ret = 0;
    for (; k; k ^= k & -k) ret += query(d[k], 1, n, l, r);
    return ret;
}

int main() {
    io >> n >> m;
    long long ans = 0;
    for (int i = 1, x; i <= n; i++) {
        io >> x;
        pos[x] = i;
        insert(i, x);
        if (x != n) ans += query(i, x + 1, n);
    }

    for (int i = 1, x; i <= m; i++) {
        io >> x;
        io << ans << '\n';
        if (x != n) ans -= query(pos[x], x + 1, n);
        if (x != 1) ans -= query(n, 1, x - 1) - query(pos[x] - 1, 1, x - 1);
        erase(pos[x], x);
    }
}

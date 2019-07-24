#include <cstdio>
#include <cctype>
#include <climits>
#include <algorithm>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

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

    void print(int x) {
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

const int MAXN = 500000;
const int INF = 0x3f3f3f3f;

struct Point {
    int x, y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    int dis(const Point &o) const { return std::abs(x - o.x) + std::abs(y - o.y); }
} pts[MAXN + 1];

bool flag;

bool cmp(const Point &p1, const Point &p2) {
    return flag ? (p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x))
                : (p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y));
}

struct Node *null;

struct Node {
    Node *c[2];
    Point p, min, max;

    Node(const Point &p) : p(p), min(p), max(p) { c[0] = c[1] = null; }

    void maintain() {
        min.x = std::min(std::min(c[0]->min.x, c[1]->min.x), min.x);
        min.y = std::min(std::min(c[0]->min.y, c[1]->min.y), min.y);
        max.x = std::max(std::max(c[0]->max.x, c[1]->max.x), max.x);
        max.y = std::max(std::max(c[0]->max.y, c[1]->max.y), max.y);
    }

    int dis(const Point &p) {
        if (this == null) return INF;
        int res = 0;
        if (p.x < min.x || p.x > max.x) res += p.x < min.x ? min.x - p.x : p.x - max.x;
        if (p.y < min.y || p.y > max.y) res += p.y < min.y ? min.y - p.y : p.y - max.y;
        return res;
    }
};

const int MAXM = (MAXN + 3) * 2 * sizeof(Node);
char pool[MAXM];

void *operator new(size_t size) {
    static char *p = pool + MAXM;
    return p -= size;
}

Node *build(int l, int r, bool d) {
    if (l > r) return null;
    int mid = (l + r) >> 1;
    flag = d;
    std::nth_element(pts + l, pts + mid, pts + r + 1, cmp);
    Node *o = new Node(pts[mid]);
    o->c[0] = build(l, mid - 1, !d);
    o->c[1] = build(mid + 1, r, !d);
    o->maintain();
    return o;
}

int ans;

void query(const Node *o, const Point &p) {
    if (o == null) return;
    ans = std::min(ans, p.dis(o->p));
    int d = o->c[0]->dis(p) > o->c[1]->dis(p);
    query(o->c[d], p);
    if (o->c[!d]->dis(p) < ans) query(o->c[!d], p);
}

void insert(Node *&o, const Point &p) {
    if (o == null) {
        o = new Node(p);
        return;
    }
    int d = !cmp(p, o->p);
    flag ^= 1;
    insert(o->c[d], p);
    o->maintain();
}

Node *root;
int n, m;

int main() {
    null = new Node(Point(0, 0));
    null->min = Point(INF, INF);
    null->max = Point(-INF, -INF);
    null->c[0] = null->c[1] = null;

    io >> n >> m;
    for (int i = 1; i <= n; i++) io >> pts[i].x >> pts[i].y;

    root = build(1, n, 0);
    for (int cmd, x, y; m--;) {
        io >> cmd >> x >> y;
        if (cmd == 1) {
            flag = 0;
            insert(root, Point(x, y));
        } else {
            ans = INF;
            query(root, Point(x, y));
            io << ans << '\n';
        }
    }
}

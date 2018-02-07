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

inline void print(double x) {
    static char buf[30];
    snprintf(buf, 30, "%.6lf", x);
    for (register char *c = buf; *c; c++) print(*c);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

const double EPS = 1e-8;
const int MAXV = 131072;
const int MAXD = 20;

struct Point {
    double x, y;
    int id;
    Point(double a = 0, double b = 0, int c = -1) : x(a), y(b), id(c) {}

    inline bool operator<(const Point &a) const {
        return x < a.x || (fabs(x - a.x) < EPS && y < a.y);
    }

    inline bool operator==(const Point &a) const {
        return fabs(x - a.x) < EPS && fabs(y - a.y) < EPS;
    }

    inline double dist2(const Point &b) {
        return (x - b.x) * (x - b.x) + (y - b.y) * (y - b.y);
    }
};

struct Point3D {
    double x, y, z;
    Point3D(double a = 0, double b = 0, double c = 0) : x(a), y(b), z(c) {}
    Point3D(const Point &p) { x = p.x, y = p.y, z = p.x * p.x + p.y * p.y; }
    inline Point3D operator-(const Point3D &a) const {
        return Point3D(x - a.x, y - a.y, z - a.z);
    }
    inline double dot(const Point3D &a) { return x * a.x + y * a.y + z * a.z; }
};

struct Edge {
    int id;
    std::list<Edge>::iterator c;
    Edge(int id = 0) { this->id = id; }
};

inline int cmp(double v) { return fabs(v) > EPS ? (v > 0 ? 1 : -1) : 0; }

inline double cross(const Point &o, const Point &a, const Point &b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

inline Point3D cross(const Point3D &a,
                     const Point3D &b) {  // normal std::vector
    return Point3D(a.y * b.z - a.z * b.y, -a.x * b.z + a.z * b.x,
                   a.x * b.y - a.y * b.x);
}

inline int inCircle(const Point &a, Point b, Point c, const Point &p) {
    if (cross(a, b, c) < 0) std::swap(b, c);
    Point3D a3(a), b3(b), c3(c), p3(p);
    //	printf("%lf %lf %lf\n", a3.x, a3.y, a3.z);
    //	printf("%lf %lf %lf\n", b3.x, b3.y, b3.z);
    //	printf("%lf %lf %lf\n", c3.x, c3.y, c3.z);
    //	printf("%lf %lf %lf\n", p3.x, p3.y, p3.z);
    b3 = b3 - a3, c3 = c3 - a3, p3 = p3 - a3;
    Point3D f = cross(b3, c3);  // normal std::vector
    return cmp(p3.dot(f));  // check same direction, in: < 0, on: = 0, out: > 0
}

inline int intersection(const Point &a, const Point &b, const Point &c,
                        const Point &d) {  // seg(a, b) and seg(c, d)
    return cmp(cross(a, c, b)) * cmp(cross(a, b, d)) > 0 &&
           cmp(cross(c, a, d)) * cmp(cross(c, d, b)) > 0;
}

class Delaunay {
   public:
    std::list<Edge> head[MAXV];  // graph
    Point p[MAXV];
    int n, rename[MAXV];
    inline void init(int n, Point p[]) {
        memcpy(this->p, p, sizeof(Point) * n);
        std::sort(this->p, this->p + n);
        for (register int i = 0; i < n; i++) rename[p[i].id] = i;
        this->n = n;
        divide(0, n - 1);
    }

    inline void addEdge(int u, int v) {
        head[u].push_front(Edge(v));
        head[v].push_front(Edge(u));
        head[u].begin()->c = head[v].begin();
        head[v].begin()->c = head[u].begin();
    }

    void divide(int l, int r) {
        if (r - l <= 2) {  // #point <= 3
            for (register int i = l; i <= r; i++)
                for (register int j = i + 1; j <= r; j++) addEdge(i, j);
            return;
        }
        register int mid = (l + r) / 2;
        divide(l, mid);
        divide(mid + 1, r);

        std::list<Edge>::iterator it;
        register int nowl = l, nowr = r;

        //		printf("divide %d %d\n", l, r);
        for (register int update = 1;
             update;) {  // find left and right convex, lower common tangent
            update = 0;
            Point ptL = p[nowl], ptR = p[nowr];
            for (it = head[nowl].begin(); it != head[nowl].end(); it++) {
                Point t = p[it->id];
                register double v = cross(ptR, ptL, t);
                if (cmp(v) > 0 ||
                    (cmp(v) == 0 && ptR.dist2(t) < ptR.dist2(ptL))) {
                    nowl = it->id, update = 1;
                    break;
                }
            }
            if (update) continue;
            for (it = head[nowr].begin(); it != head[nowr].end(); it++) {
                Point t = p[it->id];
                register double v = cross(ptL, ptR, t);
                if (cmp(v) < 0 ||
                    (cmp(v) == 0 && ptL.dist2(t) < ptL.dist2(ptR))) {
                    nowr = it->id, update = 1;
                    break;
                }
            }
        }

        addEdge(nowl,
                nowr);  // add tangent
                        //		printf("add base %d %d\n", nowl, nowr);
        for (register int update = 1; true;) {
            update = 0;
            Point ptL = p[nowl], ptR = p[nowr];
            register int ch = -1, side = 0;
            for (it = head[nowl].begin(); it != head[nowl].end(); it++) {
                //				ptL.print(), ptR.print(),
                // p[it->id].print();
                if (cmp(cross(ptL, ptR, p[it->id])) > 0 &&
                    (ch == -1 || inCircle(ptL, ptR, p[ch], p[it->id]) < 0))
                    ch = it->id, side = -1;
                //				printf("test L %d %d %d\n",
                // nowl,
                // it->id, inCircle(ptL, ptR, p[ch], p[it->id]));
            }
            for (it = head[nowr].begin(); it != head[nowr].end(); it++) {
                if (cmp(cross(ptR, p[it->id], ptL)) > 0 &&
                    (ch == -1 || inCircle(ptL, ptR, p[ch], p[it->id]) < 0))
                    ch = it->id, side = 1;
                //				printf("test R %d %d %d\n",
                // nowr,
                // it->id, inCircle(ptL, ptR, p[ch], p[it->id]));
            }
            if (ch == -1)
                break;  // upper common tangent
                        //			printf("choose %d %d\n", ch,
                        // side);
            if (side == -1) {
                for (it = head[nowl].begin(); it != head[nowl].end();) {
                    if (intersection(ptL, p[it->id], ptR, p[ch])) {
                        head[it->id].erase(it->c);
                        head[nowl].erase(it++);
                    } else
                        it++;
                }
                nowl = ch;
                addEdge(nowl, nowr);
            } else {
                for (it = head[nowr].begin(); it != head[nowr].end();) {
                    if (intersection(ptR, p[it->id], ptL, p[ch])) {
                        head[it->id].erase(it->c);
                        head[nowr].erase(it++);
                    } else
                        it++;
                }
                nowr = ch;
                addEdge(nowl, nowr);
            }
        }
    }

    inline std::vector<std::pair<int, int> > getEdge() {
        std::vector<std::pair<int, int> > ret;
        ret.reserve(n);
        std::list<Edge>::iterator it;
        for (register int i = 0; i < n; i++) {
            for (it = head[i].begin(); it != head[i].end(); it++) {
                if (it->id < i) continue;
                //				printf("DG %d %d\n", i, it->id);
                ret.push_back(std::make_pair(p[i].id, p[it->id].id));
            }
        }
        return ret;
    }
} tool;

struct Node {
    int x, y;
    long long v;
    Node(int a = 0, int b = 0, long long c = 0) : x(a), y(b), v(c) {}
    inline bool operator<(const Node &a) const { return v < a.v; }
};

class MST {
   public:
    std::vector<Node> tree[MAXV];
    int parent[MAXV], weight[MAXV];
    int visited[MAXV], depth[MAXV];
    long long dp[MAXV][MAXD];
    int dpParent[MAXV][MAXD];
    inline int get(int x) {
        return parent[x] == x ? x : (parent[x] = get(parent[x]));
    }
    inline int put(int x, int y) {
        x = get(x), y = get(y);
        if (x == y) return 0;
        if (weight[x] > weight[y])
            weight[x] += weight[y], parent[y] = x;
        else
            weight[y] += weight[x], parent[x] = y;
        return 1;
    }
    inline int kruscal(int n, std::vector<Node> &D) {
        register long long sum = 0;
        register int e = 0;

        std::sort(D.begin(), D.end());
        for (register int i = 0; i <= n; i++) {
            parent[i] = i, weight[i] = 1;
        }

        for (int i = 0; i < D.size() && e < n - 1; i++) {
            if (put(D[i].x, D[i].y)) {
                sum += D[i].v, e++;
                tree[D[i].x].push_back(Node(D[i].x, D[i].y, D[i].v));
                tree[D[i].y].push_back(Node(D[i].y, D[i].x, D[i].v));
                //	        	printf("mst %d %d %d\n", D[i].x, D[i].y,
                // D[i].v);
            }
        }
        return sum;
    }
    inline void doubling(int n) {
        static std::queue<int> Q;
        register int u, v;
        Q.push(0), depth[0] = 0, visited[0] = 1;
        dpParent[0][0] = -1;
        while (!Q.empty()) {
            u = Q.front(), Q.pop();
            for (register int i = 0; i < tree[u].size(); i++) {
                v = tree[u][i].y;
                if (!visited[v]) {
                    depth[v] = depth[u] + 1;
                    dp[v][0] = tree[u][i].v, dpParent[v][0] = u;
                    visited[v] = 1;
                    Q.push(v);
                }
            }
            for (register int i = 0; dpParent[u][i]; i++) {
                dp[u][i + 1] = std::max(dp[u][i], dp[dpParent[u][i]][i]);
                dpParent[u][i + 1] = dpParent[dpParent[u][i]][i];
            }
        }
    }
    inline long long queryPath(int u, int v) {
        register long long ret = 0;
        if (depth[u] < depth[v]) std::swap(u, v);
        register int dist = depth[u] - depth[v];
        for (int i = 0; dist; i++, dist /= 2) {
            if (dist & 1) {
                ret = std::max(ret, dp[u][i]);
                u = dpParent[u][i];
            }
        }
        for (register int i = 0; u != v;) {
            if (dpParent[u][i] != dpParent[v][i] ||
                (i == 0 && dpParent[u][i] == dpParent[v][i])) {
                ret = std::max(ret, dp[u][i]);
                u = dpParent[u][i];
                ret = std::max(ret, dp[v][i]);
                v = dpParent[v][i];
                i++;
            } else {
                i--;
            }
        }
        return ret;
    }
} mst;

Point p[MAXV];
int main() {
    // freopen("sample/1.in", "r", stdin);
    register int n, m, q, x, y;
    IO::read(n);
    for (register int i = 0; i < n; i++) {
        IO::read(x), IO::read(y);
        p[i] = Point(x, y, i);
    }

    tool.init(n, p);

    static std::vector<Node> E;

    std::vector<std::pair<int, int> > DG = tool.getEdge();
    E.reserve(DG.size() + 1);
    for (register int i = 0; i < DG.size(); i++) {
        x = DG[i].first, y = DG[i].second;
        E.push_back(Node(x, y,
                         ((long long)p[x].x - p[y].x) * (p[x].x - p[y].x) +
                             ((long long)p[x].y - p[y].y) * (p[x].y - p[y].y)));
    }

    mst.kruscal(n, E);  // build MST
    mst.doubling(n);    // doubling algorithm

    IO::read(q);
    for (register int i = 0; i < q; i++) {
        IO::read(x), IO::read(y);
        x--, y--;
        IO::print(sqrt(mst.queryPath(x, y))), IO::print('\n');
    }
    IO::flush();
    return 0;
}

/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「CodeVS 2819」无尽的毁灭 18-11-2017
 * Voronoi 图
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const double EPS = 1e-6;
const int MAXN = 40010;
const int MAXM = MAXN * 2;
const double PI2 = M_PI * 2;

int n, k;

template <typename T>
inline T square(const T &x) {
    return x * x;
}

struct Point {
    double x, y;
    int id;
    Point(double x = 0, double y = 0, int id = -1) : x(x), y(y), id(id) {}

    inline bool operator<(const Point &a) const {
        return x < a.x || (fabs(x - a.x) < EPS && y < a.y);
    }

    inline bool operator==(const Point &a) const {
        return fabs(x - a.x) < EPS && fabs(y - a.y) < EPS;
    }

    inline double dist2(const Point &a) {
        return (x - a.x) * (x - a.x) + (y - a.y) * (y - a.y);
    }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline double dis(const Point &p) const {
        return sqrt(square(x - p.x) + square(y - p.y));
    }
    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline double operator*(const Point &p) const { return x * p.y - y * p.x; }

    inline Point operator*(const double i) const { return Point(x * i, y * i); }

    inline double operator^(const Point &p) const { return x * p.x + y * p.y; }

    inline Point operator/(const double i) const { return Point(x / i, y / i); }
};

namespace Delaunay {

struct Point3D {
    double x, y, z;

    Point3D(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
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

inline Point3D cross(const Point3D &a, const Point3D &b) {
    return Point3D(a.y * b.z - a.z * b.y, -a.x * b.z + a.z * b.x,
                   a.x * b.y - a.y * b.x);
}

inline int inCircle(const Point &a, Point b, Point c, const Point &p) {
    if (cross(a, b, c) < 0) std::swap(b, c);
    Point3D a3(a), b3(b), c3(c), p3(p);
    b3 = b3 - a3, c3 = c3 - a3, p3 = p3 - a3;
    Point3D f = cross(b3, c3);
    return cmp(p3.dot(f));
}

inline int intersection(const Point &a, const Point &b, const Point &c,
                        const Point &d) {
    return cmp(cross(a, c, b)) * cmp(cross(a, b, d)) > 0 &&
           cmp(cross(c, a, d)) * cmp(cross(c, d, b)) > 0;
}

class Delaunay {
   public:
    std::list<Edge> head[MAXN];
    Point p[MAXN];
    int n, rename[MAXN];

    inline void init(int n, Point *p) {
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
        if (r - l <= 2) {
            for (register int i = l; i <= r; i++)
                for (register int j = i + 1; j <= r; j++) addEdge(i, j);
            return;
        }
        register int mid = (l + r) / 2;
        divide(l, mid), divide(mid + 1, r);

        std::list<Edge>::iterator it;
        register int nowl = l, nowr = r;
        for (register int update = 1; update;) {
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

        addEdge(nowl, nowr);
        for (;;) {
            Point ptL = p[nowl], ptR = p[nowr];
            register int ch = -1, side = 0;
            for (it = head[nowl].begin(); it != head[nowl].end(); it++) {
                if (cmp(cross(ptL, ptR, p[it->id])) > 0 &&
                    (ch == -1 || inCircle(ptL, ptR, p[ch], p[it->id]) < 0))
                    ch = it->id, side = -1;
            }
            for (it = head[nowr].begin(); it != head[nowr].end(); it++) {
                if (cmp(cross(ptR, p[it->id], ptL)) > 0 &&
                    (ch == -1 || inCircle(ptL, ptR, p[ch], p[it->id]) < 0))
                    ch = it->id, side = 1;
            }
            if (ch == -1) break;
            if (side == -1) {
                for (it = head[nowl].begin(); it != head[nowl].end();) {
                    if (intersection(ptL, p[it->id], ptR, p[ch])) {
                        head[it->id].erase(it->c);
                        head[nowl].erase(it++);
                    } else
                        it++;
                }
                nowl = ch, addEdge(nowl, nowr);
            } else {
                for (it = head[nowr].begin(); it != head[nowr].end();) {
                    if (intersection(ptR, p[it->id], ptL, p[ch])) {
                        head[it->id].erase(it->c), head[nowr].erase(it++);
                    } else {
                        it++;
                    }
                }
                nowr = ch, addEdge(nowl, nowr);
            }
        }
    }

    inline bool parallel(const Point &a, const Point &b, const Point &c) {
        return fabs((b - a) * (c - a)) < EPS;
    }

    inline void getEdge(std::vector<std::pair<int, int> > &ret) {
        ret.reserve(n);
        std::list<Edge>::iterator it, itp;
        std::set<std::pair<int, int> > vis;
        for (register int i = 0; i < n; i++) {
            for (it = head[i].begin(); it != head[i].end(); it++) {
                if (it->id < i) continue;
                Point now = p[it->id];
                for (itp = head[i].begin(); itp != head[i].end(); ++itp) {
                    if (itp->id < i) continue;
                    if (parallel(p[i], p[it->id], p[itp->id]) &&
                        p[itp->id].dist2(p[i]) < now.dist2(p[i])) {
                        now = p[itp->id];
                    }
                }
                if (vis.find(std::make_pair(p[i].id, now.id)) == vis.end()) {
                    vis.insert(std::make_pair(p[i].id, now.id));
                    ret.push_back(std::make_pair(p[i].id, now.id));
                }
            }
        }
    }
};
}  // namespace Delaunay

namespace PlanarGraph {

Point p[MAXN];

struct Edge {
    int u, v;
    double angle;

    Edge(int u, int v) : u(u), v(v) {
        angle = atan2(p[v - 1].y - p[u - 1].y, p[v - 1].x - p[u - 1].x);
        if (angle < 0) angle += PI2;
    }

    Edge() {}
} edge[MAXM];

bool vis[MAXM + 1];
int regionCnt, infArea, rank[MAXM + 1], near[MAXM + 1];

std::vector<int> et[MAXN + 1];
std::vector<Edge> vc[MAXN];

inline void findRegion(int x, int id) {
    if (vis[id]) return;
    double area = 0;
    while (!vis[id]) {
        area += p[x - 1] * p[edge[id].v - 1];
        vis[id] = true, near[id] = regionCnt, x = edge[id].v;
        vc[regionCnt].push_back(edge[id]);
        if (!rank[id ^ 1])
            id = et[x].back();
        else
            id = et[x][rank[id ^ 1] - 1];
    }
    if (area < 0) infArea = regionCnt;
    regionCnt++;
}

inline void findDualGraph(const int n, const int m) {
    static std::pair<double, int> tmp[MAXM + 1];
    for (register int i = 0; i != m << 1; i++)
        tmp[i] = std::make_pair(edge[i].angle, i);
    std::sort(tmp, tmp + (m << 1));
    for (int i = 0, id; i != m << 1; i++) {
        id = tmp[i].second;
        const Edge &e = edge[id];
        rank[id] = et[e.u].size(), et[e.u].push_back(id);
    }
    for (register int i = 1; i <= n; i++)
        for (register int j = 0; j != (int)et[i].size(); j++)
            findRegion(i, et[i][j]);
}
}  // namespace PlanarGraph

namespace Voronoi {

Delaunay::Delaunay delaunay;

PlanarGraph::Edge vorE[MAXM];

struct Line {
    Point s, t;

    Line(const Point &s, const Point &t) : s(s), t(t) {}
    inline Point intersect(const Line &l) const {
        return s +
               (t - s) * (((s - l.s) * (l.t - l.s)) / ((l.t - l.s) * (t - s)));
    }

    inline Line getPerpendicularBisector() {
        return Line(
            Point((s.x + t.x) / 2, (s.y + t.y) / 2),
            Point((s.x + t.x) / 2 + s.y - t.y, (s.y + t.y) / 2 + t.x - s.x));
    }
};

struct Cmp {
    inline bool operator()(const std::pair<double, double> &a,
                           const std::pair<double, double> &b) const {
        if (fabs(a.first - b.first) < EPS) {
            if (fabs(a.second - b.second) < EPS) return false;
            return a.second < b.second;
        }
        return a.first < b.first;
    }
};

const double INF = 1e7;

inline void insert(PlanarGraph::Edge *p1, PlanarGraph::Edge *p2,
                   std::set<std::pair<double, double>, Cmp> &ret) {
    // parallel (p1, p2)
    if (!(fabs((PlanarGraph::p[p1->v - 1] - PlanarGraph::p[p1->u - 1]) *
               (PlanarGraph::p[p2->v - 1] - PlanarGraph::p[p2->u - 1])) <
          EPS)) {
        // circle center
        Point o = Line(PlanarGraph::p[p1->u - 1], PlanarGraph::p[p1->v - 1])
                      .getPerpendicularBisector()
                      .intersect(Line(PlanarGraph::p[p2->u - 1],
                                      PlanarGraph::p[p2->v - 1])
                                     .getPerpendicularBisector());

        // valid
        if (fabs(o.x) <= INF && fabs(o.y) <= INF && !isinf(o.x) &&
            !isnan(o.x) && !isinf(o.y) && !isnan(o.y)) {
            ret.insert(std::make_pair(o.x, o.y));
        }
    }
}

inline void getCenter(std::set<std::pair<double, double>, Cmp> &ret, int i) {
    PlanarGraph::Edge *p1 = &PlanarGraph::vc[i][0],
                      *p2 = &PlanarGraph::vc[i].back();
    insert(p1, p2, ret);
    if (i == PlanarGraph::infArea) return;
    for (register int j = 0; j < (int)PlanarGraph::vc[i].size() - 1; j++) {
        p1 = &PlanarGraph::vc[i][j], p2 = &PlanarGraph::vc[i][j + 1];
        insert(p1, p2, ret);
    }
}

inline void buildVoronoi() {
    delaunay.init(n, PlanarGraph::p);
    std::vector<std::pair<int, int> > edge;
    delaunay.getEdge(edge);
    register int m = edge.size();

    for (register int i = 0; i < m; i++) {
        if (edge[i].first > edge[i].second)
            std::swap(edge[i].first, edge[i].second);
        PlanarGraph::edge[i << 1] =
            PlanarGraph::Edge(edge[i].first + 1, edge[i].second + 1);
        PlanarGraph::edge[i << 1 | 1] =
            PlanarGraph::Edge(edge[i].second + 1, edge[i].first + 1);
    }

    PlanarGraph::findDualGraph(n, m);

    for (register int i = 0, a; i != m; i++) {
        a = i << 1;
        vorE[i].u = PlanarGraph::near[a];
        vorE[i].v = PlanarGraph::near[a ^ 1];
    }
    static int deg[MAXN + 1];
    for (register int i = 0; i < m; i++) deg[vorE[i].u]++, deg[vorE[i].v]++;
    std::set<std::pair<double, double>, Cmp> ans;
    for (register int i = 0; i < PlanarGraph::regionCnt; i++) {
        if (deg[i] >= k) getCenter(ans, i);
    }
    std::cout << ans.size() << '\n';
    for (std::set<std::pair<double, double>, Cmp>::iterator it = ans.begin();
         it != ans.end(); ++it) {
        std::cout << std::fixed << std::setprecision(4) << it->first << ' '
                  << it->second << '\n';
    }
}
}  // namespace Voronoi

inline void solve() {
    std::cin >> n >> k;
    for (register int i = 0; i < n; i++) {
        std::cin >> PlanarGraph::p[i].x >> PlanarGraph::p[i].y;
        PlanarGraph::p[i].id = i;
    }
    Voronoi::buildVoronoi();
}
}  // namespace

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}
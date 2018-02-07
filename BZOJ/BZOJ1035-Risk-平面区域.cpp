/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1035」Risk 22-11-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 100000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline void read(T& x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 100000;

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
    ~InputOutputStream() { flush(); }

    template <typename T>
    inline InputOutputStream& operator>>(T& x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream& operator<<(const T& x) {
        print(x);
        return *this;
    }
} io;
}  // namespace IO

namespace {
#define foreach(e, x) for (__typeof(x.begin()) e = x.begin(); e != x.end(); ++e)

using IO::io;

struct Point;
struct Polygon;

struct Edge {
    Polygon* poly;
    Point *src, *dst;
    double alpha;
    Edge *rev, *prev, *next;
    Edge(Point* _src, Point* _dst);
};

struct Point {
    std::vector<Edge*> outgo;
    int x, y;
    double alpha;
    Point(int _x, int _y) : x(_x), y(_y) {}
    Point() {}
    void doSort();
};

inline int det(Point* p1, Point* p2) { return p1->x * p2->y - p2->x * p1->y; }

inline int cross(Point* p1, Point* p2, Point* p3) {
    return (p2->x - p1->x) * (p3->y - p1->y) -
           (p3->x - p1->x) * (p2->y - p1->y);
}

inline int sign(int x) { return x < 0 ? -1 : x > 0; }

inline int crossOp(Point* p1, Point* p2, Point* p3) {
    return sign(cross(p1, p2, p3));
}

inline bool isSS(Point* p1, Point* p2, Point* q1, Point* q2) {
    return crossOp(p1, p2, q1) * crossOp(p1, p2, q2) < 0 &&
           crossOp(q1, q2, p1) * crossOp(q1, q2, p2) < 0;
}

inline int dot(Point* a, Point* b) { return a->x * b->x + a->y * b->y; }

Point memo[100];
int memoTop = 0;

Point* sub(Point* a, Point* b) {
    Point* ret = memo + memoTop++;
    ret->x = a->x - b->x;
    ret->y = a->y - b->y;
    return ret;
}

bool inSP(Point* p1, Point* p2, Point* q) {
    if (crossOp(p1, p2, q) != 0) return false;
    memoTop = 0;
    return dot(sub(q, p1), sub(q, p2)) <= 0;
}

struct Polygon;

std::vector<Polygon*> polys;

struct Polygon {
    std::set<int> ans;
    int idx;
    std::vector<Edge*> edges;
    Polygon() {
        idx = -1;
        polys.push_back(this);
    }
    bool isInside(Point* p) {
        static Point* far = new Point(23413, 32145);
        int isCount = 0;
        foreach (iter, edges) {
            Edge* e = *iter;
            isCount += isSS(e->src, e->dst, p, far);
        }
        return isCount % 2 == 1;
    }
    bool contain(Polygon* p) {
        foreach (iter, p->edges) {
            Edge* e = *iter;
            Point* q = e->src;
            bool check = false;
            foreach (iter2, edges) {
                Edge* e2 = *iter2;
                if (inSP(e2->src, e2->dst, q)) {
                    check = true;
                    break;
                }
            }
            if (check) continue;
            if (!isInside(q)) return false;
        }
        return true;
    }
    int area;
    void calcArea() {
        area = 0;
        foreach (iter, edges) {
            Edge* e = *iter;
            area += det(e->src, e->dst);
        }
    }
};

std::vector<Edge*> edges;

Edge::Edge(Point* _src, Point* _dst) : src(_src), dst(_dst), poly(0) {
    alpha = atan2(dst->y - src->y, dst->x - src->x);
    edges.push_back(this);
}

bool cmpEdgePtr(Edge* a, Edge* b) { return a->alpha < b->alpha; }

void Point::doSort() {
    std::sort(outgo.begin(), outgo.end(), cmpEdgePtr);
    int n = outgo.size();
    for (int i = 0; i < n; ++i) {
        int prev = i == 0 ? n - 1 : i - 1;
        int next = i + 1 == n ? 0 : i + 1;
        outgo[i]->prev = outgo[prev];
        outgo[i]->next = outgo[next];
    }
}

bool cmpPointPtr(Point* a, Point* b) {
    if (a->x != b->x) return a->x < b->x;
    return a->y < b->y;
}

struct Comparator {
    bool operator()(Point* a, Point* b) const { return cmpPointPtr(a, b); }
};

bool isEqual(Point* a, Point* b) { return a->x == b->x && a->y == b->y; }

std::map<Point*, Point*, Comparator> pointToId;

Point* getPoint(Point* p) {
    if (pointToId.count(p) > 0) {
        delete p;
        return pointToId[p];
    }
    return pointToId[p] = p;
}

std::vector<Point*> armies;
int nNations, nEdges;

Point* readPoint() {
    int x, y;
    io >> x >> y;
    return new Point(x, y);
}

void buildEdge(Point* p, Point* q) {
    Edge* pq = new Edge(p, q);
    Edge* qp = new Edge(q, p);
    pq->rev = qp;
    qp->rev = pq;
    p->outgo.push_back(pq);
    q->outgo.push_back(qp);
}

void readInput() {
    io >> nNations >> nEdges;
    for (int i = 0; i < nNations; ++i) {
        armies.push_back(readPoint());
    }
    for (int i = 0; i < nEdges; ++i) {
        Point *p = readPoint(), *q = readPoint();
        p = getPoint(p);
        q = getPoint(q);
        buildEdge(p, q);
    }
}

bool cmpPolyPtr(Polygon* a, Polygon* b) { return a->area < b->area; }

void work() {
    foreach (iter, pointToId) {
        Point* p = iter->second;
        p->doSort();
    }
    foreach (iter, edges) {
        Edge* e = *iter;
        if (e->poly) continue;
        Polygon* poly = new Polygon;
        int cur = 0;
        while (e->poly == 0) {
            e->poly = poly;
            poly->edges.push_back(e);
            e = e->rev->prev;
        }
    }

    std::vector<Polygon*> validPolys;
    foreach (iter, polys) {
        Polygon* poly = *iter;
        poly->calcArea();
        if (poly->area > 0) {
            validPolys.push_back(poly);
        }
    }

    sort(validPolys.begin(), validPolys.end(), cmpPolyPtr);
    std::vector<Polygon*> byIdx(validPolys.size());
    std::vector<bool> used(armies.size(), false);
    for (int i = 0; i < validPolys.size(); ++i) {
        Polygon* cur = validPolys[i];
        for (int j = 0; j < armies.size(); ++j) {
            if (used[j]) continue;
            Point* p = armies[j];
            if (cur->isInside(p)) {
                cur->idx = j;
                used[j] = true;
                break;
            }
        }
        assert(cur->idx != -1);
        byIdx[cur->idx] = cur;
    }

    for (int i = 0; i < byIdx.size(); ++i) {
        Polygon* poly = byIdx[i];
        std::set<int>& ans = poly->ans;
        foreach (iter, poly->edges) {
            Edge* e = *iter;
            Polygon* np = e->rev->poly;
            if (np->area > 0) {
                ans.insert(np->idx);
            }
        }
        Polygon* fa = 0;
        for (int j = 0; j < validPolys.size(); ++j) {
            Polygon* f = validPolys[j];
            if (f->area > poly->area && f->contain(poly)) {
                fa = f;
                break;
            }
        }
        if (fa != 0) {
            foreach (iter, poly->edges) {
                Edge* e = *iter;
                Polygon* np = e->rev->poly;
                if (np->area < 0) {
                    ans.insert(fa->idx);
                    fa->ans.insert(poly->idx);
                    break;
                }
            }
        }
    }

    for (int i = 0; i < byIdx.size(); ++i) {
        Polygon* p = byIdx[i];
        std::set<int>& ans = p->ans;
        io << (int)ans.size();
        foreach (iter, ans) { io << ' ' << *iter + 1; }
        io << '\n';
    }
}

void solve() {
    readInput();
    work();
}
}

int main() { solve(); }
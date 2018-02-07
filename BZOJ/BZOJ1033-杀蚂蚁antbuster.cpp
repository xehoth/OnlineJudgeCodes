/*******************************************************************************
 * Copyright (c) 2016-2017, xehoth
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name xehoth, nor the names of its contributors may be used
 *       to endorse or promote products derived from this software without
 *       specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY XEHOTH AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEHOTH AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
#include <bits/stdc++.h>
#include <algorithm>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <iostream>
/**
 * 「BZOJ 1033」杀蚂蚁antbuster 20-05-2017
 *
 * @author xehoth
 */
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
    for (iosig = false, c = read(); !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
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
}  // namespace IO

namespace Task {

typedef long double ld;

const int MAXN = 8;
const int MAXM = 8;
const int MAXS = 20;
const int MAXT = 200000;
const ld EPS = 1e-6;

int n, m, s, d, r, t, time;

struct Map {
    int message;
    bool reachable;

    Map() : message(0), reachable(true) {}
} map[MAXN + 1][MAXM + 1];

template <typename T>
struct Point {
    T x, y;

    Point(const T x = 0, const T y = 0) : x(x), y(y) {}

    inline Map *operator->() const {
        return &map[static_cast<int>(x)][static_cast<int>(y)];
    }

    inline Point offset(const int id) const {
        switch (id) {
            case 0:
                return Point(x, y + 1);
            case 1:
                return Point(x + 1, y);
            case 2:
                return Point(x, y - 1);
            case 3:
                return Point(x - 1, y);
            default:
                return *this;
        }
    }

    inline bool valid() const { return x >= 0 && x <= n && y >= 0 && y <= m; }

    inline bool operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }
};

inline bool sign(const ld a) { return fabs(a) <= EPS; }

inline bool sign(const ld a, const ld b) { return sign(a - b); }

template <typename T>
inline T square(const T &x) {
    return x * x;
}

template <typename Ta, typename Tb>
inline ld dis(const Point<Ta> a, const Point<Tb> b) {
    return sqrt(static_cast<ld>(square(a.x - b.x) + square(a.y - b.y)));
}

struct Ant {
    Point<int> position, lastPosition;
    int level;
    int hpLimit, hp;
    bool hasCake;
    int spawnTime;

    Ant(const int level)
        : position(0, 0),
          lastPosition(-1, -1),
          level(level),
          hpLimit(floor(4 * pow(1.1, level))),
          hp(hpLimit),
          hasCake(false),
          spawnTime(time) {
        position->reachable = false;
    }

    inline void moveTo(const Point<int> &nextPosition) {
        lastPosition = position;
        position = nextPosition;
        if (lastPosition == position) return;
        lastPosition->reachable = true;
        position->reachable = false;
    }

    inline int age() const { return time - spawnTime; }

    inline bool attackable(const Point<int> &p) {
        const ld dist = dis(position, p);
        return dist < r || sign(dist, r);
    }
};

struct Line {
    ld k, b;

    Line(const Point<int> &p1, const Point<int> &p2) {
        const ld x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
        if (x1 == x2) {
            k = NAN, b = x1;
        } else {
            k = (y2 - y1) / (x2 - x1), b = y1 - k * x1;
        }
    }

    Line(const ld k, const Point<int> &p1)
        : k(std::isinf(k) ? NAN : k),
          b(std::isinf(k) ? p1.x : (p1.y - k * p1.x)) {}

    Line(const ld k, const ld b) : k(k), b(b) {}

    Line perpendicular(const Point<int> &p1) const {
        return std::isnan(k) ? Line(0, p1.y) : Line(-1.0 / k, p1);
    }
};

struct Segment {
    Point<int> p1, p2;
    Line line;
    ld length;

    Segment(const Point<int> &p1, const Point<int> &p2)
        : p1(p1), p2(p2), line(p1, p2), length(dis(p1, p2)) {}
};

Point<ld> lineIntersection(const Line &a, const Line &b) {
    if (std::isnan(a.k) && std::isnan(b.k) || a.k == b.k) throw;
    if (std::isnan(a.k)) return Point<ld>(a.b, a.b * b.k + b.b);
    if (std::isnan(b.k)) return Point<ld>(b.b, b.b * a.k + a.b);
    const ld x = (b.b - a.b) / (a.k - b.k);
    const ld y = a.k * x + a.b;
    return Point<ld>(x, y);
}

inline bool isPointOnSegment(const Point<ld> &p, const Segment &s) {
    return sign(dis(p, s.p1) + dis(p, s.p2) - s.length);
}

inline bool segmentCircleIntersection(const Segment &s, const Point<int> &p,
                                      const ld r) {
    Line v = s.line.perpendicular(p);
    Point<ld> is = lineIntersection(v, s.line);
    ld min, max;
    if (isPointOnSegment(is, s)) {
        min = dis(is, p), max = std::max(dis(is, s.p1), dis(is, s.p2));
    } else {
        min = dis(p, s.p1), max = dis(p, s.p2);
        (min > max) ? std::swap(min, max) : (void)0;
    }
    return (min < r || sign(min, r)) && (max > r || sign(max, r));
}

std::list<Ant> ants;
std::list<Ant>::iterator cakeOwner = ants.end();
Point<int> towers[MAXS];

inline void spawn() {
    if (ants.size() < 6) {
        if (map[0][0].reachable == false) return;

        static int cnt = -1;
        cnt++;
        ants.push_back(Ant(cnt / 6 + 1));
    }
}

inline void incMessage() {
    for (std::list<Ant>::const_iterator it = ants.begin(); it != ants.end();
         it++)
        it->position->message += it->hasCake ? 5 : 2;
}

inline void move() {
    for (std::list<Ant>::iterator it = ants.begin(); it != ants.end(); it++) {
        register int id = -1;
        for (register int i = 0; i < 4; i++) {
            Point<int> newPosition = it->position.offset(i);

            if (!newPosition.valid() || newPosition == it->lastPosition ||
                !newPosition->reachable)
                continue;
            if (id == -1 ||
                newPosition->message > it->position.offset(id)->message)
                id = i;
        }

        if (id != -1 && (it->age() + 1) % 5 == 0) {
            Point<int> newPosition;
            do {
                id = (id - 1 + 4) % 4;
                newPosition = it->position.offset(id);
            } while (!newPosition.valid() || !newPosition->reachable ||
                     newPosition == it->lastPosition);
        }

        it->moveTo(it->position.offset(id));
    }
}

inline void getCake() {
    if (cakeOwner != ants.end()) return;

    for (std::list<Ant>::iterator it = ants.begin(); it != ants.end(); it++) {
        if (it->position == Point<int>(n, m)) {
            cakeOwner = it;
            it->hasCake = true;
            it->hp = std::min(it->hpLimit, it->hp + it->hpLimit / 2);
            break;
        }
    }
}

inline void attack() {
    for (register int i = 0; i < s; i++) {
        std::list<Ant>::iterator target = ants.end();
        if (cakeOwner != ants.end() && cakeOwner->attackable(towers[i]))
            target = cakeOwner;
        if (target == ants.end()) {
            for (std::list<Ant>::iterator it = ants.begin(); it != ants.end();
                 it++) {
                ld d = dis(it->position, towers[i]);
                if ((d < r || sign(d, r)) &&
                    (target == ants.end() ||
                     dis(it->position, towers[i]) <
                         dis(target->position, towers[i])))
                    target = it;
            }
        }

        if (target != ants.end()) {
            target->hp -= d;
            Segment s(towers[i], target->position);
            for (std::list<Ant>::iterator it = ants.begin(); it != ants.end();
                 it++)
                if (it != target &&
                    segmentCircleIntersection(s, it->position, 0.5))
                    it->hp -= d;
        }
    }
}

inline void kill() {
    for (std::list<Ant>::iterator it = ants.begin(); it != ants.end();) {
        if (it->hp < 0) {
            it->position->reachable = true;
            if (it == cakeOwner) cakeOwner = ants.end();
            it = ants.erase(it);
        } else {
            it++;
        }
    }
}

inline bool check() {
    for (std::list<Ant>::const_iterator it = ants.begin(); it != ants.end();
         it++)
        if (it->position == Point<int>(0, 0) && it->hasCake) return false;
    return true;
}

inline void decMessage() {
    for (register int i = 0; i <= n; i++)
        for (register int j = 0; j <= m; j++)
            if (map[i][j].message > 0) map[i][j].message--;
}

inline void solve() {
    using namespace IO;
    read(n), read(m), read(s), read(d), read(r);
    for (register int i = 0; i < s; i++)
        read(towers[i].x), read(towers[i].y), towers[i]->reachable = false;
    read(t);
    for (time = 1; time <= t; time++) {
        spawn();
        incMessage();
        move();
        getCake();
        attack();
        kill();
        if (!check()) break;
        decMessage();
    }

    time > t ? print("The game is going on\n")
             : (print("Game over after "), print(time), print(" seconds\n"));
    print(ants.size()), print('\n');
    for (std::list<Ant>::const_iterator it = ants.begin(); it != ants.end();
         it++)
        print(it->age()), print(' '), print(it->level), print(' '),
            print(it->hp), print(' '), print(it->position.x), print(' '),
            print(it->position.y), print('\n');
}
}  // namespace Task

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    Task::solve();
    IO::flush();
    return 0;
}

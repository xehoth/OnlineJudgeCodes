/**
 * Copyright (c) 2018, xehoth
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * 「BZOJ 5329」战略游戏 25-05-2018
 * 圆方树 + 虚树
 * @author xehoth
 */
#include <bits/stdc++.h>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    inline InputOutputStream &operator>>(int &x) {
        static char c;
        for (c = read(); !isdigit(c); c = read())
            if (c == -1) return *this;
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        return *this;
    }

    inline void print(char c) {
        if (oh == obuf + SIZE) {
            fwrite(obuf, 1, SIZE, stdout);
            oh = obuf;
        }
        *oh++ = c;
    }

    template <typename T>
    inline void print(T x) {
        static int buf[21], cnt;
        if (x != 0) {
            if (x < 0) {
                print('-');
                x = -x;
            }
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
            while (cnt) print((char)buf[cnt--]);
        } else {
            print('0');
        }
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 100000;
const int MAX_LOG = 20;

class CircleSquareTree {
   public:
    static inline void addEdge(int u, int v, std::vector<int> *g) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void addEdge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void setType(int n) {
        this->n = n;
        for (int i = 1; i <= n; i++) type[i] = CIRCLE;
    }

   protected:
    enum Type { SQUARE = 0, CIRCLE = 1 };

    Type type[MAXN * 2 + 1];
    int cnt;

    void clear() {
        idx = 0;
        cnt = 0;
        st.clear();
        memset(dfn, 0, sizeof(int) * (n + 1));
        for (int i = 1; i <= n; i++) g[i].clear();
    }

    void build(std::vector<int> *ng) {
        this->ng = ng;
        tarjan(1, 0);
    }

    int getVertexCount() { return n + cnt; }

   private:
    int dfn[MAXN + 1], low[MAXN + 1], idx, n;
    std::vector<int> st;
    std::vector<int> g[MAXN + 1];
    std::vector<int> *ng;

    void tarjan(int u, int fa) {
        low[u] = dfn[u] = ++idx;
        st.push_back(u);
        for (int i = 0, v; i < (int)g[u].size(); i++) {
            if ((v = g[u][i]) != fa) {
                if (!dfn[v]) {
                    tarjan(v, u);
                    low[u] = std::min(low[u], low[v]);
                } else {
                    low[u] = std::min(low[u], dfn[v]);
                }
            }
        }
        if (low[u] >= dfn[fa]) {
            int np = n + (++cnt);
            type[np] = SQUARE;
            addEdge(u, np, ng);
            if (fa > 0) addEdge(fa, np, ng);
            for (int v = st.back();; v = st.back()) {
                st.pop_back();
                if (u == v) break;
                addEdge(v, np, ng);
            }
        }
    }
};

struct Comparator {
    const struct VirtualCircleSquareTree *g;
    bool operator()(int, int);
};

struct VirtualCircleSquareTree : public CircleSquareTree {
    std::vector<int> g[MAXN * 2 + 1];
    int n;
    int min[MAX_LOG][MAXN * 4 + 1], logs[MAXN * 4 + 1], idx;
    int dfn[MAXN * 2 + 1], id[MAXN * 4 + 1], dep[MAXN * 2 + 1], dw[MAXN * 2 + 1];
    Comparator cmp;

    void dfs(int u, int pre) {
        dep[u] = dep[pre] + 1;
        dw[u] = dw[pre] + type[u];
        id[dfn[u] = ++idx] = u;
        for (int i = 0, v; i < (int)g[u].size(); i++) {
            if ((v = g[u][i]) != pre) {
                dfs(v, u);
                id[++idx] = u;
            }
        }
    }

    int get(int a, int b) { return dep[a] < dep[b] ? a : b; }

    void build() {
        CircleSquareTree::build(g);
        n = CircleSquareTree::getVertexCount();
        dfs(1, 0);
        memcpy(min[0] + 1, id + 1, sizeof(int) * idx);
        for (int w = 1; (1 << w) <= idx; w++)
            for (int i = 1; i + (1 << w) - 1 <= idx; i++)
                min[w][i] = get(min[w - 1][i], min[w - 1][i + (1 << (w - 1))]);
        logs[0] = -1;
        for (int i = 1; i <= idx; i++) logs[i] = logs[i >> 1] + 1;
    }

    int lca(int u, int v) {
        u = dfn[u];
        v = dfn[v];
        if (u > v) std::swap(u, v);
        static int bit;
        bit = logs[v - u + 1];
        return get(min[bit][u], min[bit][v - (1 << bit) + 1]);
    }

    int circleCount(int u, int v) {
        int l = lca(u, v);
        return dw[u] + dw[v] - 2 * dw[l] + type[l] - type[u] - type[v];
    }

    void clear() {
        for (int i = 0; i <= n; i++) g[i].clear();
        idx = 0;
        CircleSquareTree::clear();
    }

    int buildVirtualTree(std::vector<int> &a) {
        cmp.g = this;
        std::sort(a.begin(), a.end(), cmp);
        int ret = 0, top = 0, n = a.size();
        static int st[MAXN * 2 + 9];
        for (int i = 0, u; i < n; i++) {
            for (u = a[i]; top > 1 && lca(st[top - 2], u) == lca(st[top - 1], u); top--)
                ret += circleCount(st[top - 2], st[top - 1]);
            if (top > 0) {
                int l = lca(st[top - 1], u);
                if (l != st[top - 1]) {
                    ret += circleCount(st[top - 1], l) + type[l];
                    st[top - 1] = l;
                }
            }
            st[top++] = u;
        }
        for (; top > 1; top--) ret += circleCount(st[top - 2], st[top - 1]);
        return ret;
    }
} g;

bool Comparator::operator()(int a, int b) { return g->dfn[a] < g->dfn[b]; }

inline void solve() {
    g.clear();
    int n, m;
    io >> n >> m;
    for (int i = 0, u, v; i < m; i++) {
        io >> u >> v;
        g.addEdge(u, v);
    }
    g.setType(n);
    g.build();
    static std::vector<int> a;
    int q;
    io >> q;
    for (int s; q--;) {
        io >> s;
        a.resize(s);
        for (int i = 0; i < s; i++) io >> a[i];
        io << g.buildVirtualTree(a) << '\n';
    }
}

int main() {
    int T;
    io >> T;
    while (T--) solve();
    return 0;
}
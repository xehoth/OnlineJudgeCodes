/**
 * 「CodeVS 3269」混合背包 09-06-2017
 * dp + 单调队列
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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}

namespace Task {

const int MAXN = 200;
const int MAXV = 200000;

template <typename T, typename Comparator = std::less<T> >
class MonotoneQueue : public std::deque<std::pair<T, int> > {
   public:
    typedef std::pair<T, int> Pair;
    typedef std::deque<Pair> super;

    MonotoneQueue(Comparator cmp = Comparator()) : cmp(cmp), pos(0), cur(0) {}

    inline void push(const T &v) {
        while (!super::empty() && cmp(super::front().first, v))
            super::pop_front();
        super::push_front(Pair(v, cur++));
    }

    inline void pop() {
        if (super::back().second == pos++) super::pop_back();
    }

    inline const T &top() { return super::back().first; }

    inline void clear() { super::clear(), pos = cur = 0; }

    inline int size() { return cur - pos; }

   private:
    int pos, cur;
    Comparator cmp;
};

int v;
int f[MAXV + 1];

inline void pack(int c, int w, int n) {
    if (n == 1) {
        for (register int v = Task::v; v >= c; v--)
            f[v] = std::max(f[v], f[v - c] + w);
    } else if (n == -1) {
        for (register int v = c; v <= Task::v; v++)
            f[v] = std::max(f[v], f[v - c] + w);
    } else {
        n = std::min(n, Task::v / c);
        for (register int r = 0; r < c; r++) {
            static MonotoneQueue<int> q;
            q.clear();
            register int m = (Task::v - r) / c;
            for (register int k = 0; k <= m; k++) {
                if (q.size() == n + 1) q.pop();
                q.push(f[k * c + r] - k * w);
                f[k * c + r] = q.top() + k * w;
            }
        }
    }
}

inline void solve() {
    using namespace IO;
    register int n;
    read(n), read(v);
    for (register int i = 0, c, w, t; i < n; i++)
        read(c), read(w), read(t), pack(c, w, t);
    print(f[v]);
}
}

int main() {
    Task::solve();
    IO::flush();
    return 0;
}
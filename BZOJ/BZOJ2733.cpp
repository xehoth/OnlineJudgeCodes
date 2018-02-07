/*
 * created by xehoth on 19-05-2017
 */
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

const int MAXM = 250001;

template <typename Type>
class AllocBase {
   public:
    typedef unsigned size_type;
    typedef int difference_type;
    typedef Type* pointer;
    typedef const Type* const_pointer;
    typedef Type& reference;
    typedef const Type& const_reference;
    typedef Type value_type;

    template <typename OtherType>
    struct rebind {
        typedef AllocBase<OtherType> other;
    };

    static Type memoryPool[MAXM], *p;
    static Type* bin[MAXM];
    static int cnt;

    AllocBase() {}

    AllocBase(const AllocBase&) {}

    template <typename OtherType>
    AllocBase(const AllocBase<OtherType>&) {}

    ~AllocBase() {}

    inline pointer address(reference x) const { return &x; }

    inline const_pointer address(const_reference x) const { return &x; }

    inline pointer allocate(size_type n, const void* = 0) const {
        // std::cerr << n << std::endl;
        return cnt ? bin[cnt--] : p++;
        // return static_cast<pointer>(operator new(n * sizeof(Type)));
    }

    inline void deallocate(pointer p, size_type n) const {
        bin[++cnt] = p;
        // operator delete(p);
    }

    inline size_type max_size() const { return size_type(-1) / sizeof(Type); }

    inline void construct(pointer p, const Type& val) const {
        new (static_cast<void*>(p)) Type(val);
    }

    inline void destroy(pointer p) const {
        // p->~Type();
    }
};

template <typename Type>
Type AllocBase<Type>::memoryPool[MAXM];
template <typename Type>
Type* AllocBase<Type>::p = memoryPool;
template <typename Type>
Type* AllocBase<Type>::bin[MAXM];
template <typename Type>
int AllocBase<Type>::cnt;

template <typename Type>
inline bool operator==(const AllocBase<Type>&, const AllocBase<Type>&) {
    return true;
}

template <typename Type>
inline bool operator!=(const AllocBase<Type>&, const AllocBase<Type>&) {
    return false;
}

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <class T>
inline void read(T& x) {
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

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

namespace Task {

const int MAXN = 100005;

int fa[MAXN], rank[MAXN];
typedef __gnu_pbds::tree<int, int, std::less<int>, __gnu_pbds::rb_tree_tag,
                         __gnu_pbds::tree_order_statistics_node_update,
                         AllocBase<char> >
    Tree;
Tree t[MAXN];

inline int get(int x) {
    register int p = x, i;
    while (p != fa[p]) p = fa[p];
    while (p != x) i = fa[x], fa[x] = p, x = i;
    return p;
}

inline void put(int x, int y) {
    if ((x = get(x)) != (y = get(y))) {
        rank[x] > rank[y] ? (std::swap(x, y), 0) : 0;
        fa[x] = y;
        for (Tree::iterator it = t[x].begin(); it != t[x].end(); it++)
            t[y][it->first] = it->second;
        rank[x] == rank[y] ? rank[y]++ : 0;
        t[x].clear();
    }
}

inline int query(int x, int k) {
    return k > t[x = get(x)].size() ? -1
                                    : t[get(x)].find_by_order(k - 1)->second;
}

inline void solve() {
    register int n, m, q;
    read(n), read(m);
    for (register int i = 1, a; i <= n; i++) read(a), fa[i] = i, t[i][a] = i;
    for (register int i = 1, x, y; i <= m; i++) read(x), read(y), put(x, y);
    read(q);
    register char c;
    while (q--) {
        c = read();
        while (isspace(c)) c = read();
        switch (c) {
            case 'B':
                read(n), read(m), put(n, m);
                break;
            case 'Q':
                read(n), read(m);
                print(query(n, m)), print('\n');
                break;
        }
    }
}

}  // namespace Task

int main() {
    // freopen("in.in", "r", stdin);
    Task::solve(), flush();
    return 0;
}
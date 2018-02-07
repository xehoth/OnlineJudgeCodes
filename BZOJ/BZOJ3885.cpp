#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 100000;
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
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;
}  // namespace IO

namespace {

const int MAXN = 1010;
const int INF = 0x3f3f3f3f;

int map[MAXN][MAXN], f[MAXN][MAXN];
struct Data {
    int H, h, l, r;
} s[MAXN];

int ans1, ans2 = INF, q[MAXN];

inline int get(int a, int b, int c, int d) {
    return f[d][b] - f[d][a] - f[c][b] + f[c][a];
}

inline int getArea(int a, int b, int c, int d) {
    register int l = c, r = d, mid, ans;
    while (r - l > 1) {
        if (r - l <= 3) {
            ans = l;
            for (register int i = r; i >= l; i--) {
                if (get(a, b, c, i) == 0) {
                    ans = i;
                    break;
                }
            }
            break;
        }
        mid = l + r >> 1;
        if (get(a, b, c, mid) == 0)
            l = mid;
        else
            r = mid;
    }
    return (b - a - 1) * (d - ans - 1);
}

using IO::io;

inline void solve() {
    register char c;
    register int n;
    io >> n;
    for (register int a, b; n--;) {
        io >> a >> b >> c, a++, b++;
        if (c == 'H')
            map[a][b] = 1;
        else
            map[a][b] = 2;
    }
    for (register int i = 1, tmp; i <= 1001; i++) {
        tmp = 0;
        for (register int j = 1; j <= 1001; j++) {
            if (map[i][j] == 1) tmp++;
            f[i][j] = f[i - 1][j] + tmp;
        }
    }
    for (register int i = 1; i <= 1001; i++) s[i].h = INF;
    register int l, r;
    for (register int i = 1; i <= 1001; i++) {
        for (register int j = 1; j <= 1001; j++) {
            if (map[i][j] == 2) {
                s[j].H = 0, s[j].h = INF;
            } else {
                s[j].H++;
                if (map[i][j] == 1)
                    s[j].h = 1;
                else
                    s[j].h++;
            }
            s[j].l = s[j].r = j;
        }
        l = 1, r = 0;
        for (register int j = 1; j <= 1001; j++) {
            while (l <= r && s[q[r]].H >= s[j].H) s[j].l = s[q[r--]].l;
            while (s[j].l < j && s[s[j].l].h > s[j].H) s[j].l++;
            q[++r] = j;
        }
        l = 1, r = 0;
        for (register int j = 1001; j; j--) {
            while (l <= r && s[q[r]].H >= s[j].H) s[j].r = s[q[r--]].r;
            while (s[j].r > j && s[s[j].r].h > s[j].H) s[j].r--;
            q[++r] = j;
        }
        for (register int j = 1; j <= 1001; j++) {
            int ret = get(s[j].l - 1, s[j].r, i - s[j].H, i);
            if (ret >= ans1) {
                int tmp = getArea(s[j].l - 1, s[j].r, i - s[j].H, i);
                if (ret == ans1)
                    ans2 = std::min(ans2, tmp);
                else
                    ans1 = ret, ans2 = tmp;
            }
        }
    }
    io << ans1 << '\n' << ans2 << '\n';
}
}  // namespace

int main() {
    solve();
    return 0;
}
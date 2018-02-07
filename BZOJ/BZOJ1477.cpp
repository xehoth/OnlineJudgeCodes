#include <iostream>
using namespace std;
typedef long long Long;
inline Long abs(Long l) { return l < 0 ? -l : l; }
Long x0, y0;
Long x, y, m, n, L;
Long gcd(Long a, Long b) {
    Long t, d;
    if (b == 0) {
        x0 = 1;
        y0 = 0;
        return a;
    }
    d = gcd(b, a % b);
    t = x0;
    x0 = y0;
    y0 = t - a / b * y0;
    return d;
}
int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cin >> x >> y >> m >> n >> L;
    x = x % L, y = y % L;
    if (x > y) {
        Long t = y;
        y = x, x = t, t = n, n = m, m = t;
    }
    Long a = abs(m - n);
    Long b = -L;
    Long c;
    if (m > n)
        c = y - x;
    else
        c = x - y + L;
    Long d = gcd(a, b);
    if (c % d != 0)
        cout << "Impossible\n";
    else {
        Long add1 = x0 * c / d;
        Long add2 = abs(b / d);
        while (add1 < 0) add1 += add2;
        while (add1 - add2 >= 0) add1 -= add2;
        cout << add1 << "\n";
    }
    return 0;
}
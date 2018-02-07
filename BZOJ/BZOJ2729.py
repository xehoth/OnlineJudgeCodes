n, m = map(int, raw_input().split())
fac = [1] * 2005
for i in xrange(1, 2005):
    fac[i] = fac[i - 1] * i


def c(n, m):
    if (n < m):
        return 0
    return fac[n] / fac[n - m]


print(c(n, n) * (c(n + 1, 2) * c(n + 3, m) + 2 * (n + 1) * c(n + 2, m - 1) * m))

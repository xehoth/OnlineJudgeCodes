# created by xehoth on 01-03-2017
m, n = int(input()), int(input())

if n == 0:
    print 0
    sys.exit()
tmpn, len = n, 0

while tmpn > 0:
    tmpn /= 10
    len += 1

base, digit, cur = 300, len / m, len % m

while (cur + m <= base) and (digit > 0):
    cur += m
    digit -= 1

div = 10 ** (digit * m)

tmpn = n / div

x = int(float(tmpn) ** (1.0 / m))

x *= (10 ** digit)

while True:
    x0 = x
    x = x + x * (n - x ** m) / (n * m)
    if x == x0:
        break

while (x + 1) ** m <= n:
    x = x + 1
print x

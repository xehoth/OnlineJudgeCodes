from math import log2
n, l, r = map(int, input().split())
a = list(map(int, list(bin(n)[2:])))
ans = 0
for i in range(l, r + 1):
    ans += a[int(log2(i & -i))]
print(ans)
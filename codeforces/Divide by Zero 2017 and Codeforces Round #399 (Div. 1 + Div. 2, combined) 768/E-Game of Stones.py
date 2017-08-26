from math import sqrt
k = 0
for t in range(int(input())): k ^= int(int(sqrt(8 * int(input()) + 1) - 1) / 2)
print("NO" if k != 0 else "YES")
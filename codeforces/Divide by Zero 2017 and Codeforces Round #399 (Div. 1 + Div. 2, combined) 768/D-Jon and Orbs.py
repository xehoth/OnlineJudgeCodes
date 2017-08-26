k, q = map(int, input().split())
dp = [[0.0 for i in range(k + 1)] for j in range(10000)]
dp[0][0] = 1.0
for i in range(1, 10000):
    for j in range(1, k + 1):
        dp[i][j] = dp[i - 1][j] * j / k + dp[i - 1][j - 1] * (k - j + 1) / k
for t in range(q):
    p = int(input())
    for i in range(10000):
        if p <= dp[i][k] * 2000:
            print(i)
            break
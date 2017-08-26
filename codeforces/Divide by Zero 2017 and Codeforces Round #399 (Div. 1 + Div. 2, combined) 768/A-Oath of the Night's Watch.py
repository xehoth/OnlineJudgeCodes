n = int(input())
i = list(map(int, input().split()))
print(max(0, n - i.count(max(i)) - i.count(min(i))))
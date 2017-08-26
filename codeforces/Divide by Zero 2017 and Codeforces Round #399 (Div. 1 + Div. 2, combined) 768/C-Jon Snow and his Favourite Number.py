n, k, x = map(int, input().split())
rangers = list(map(int, input().split()))
for i in range(min(k, 8 + (k & 3))):
    rangers.sort()
    rangers = [rangers[i] if (i & 1) else rangers[i] ^ x for i in range(n)]
    print(rangers)
rangers.sort()
print(rangers[-1], rangers[0])
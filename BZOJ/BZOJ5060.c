int n, m, k, i = 1;
int main() {
    scanf("%d%d%d", &n, &m, &k);
    if (n < 1 || (n == 1 && m)) {
        printf("0\n");
    } else if (k == 0 || m == 0) {
        printf("1\n%d", n);
    } else if (m >= n - 1) {
        printf("%d\n", n - 1);
        for (; i < n; i++) printf("%d ", i);
    } else {
        printf("%d\n", m);
        for (i = n - m; i < n; i++) printf("%d ", i);
    }
    return 0;
}
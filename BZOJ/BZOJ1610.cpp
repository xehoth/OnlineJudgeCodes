#include <bits/stdc++.h>
using namespace std;
double k[20001], e = 1e-7;
int n, x[201], y[201], cnt, sum = 1;
int main() {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) scanf("%d%d", &x[i], &y[i]);
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++) {
            if (abs(x[i] - x[j]) > e)
                k[cnt++] = (y[i] - y[j]) * 1.0 / (x[i] - x[j]);
            else
                k[cnt++] = 1e15;
        }
    sort(k, k + cnt);
    for (int i = 1; i < cnt; i++)
        if (k[i] - k[i - 1] > e) sum++;
    printf("%d\n", sum);
}
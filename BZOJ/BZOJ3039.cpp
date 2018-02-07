#include <cstdio>
#include <iostream>
int a[1001][1001];
int s[1001], l[1001];
int n, m, x, ans = 0;
void work(int h[]) {
    int top = 0, len = 0;
    s[top] = 0;
    l[top] = 0;
    for (int i = 1; i <= m; i++) {
        if (h[i] >= s[top]) {
            s[++top] = h[i];
            l[top] = 1;
        } else {
            len = 0;
            while (top && s[top] > h[i]) {
                len += l[top];
                ans = std::max(ans, len * s[top]);
                top--;
            }
            s[++top] = h[i];
            l[top] = len + 1;
        }
    }
    len = 0;
    while (top) {
        len += l[top];
        ans = std::max(ans, len * s[top]);
        top--;
    }
}
int main() {
    // freopen("sample/1.in", "r", stdin);
    int p;
    char ch[1];
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            scanf("%s", ch);
            if (ch[0] == 'F') a[i][j] = a[i - 1][j] + 1;
        }
    for (int i = 1; i <= n; i++) work(a[i]);
    printf("%d", 3 * ans);
    return 0;
}
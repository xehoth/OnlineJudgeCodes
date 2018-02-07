#include <algorithm>
#include <cstdio>

const int MAXN = 200000;
const int MAXLOGN = 18;

struct Fighter {
    int id, l, r, ans;
    Fighter *next[MAXLOGN + 1];
} fighters[MAXN * 2];

int n, m, ans[MAXN];

bool operator<(const Fighter &a, const Fighter &b) { return a.l < b.l; }

bool operator<(int r, const Fighter &a) { return r < a.l; }

int main() {
    scanf("%d %d", &n, &m);
    for (int i = 0; i < n; i++) {
        Fighter &a = fighters[i];
        a.id = i;
        scanf("%d %d", &a.l, &a.r);
        if (a.l > a.r) a.r += m;
        fighters[i + n].l = a.l + m, fighters[i + n].r = a.r + m;
    }

    std::sort(fighters, fighters + n * 2);

    for (int i = 0; i < n * 2; i++) {
        Fighter &a = fighters[i];
        a.next[0] = std::upper_bound(fighters, fighters + n * 2, a.r) - 1;
    }

    for (int j = 1; (1 << j) <= n * 2; j++) {
        for (int i = 0; i < n * 2; i++) {
            Fighter &a = fighters[i];
            if (a.next[j - 1]) {
                a.next[j] = a.next[j - 1]->next[j - 1];
            }
        }
    }

    for (int i = 0; i < n; i++) {
        Fighter *p = &fighters[i];
        for (int j = MAXLOGN; j >= 0; j--) {
            if (p->next[j] && p->next[j]->r < fighters[i].l + m) {
                ans[fighters[i].id] += (1 << j);
                p = p->next[j];
            }
        }
    }
    for (int i = 0; i < n; i++) {
        printf("%d ", ans[i] + 2);
    }
    putchar('\n');
    return 0;
}

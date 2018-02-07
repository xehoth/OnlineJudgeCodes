#include <bits/stdc++.h>

using namespace std;

inline void R(int &v) {
    static char ch;
    v = 0;
    bool p = 0;
    do {
        ch = getchar();
        if (ch == '-') p = 1;
    } while (!isdigit(ch));
    while (isdigit(ch)) {
        v = (v + (v << 2) << 1) + (ch ^ '0');
        ch = getchar();
    }
    if (p) v = -v;
}

inline void R(long long &v) {
    static char ch;
    v = 0;
    bool p = 0;
    do {
        ch = getchar();
        if (ch == '-') p = 1;
    } while (!isdigit(ch));
    while (isdigit(ch)) {
        v = (v + (v << 2) << 1) + (ch ^ '0');
        ch = getchar();
    }
    if (p) v = -v;
}

int n;
long long a[1000006], b[1000006];
long long dp[1000006][2];
int main() {
    R(n);
    for (int i = 1; i <= n; ++i) R(a[i]);
    for (int i = 1; i <= n; ++i) {
        R(b[i]);
        if (abs(b[i] - a[i - 1]) == 0 || (abs(b[i] - a[i + 1]) == 0)) {
            cout << -1;
            return 0;
        }
    }
    for (int i = 1; i <= n; ++i) dp[i][0] = dp[i][1] = (1LL << 62);
    dp[1][0] = 0;
    dp[1][1] = a[1] - b[1];
    for (int i = 2; i <= n; ++i) {
        // dp[i - 1][1] = min(dp[i - 1][1], dp[i - 1][0] + (a[i] - b[i]));
        // dp[i - 1][0] = min(dp[i - 1][0], dp[i - 1][1] + (a[i] - b[i]));
        dp[i][1] = min(dp[i][1], dp[i][0] + (a[i] - b[i]));
        dp[i][0] = min(dp[i][0], dp[i][1] + (a[i] - b[i]));
        if (a[i - 1] <= a[i]) dp[i][1] = min(dp[i][1], dp[i - 1][1]);
        if (b[i - 1] >= b[i]) dp[i][0] = min(dp[i][0], dp[i - 1][0]);
        dp[i][1] = min(dp[i][1], dp[i][0] + (a[i] - b[i]));
        dp[i][0] = min(dp[i][0], dp[i][1] + (a[i] - b[i]));
        //  dp[i][1] = min(dp[i][1], dp[i][0] + (a[i] - b[i]));
        //  dp[i][0] = min(dp[i][0], dp[i][1] + (a[i] - b[i]));
    }
    if (dp[n][0] >= 10000000000000000)
        cout << -1;
    else
        cout << dp[n][0];
    return 0;
}

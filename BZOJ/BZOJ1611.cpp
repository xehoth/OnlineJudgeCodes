#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#define INF 0x3ffffff
using namespace std;
char ch_buffer;
bool signum;
inline void readInt(int& l) {
    l = 0;
    do
        ch_buffer = cin.get();
    while (ch_buffer < '0' ||
           ch_buffer > '9' && ch_buffer != '0' && ch_buffer != '-');
    if (ch_buffer == '-') signum = true, ch_buffer = cin.get();
    while (ch_buffer >= '0' && ch_buffer <= '9')
        l = (l << 1) + (l << 3) + ch_buffer - '0', ch_buffer = cin.get();
    if (signum) l = -l, signum = false;
}
int meteor[500][500], dis[500][500];
int n;
const int dx[] = {-1, 0, 1, 0};
const int dy[] = {0, -1, 0, 1};
vector<pair<int, int> > vc;
inline void move(int x, int y, int t) {
    if ((x >= 0) && (y >= 0) && (t < meteor[x][y]) && (t < dis[x][y])) {
        if (meteor[x][y] == INF) cout << t, exit(0);
        dis[x][y] = t;
        vc.push_back(pair<int, int>(x, y));
    }
}
inline void init() {
    // freopen("meteor.in","r",stdin);
    // freopen("meteor.out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    readInt(n);
    for (register int i = 0; i < 500; i++)
        fill(meteor[i], meteor[i] + 500, INF), fill(dis[i], dis[i] + 500, INF);
    for (register int i = 0, x_i, y_i, t; i < n; i++) {
        readInt(x_i), readInt(y_i), readInt(t);
        meteor[x_i][y_i] = min(meteor[x_i][y_i], t);
        if (x_i > 0) meteor[x_i - 1][y_i] = min(meteor[x_i - 1][y_i], t);
        if (y_i > 0) meteor[x_i][y_i - 1] = min(meteor[x_i][y_i - 1], t);
        meteor[x_i + 1][y_i] = min(meteor[x_i + 1][y_i], t);
        meteor[x_i][y_i + 1] = min(meteor[x_i][y_i + 1], t);
    }
    move(0, 0, 0);
}
int main() {
    init();
    for (register int i = 0; i < vc.size(); i++) {
        for (register int j = 0; j < 4; j++) {
            pair<int, int>* pir = &vc[i];
            move(pir->first + dx[j], pir->second + dy[j],
                 dis[pir->first][pir->second] + 1);
        }
    }
    cout << "-1";
    return 0;
}

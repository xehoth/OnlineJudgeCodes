#include <bits/stdc++.h>

#include <bits/stdc++.h>

#ifdef __linux__
unsigned long GetTickCount() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
#else
#include <Windows.h>
#endif

int main() {
    char *seed = new char;
    srand((long)seed ^ time(0) ^ GetTickCount());
    delete seed;
    return 0;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    char *seed = new char;
    std::mt19937 engine(time(0) | (long)seed);
    delete seed;
    int n = 180, c = 50, m = 40000;
    std::uniform_int_distribution<> dis(1, 1000000000);
    auto gen = std::bind(dis, engine);
    std::cout << n << ' ' << m << ' ' << c << '\n';
    for (int i = 1; i <= m; i++)
        std::cout << engine() % n + 1 << ' ' << engine() % n + 1 << ' ' << engine() % c + 1 << '\n';
    std::vector<int> w;
    for (int i = 1; i < 2; i++) w.push_back(0);
    for (int i = 2; i <= c; i++) w.push_back(gen());
    std::sort(w.begin(), w.end());
    for (int v : w) std::cout << v << ' ';
    return 0;
}
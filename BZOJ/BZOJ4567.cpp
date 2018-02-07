#include <algorithm>
#include <cstdio>
#include <cstring>
#include <vector>

const int MAXN = 100000;
const int MAXL = 510000;
const int CHARSET_SIZE = 26;

struct Node {
    Node *ch[CHARSET_SIZE];
    std::vector<Node *> chWord;
    int size, wordID;
    bool isWord;

    Node() : ch(), size(0), isWord(false) {}
} _pool[MAXL + 1], *_curr = _pool, *rt = new (_curr++) Node;

inline void insert(char *begin, char *end) {
    Node *v = rt;
    for (char *p = begin; p != end; p++) {
        v->size++;
        if (!v->ch[*p]) {
            v->ch[*p] = new (_curr++) Node;
        }
        v = v->ch[*p];
    }
    v->size++;
    v->isWord = true;
}

long long ans;

inline bool compare(Node *a, Node *b) {
    int x = a ? a->size : 0;
    int y = b ? b->size : 0;
    return x < y;
}

inline void dfs(Node *v, Node *lastPrefix) {
    if (v->isWord) {
        lastPrefix->chWord.push_back(v);
    }

    for (int i = 0; i < CHARSET_SIZE; i++) {
        if (v->ch[i]) {
            dfs(v->ch[i], v->isWord ? v : lastPrefix);
        }
    }
}

inline void dfs2(Node *v, int dfnFa) {
    static int ts = 0;
    int dfn = ts++;  // root's dfn is 0

    ans += dfn - dfnFa;

    std::sort(v->chWord.begin(), v->chWord.end(), compare);
    for (size_t i = 0; i < v->chWord.size(); i++) {
        dfs2(v->chWord[i], dfn);
    }
}

int main() {
    int n;
    scanf("%d", &n);
    while (n--) {
        static char s[MAXL + 1];
        scanf("%s", s);
        int len = strlen(s);
        for (int i = 0; i < len; i++) s[i] -= 'a';

        std::reverse(s, s + len);
        // puts(s);

        insert(s, s + len);
    }

    dfs(rt, rt);
    dfs2(rt, 0);

    printf("%lld\n", ans);
}
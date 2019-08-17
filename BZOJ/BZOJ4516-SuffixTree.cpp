#include <bits/stdc++.h>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        if (iosig) x = -x;
        return *this;
    }

    inline void print(char c) {
        if (oh == obuf + SIZE) {
            fwrite(obuf, 1, SIZE, stdout);
            oh = obuf;
        }
        *oh++ = c;
    }

    template <typename T>
    inline void print(T x) {
        static int buf[21], cnt;
        if (x != 0) {
            if (cnt < 0) {
                print('-');
                x = -x;
            }
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
            while (cnt) print((char)buf[cnt--]);
        } else {
            print('0');
        }
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int POOL_SIZE = 1024 * 1024 * 20;

char pool[POOL_SIZE];

void *operator new(size_t size) {
    static char *s = pool;
    char *t = s;
    s += size;
    return t;
}

void operator delete(void *) {}

void operator delete(void *, size_t) {}

const int MAXN = 100000 + 9;
const int INF = 0x3f3f3f3f;
const int SIGMA = 26;

// Ukkonen's Algorithm
// https://stackoverflow.com/questions/9452701/ukkonens-suffix-tree-algorithm-in-plain-english/9513423#9513423
struct SuffixTree {
    struct Node {
        // [start, end)
        int start, end;

        std::map<int, Node *> c;
        Node *fa, *link;

        // 表示的长度
        int length(int curPos) { return std::min(end, curPos + 1) - start; }

        // 使用不回收的静态内存池保证 c, fa, link 为 NULL
        Node(int start, int end = INF) : start(start), end(end) {}
    };

    // Ukkonen 中当前字符编号
    int curPos;
    int text[MAXN];

    Node *activeNode, *needSuffixLink, *root;
    int activeEdge, activeLen, remain;

    long long distinctSubstrings;

    int tail;

    int getActiveEdge() { return text[activeEdge]; }

    void addSuffixLink(Node *p) {
        if (needSuffixLink) needSuffixLink->link = p;
        needSuffixLink = p;
    }

    bool walkDown(Node *p) {
        int len = p->length(curPos);
        if (activeLen < len) {
            return false;
        }
        activeEdge += len;
        activeLen -= len;
        activeNode = p;
        return true;
    }

    void init() {
        needSuffixLink = NULL;
        curPos = -1;
        remain = activeEdge = activeLen = 0;
        root = activeNode = new Node(-1, -1);
    }

    void insertDone() {
        remain--;
        // Rule 1
        if (activeNode == root && activeLen > 0) {
            activeLen--;
            activeEdge = curPos - remain + 1;
        } else {
            activeNode = activeNode->link ? activeNode->link : root;
        }
    }

    void extend(int c) {
        text[++curPos] = c;
        needSuffixLink = NULL;
        distinctSubstrings += tail;
        for (remain++; remain > 0;) {
            if (activeLen == 0) {
                activeEdge = curPos;
            }

            if (activeNode->c[getActiveEdge()] == NULL) {
                Node *leaf = new Node(curPos);
                activeNode->c[getActiveEdge()] = leaf;
                leaf->fa = activeNode;

                // Rule 2
                addSuffixLink(activeNode);
                tail++;
            } else {
                Node *p = activeNode->c[getActiveEdge()];
                // Observation 2
                if (walkDown(p)) continue;
                // Observation 1
                if (text[p->start + activeLen] == c) {
                    activeLen++;
                    // Observation 3
                    addSuffixLink(activeNode);
                    break;
                }

                // split edge
                Node *split = new Node(p->start, p->start + activeLen);
                activeNode->c[getActiveEdge()] = split;
                split->fa = activeNode;
                Node *leaf = new Node(curPos);
                split->c[c] = leaf;
                leaf->fa = split;
                p->start += activeLen;
                split->c[text[p->start]] = p;
                p->fa = split;
                // Rule 2
                addSuffixLink(split);
                tail++;
            }

            distinctSubstrings++;
            insertDone();
        }
    }
} d;

int main() {
    int q;
    d.init();
    io >> q;
    for (int i = 1, c; i <= q; i++) {
        io >> c;
        d.extend(c);
        io << d.distinctSubstrings << '\n';
    }
    return 0;
}

#include <algorithm>
#include <cstdio>

typedef unsigned int uint;

const uint MAXN = 100000;
const uint MAXM = 100000;

void print(void *node);

template <typename T>
struct splay_t {
    struct node_t {
        node_t *lchild, *rchild, *parent, **root;
        T value;
        uint size;
        bool reversed;
        bool bound;

        node_t(node_t *parent, node_t **root, const T &value,
               bool bound = false)
            : parent(parent),
              lchild(NULL),
              rchild(NULL),
              root(root),
              value(value),
              size(1),
              reversed(false),
              bound(bound) {}

        ~node_t() {
            if (lchild) {
                delete lchild;
            }

            if (rchild) {
                delete rchild;
            }
        }

        void maintain() { size = lsize() + rsize() + 1; }

        uint lsize() { return lchild ? lchild->size : 0; }

        uint rsize() { return rchild ? rchild->size : 0; }

        node_t *&child(uint x) { return !x ? lchild : rchild; }

        node_t *grandparent() { return !parent ? NULL : parent->parent; }

        uint relation() { return this == parent->lchild ? 0 : 1; }

        node_t *pushdown() {
            if (reversed) {
                std::swap(lchild, rchild);

                if (lchild) {
                    lchild->reversed ^= 1;
                }

                if (rchild) {
                    rchild->reversed ^= 1;
                }

                reversed = false;
            }

            return this;
        }

        void rotate() {
            parent->pushdown();
            pushdown();

            node_t *old = parent;
            uint x = relation();

            if (grandparent()) {
                grandparent()->child(old->relation()) = this;
            }
            parent = grandparent();

            old->child(x) = child(x ^ 1);
            if (child(x ^ 1)) {
                child(x ^ 1)->parent = old;
            }

            child(x ^ 1) = old;
            old->parent = this;

            old->maintain();
            maintain();

            if (!parent) {
                *root = this;
            }
        }

        node_t *splay(node_t **target = NULL) {
            if (!target) {
                target = root;
            }

            while (this != *target) {
                parent->pushdown();

                if (parent == *target) {
                    rotate();
                } else if (relation() == parent->relation()) {
                    parent->rotate(), rotate();
                } else {
                    rotate(), rotate();
                }
            }

            return *target;
        }
    } * root;

    ~splay_t() { delete root; }

    void build(const T *a, uint n) {
        root = build(a, 1, n, NULL);

        node_t **lbound = &root, *lbound_parent = NULL;
        while (*lbound) {
            lbound_parent = *lbound;
            lbound_parent->size++;
            lbound = &(*lbound)->lchild;
        }
        *lbound = new node_t(lbound_parent, &root, 0, true);

        node_t **rbound = &root, *rbound_parent = NULL;
        while (*rbound) {
            rbound_parent = *rbound;
            rbound_parent->size++;
            rbound = &(*rbound)->rchild;
        }
        *rbound = new node_t(rbound_parent, &root, 0, true);
    }

    node_t *build(const T *a, uint l, uint r, node_t *parent) {
        if (l > r) {
            return NULL;
        }

        uint mid = l + ((r - l) >> 1);

        node_t *node = new node_t(parent, &root, a[mid - 1]);
        if (l != r) {
            node->lchild = build(a, l, mid - 1, node);
            node->rchild = build(a, mid + 1, r, node);
            node->maintain();
        }

        return node;
    }

    node_t *select(uint k) {
        k++;
        node_t *node = root;
        while (k != node->pushdown()->lsize() + 1) {
            if (k < node->lsize() + 1) {
                node = node->lchild;
            } else {
                k -= node->lsize() + 1;
                node = node->rchild;
            }
        }

        return node->splay();
    }

    node_t *select(uint l, uint r) {
        node_t *lbound = select(l - 1);
        node_t *rbound = select(r + 1);

        lbound->splay();
        rbound->splay(&lbound->rchild);

        return rbound->lchild;
    }

    void reverse(uint l, uint r) {
        node_t *range = select(l, r);
        range->reversed ^= 1;
    }

    void fetch(T *a) { dfs(a, root); }

    void dfs(T *&a, node_t *node) {
        if (node) {
            node->pushdown();

            dfs(a, node->lchild);

            if (!node->bound) {
                *a++ = node->value;
            }

            dfs(a, node->rchild);
        }
    }
};

void dfs(splay_t<uint>::node_t *node, uint depth = 0) {
    if (node) {
        dfs(node->rchild, depth + 1);

        for (uint i = 0; i < depth; i++) {
            putchar(' ');
        }
        printf("%d : %u\n", node->value, node->size);

        dfs(node->lchild, depth + 1);
    }
}

void print(void *node) {
    puts("------------------------------------------");
    dfs((splay_t<uint>::node_t *)node);
    puts("------------------------------------------");
}

uint n, m;
splay_t<uint> splay;
uint a[MAXN];

int main() {
    scanf("%u %u", &n, &m);

    for (uint i = 0; i < n; i++) {
        a[i] = i + 1;
    }

    splay.build(a, n);

    for (uint i = 0; i < m; i++) {
        uint l, r;
        scanf("%u %u", &l, &r);
        splay.reverse(l, r);
    }

    splay.fetch(a);

    for (uint i = 0; i < n; i++) {
        printf("%u ", a[i]);
    }

    return 0;
}

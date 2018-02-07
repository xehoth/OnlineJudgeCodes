/*
 * created by xehoth on 10-01-2017
 */
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
typedef __gnu_pbds::tree<int, __gnu_pbds::null_mapped_type, std::less<int>,
                         __gnu_pbds::rb_tree_tag,
                         __gnu_pbds::tree_order_statistics_node_update>
    RedBlackTree;

int main() {
    RedBlackTree tree;
    static int pos[100001], val[500001], a, x, n, m;
    char op[7];

    scanf("%d%d", &n, &m);
    for (register int i = 1; i <= n; i++) {
        scanf("%d", &val[i + 100000]);
        pos[val[i + 100000]] = i + 100000;
        tree.insert(i + 100000);
    }

    for (int i = 1; i <= m; i++) {
        scanf("%s", op);
        if (op[0] == 'T') {
            scanf("%d", &a);
            int tmp = *tree.find_by_order(0);
            tree.erase(tree.lower_bound(pos[a]));
            pos[a] = tmp - 1;
            val[tmp - 1] = a;
            tree.insert(tmp - 1);
        } else if (op[0] == 'B') {
            scanf("%d", &a);
            int tmp = *tree.find_by_order(n - 1);
            tree.erase(tree.lower_bound(pos[a]));
            pos[a] = tmp + 1;
            val[tmp + 1] = a;
            tree.insert(tmp + 1);
        } else if (op[0] == 'I') {
            scanf("%d%d", &a, &x);
            if (x == 0) continue;
            RedBlackTree::iterator it = tree.lower_bound(pos[a]), itBeside = it;
            if (x == 1)
                itBeside++;
            else
                itBeside--;
            std::swap(val[*it], val[*itBeside]);
            std::swap(pos[val[*it]], pos[val[*itBeside]]);
        } else if (op[0] == 'A') {
            scanf("%d", &a);
            printf("%d\n", tree.order_of_key(pos[a]));
        } else {
            scanf("%d", &a);
            printf("%d\n", val[*tree.find_by_order(a - 1)]);
        }
    }
    return 0;
}

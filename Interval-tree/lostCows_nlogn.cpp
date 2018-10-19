#include <iostream>
#include <cassert>
#define MAX 100010
using namespace std;

int pre_num[MAX];

struct Node
{
    int l;
    int r;
    int num; // # of vacant nodes in this range
    int mid() { return (l + r) / 2; }
} tree[4 * MAX];

void buildTree(int root, int L, int R)
{
    tree[root].l = L;
    tree[root].r = R;
    tree[root].num = R - L + 1;
    if (L != R)
    {
        buildTree(2 * root, L, (L + R) / 2);
        buildTree(2 * root + 1, (L + R) / 2 + 1, R);
    }
}

int get_pos(int root, int pre)
{
    tree[root].num--;
    if (tree[root].l == tree[root].r)
    {
        assert(pre == 0);
        return tree[root].l;
    }
    int left_num = tree[2 * root].num;
    return pre < left_num ? get_pos(2 * root, pre) : get_pos(2 * root + 1, pre - left_num);
}

int main(int argc, char const *argv[])
{
    int n;
    while (cin >> n)
    {
        pre_num[1] = 0;
        for (int i = 2; i <= n; ++i)
            cin >> pre_num[i];
        buildTree(1, 1, n);
        for (int i = n; i >= 1; --i)
            pre_num[i] = get_pos(1, pre_num[i]);
        for (int i = 1; i <= n; ++i)
            cout << pre_num[i] << endl;
    }
    return 0;
}

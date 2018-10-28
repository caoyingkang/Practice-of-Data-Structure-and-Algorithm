#include <iostream>
#include <algorithm>
#include <map>
#define MAXP 10010
#define MAXL (MAXP * 4)

using namespace std;

int terminals[2 * MAXP];
int unique_tmnls[2 * MAXP];
map<int, int> tmnl2idx;

struct Node
{
    int L, R;
    bool covered;
    inline int mid() { return (L + R) >> 1; }
};

Node tree[4 * MAXL];

void BuildTree(int root, int l, int r)
{
    tree[root].L = l;
    tree[root].R = r;
    tree[root].covered = false;
    if (l == r)
        return;
    BuildTree(root << 1, l, tree[root].mid());
    BuildTree((root << 1) + 1, tree[root].mid() + 1, r);
}

bool add_poster(int root, int l, int r)
{
    if (tree[root].covered)
        return false;
    if (tree[root].L == l && tree[root].R == r)
        return tree[root].covered = true;
    int mid = tree[root].mid();
    bool ret;
    if (r <= mid)
        ret = add_poster(root << 1, l, r);
    else if (l > mid)
        ret = add_poster((root << 1) + 1, l, r);
    else
    {
        bool ret1 = add_poster(root << 1, l, mid),
             ret2 = add_poster((root << 1) + 1, mid + 1, r);
        ret = ret1 || ret2;
    }
    tree[root].covered = tree[root << 1].covered &&
                         tree[(root << 1) + 1].covered;
    return ret;
}

int main(int argc, char const *argv[])
{
    int c;
    cin >> c;
    for (; c > 0; --c)
    {
        tmnl2idx.clear();
        int np; // # of posters
        cin >> np;
        for (int i = 1; i <= np; ++i)
        {
            int l, r;
            cin >> l >> r;
            unique_tmnls[(i << 1) - 1] = terminals[(i << 1) - 1] = l;
            unique_tmnls[i << 1] = terminals[i << 1] = r;
        }
        sort(unique_tmnls + 1, unique_tmnls + 1 + 2 * np);
        int sz = unique(unique_tmnls + 1,
                        unique_tmnls + 1 + 2 * np) -
                 (unique_tmnls + 1);
        int nl = 0; // # of leaf nodes
        for (int i = 1; i <= sz; ++i)
        {
            tmnl2idx[unique_tmnls[i]] = ++nl;
            if (i < sz && unique_tmnls[i + 1] - unique_tmnls[i] > 1)
                ++nl;
        }
        BuildTree(1, 1, nl);
        int visible = 0;
        for (int i = np; i >= 1; --i)
        {
            int l = tmnl2idx[terminals[(i << 1) - 1]];
            int r = tmnl2idx[terminals[i << 1]];
            if (add_poster(1, l, r))
                ++visible;
        }
        cout << visible << endl;
    }
    return 0;
}

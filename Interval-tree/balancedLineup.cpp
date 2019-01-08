#include <cstdio>
#include <cstring>
#include <algorithm>
#define MAXN 50010
#define INF 0xffffff
#define NEGINF (-INF)

using namespace std;

int n, Q;

struct Node
{
    int l, r;
    int maxv, minv;
    int mid() { return (r + l) / 2; }
} tree[4 * MAXN];

void BuildTree(int root, int L, int R)
{
    tree[root].l = L;
    tree[root].r = R;
    tree[root].maxv = NEGINF;
    tree[root].minv = INF;
    int mid = tree[root].mid();
    if (L != R)
    {
        BuildTree(root * 2, L, mid);
        BuildTree(root * 2 + 1, mid + 1, R);
    }
}

void Insert(int root, int i, int h)
{
    int mid = tree[root].mid();
    if (tree[root].l == tree[root].r)
    {
        tree[root].maxv = tree[root].minv = h;
    }
    else
    {
        if (i <= mid)
            Insert(root * 2, i, h);
        else
            Insert(root * 2 + 1, i, h);
        tree[root].maxv = max(tree[root * 2].maxv,
                              tree[root * 2 + 1].maxv);
        tree[root].minv = min(tree[root * 2].minv,
                              tree[root * 2 + 1].minv);
    }
}

void Query(int root, int L, int R, int &minv, int &maxv)
{
    if (L == tree[root].l && R == tree[root].r)
    {
        minv = min(minv, tree[root].minv);
        maxv = max(maxv, tree[root].maxv);
    }
    else
    {
        int mid = tree[root].mid();
        if (R <= mid)
        {
            Query(root * 2, L, R, minv, maxv);
        }
        else if (L > mid)
        {
            Query(root * 2 + 1, L, R, minv, maxv);
        }
        else
        {
            Query(root * 2, L, mid, minv, maxv);
            Query(root * 2 + 1, mid + 1, R, minv, maxv);
        }
    }
}

int main(int argc, char const *argv[])
{
    scanf("%d%d", &n, &Q);
    BuildTree(1, 1, n);
    int i, j, k;
    int h, a, b;
    int minv, maxv;
    for (i = 1; i <= n; ++i)
    {
        scanf("%d", &h);
        Insert(1, i, h);
    }
    for (i = 1; i <= Q; ++i)
    {
        scanf("%d%d", &a, &b);
        minv = INF;
        maxv = NEGINF;
        Query(1, a, b, minv, maxv);
        printf("%d\n", maxv - minv);
    }
    return 0;
}

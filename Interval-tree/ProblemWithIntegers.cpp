#include <cstdio>
#include <cstring>
#define MAXN 100010

using namespace std;

int n, Q;

struct Node
{
    int l, r;
    long long sum;
    long long inc;
    int mid() { return (l + r) / 2; }
    int num() { return r - l + 1; }
} tree[4 * MAXN];

void buildTree(int root, int L, int R)
{
    tree[root].l = L;
    tree[root].r = R;
    tree[root].sum = tree[root].inc = 0;
    if (L != R)
    {
        int mid = tree[root].mid();
        buildTree(root * 2, L, mid);
        buildTree(root * 2 + 1, mid + 1, R);
    }
}

void Add(int root, int L, int R, long long ad)
{
    if (L == tree[root].l && R == tree[root].r)
    {
        tree[root].inc += ad;
    }
    else
    {
        tree[root * 2].inc += tree[root].inc;
        tree[root * 2 + 1].inc += tree[root].inc;
        tree[root].sum += tree[root].inc * tree[root].num() +
                          ad * (R - L + 1);
        tree[root].inc = 0;
        int mid = tree[root].mid();
        if (R <= mid)
        {
            Add(root * 2, L, R, ad);
        }
        else if (L > mid)
        {
            Add(root * 2 + 1, L, R, ad);
        }
        else
        {
            Add(root * 2, L, mid, ad);
            Add(root * 2 + 1, mid + 1, R, ad);
        }
    }
}

long long getSum(int root, int L, int R)
{
    if (L == tree[root].l && R == tree[root].r)
    {
        return tree[root].sum + tree[root].inc * tree[root].num();
    }
    else
    {
        tree[root * 2].inc += tree[root].inc;
        tree[root * 2 + 1].inc += tree[root].inc;
        tree[root].sum += tree[root].inc * tree[root].num();
        tree[root].inc = 0;
        int mid = tree[root].mid();
        long long ret = 0;
        if (R <= mid)
        {
            ret += getSum(root * 2, L, R);
        }
        else if (L > mid)
        {
            ret += getSum(root * 2 + 1, L, R);
        }
        else
        {
            ret += getSum(root * 2, L, mid);
            ret += getSum(root * 2 + 1, mid + 1, R);
        }
        return ret;
    }
}

int main(int argc, char const *argv[])
{
    scanf("%d%d", &n, &Q);
    buildTree(1, 1, n);
    int i, j, k;
    int ad;
    for (i = 1; i <= n; ++i)
    {
        scanf("%d", &ad);
        Add(1, i, i, ad);
    }
    char ch;
    int a, b;
    for (i = 1; i <= Q; ++i)
    {
        while (scanf("%c", &ch), (ch != 'Q' && ch != 'C'))
            ;
        if (ch == 'Q')
        {
            scanf("%d%d", &a, &b);
            printf("%lld\n", getSum(1, a, b));
        }
        else
        {
            scanf("%d%d%d", &a, &b, &ad);
            Add(1, a, b, ad);
        }
    }
    return 0;
}

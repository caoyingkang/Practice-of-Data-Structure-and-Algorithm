#include <cstdio>
#include <algorithm>
#include <vector>
#include <cassert>
#define MAX 100010

using namespace std;

int n;
int a[MAX];
vector<int> sorted_a;
bool isfound;

struct Node
{
    int L, R;
    vector<int> vec;
    inline int mid() { return (L + R) >> 1; }
} tree[4 * MAX];

void MergePass(const vector<int> &src1, const vector<int> &src2,
               vector<int> &dest)
{
    int n1 = src1.size(), n2 = src2.size();
    int i = 0, j = 0;
    while (i < n1 && j < n2)
        dest.push_back(src1[i] <= src2[j] ? src1[i++] : src2[j++]);
    while (i < n1)
        dest.push_back(src1[i++]);
    while (j < n2)
        dest.push_back(src2[j++]);
}

void BuildTree(int root, int l, int r)
{
    tree[root].L = l;
    tree[root].R = r;
    if (l == r)
    {
        tree[root].vec.push_back(a[l]);
        return;
    }
    int mid = tree[root].mid();
    BuildTree(root << 1, l, mid);
    BuildTree((root << 1) + 1, mid + 1, r);
    MergePass(tree[root << 1].vec, tree[(root << 1) + 1].vec,
              tree[root].vec);
}

int test(int root, int l, int r, int guess)
{
    // set isfound=false before call this function
    if (tree[root].L == l && tree[root].R == r)
    {
        vector<int>::iterator it = lower_bound(tree[root].vec.begin(),
                                               tree[root].vec.end(), guess);
        if (it != tree[root].vec.end() && *it == guess)
        {
            isfound = true;
            return it - tree[root].vec.begin() + 1;
        }
        else
            return it - tree[root].vec.begin();
    }
    int mid = tree[root].mid();
    if (r <= mid)
        return test(root << 1, l, r, guess);
    else if (l > mid)
        return test((root << 1) + 1, l, r, guess);
    else
        return test(root << 1, l, mid, guess) +
               test((root << 1) + 1, mid + 1, r, guess);
}

int Query(int l, int r, int k)
{
    int left_idx = 0, right_idx = n - 1, middle_idx;
    while (left_idx <= right_idx)
    {
        middle_idx = (left_idx + right_idx) >> 1;
        isfound = false;
        int order = test(1, l, r, sorted_a[middle_idx]);
        if (order < k)
            left_idx = middle_idx + 1;
        else if (order > k)
            right_idx = middle_idx - 1;
        else
        {
            if (isfound)
                return sorted_a[middle_idx];
            else
                right_idx = middle_idx - 1;
        }
    }
    assert(0);
}

int main(int argc, char const *argv[])
{
    int m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i)
        scanf("%d", &a[i]);
    BuildTree(1, 1, n);
    sorted_a = tree[1].vec;
    int l, r, k;
    int guess;
    for (; m > 0; --m)
    {
        scanf("%d%d%d", &l, &r, &k);
        printf("%d\n", Query(l, r, k));
    }
    return 0;
}

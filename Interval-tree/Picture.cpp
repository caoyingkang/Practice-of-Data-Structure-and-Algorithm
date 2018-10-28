#include <iostream>
#include <algorithm>
#include <cstdlib>
#define MAX_REC 5010
#define MAX_LEAF (2 * MAX_REC)

using namespace std;

int y_delim[MAX_LEAF + 1];
int x_delim[MAX_LEAF + 1];

struct YLine
{
    int x, y1, y2;
    bool isleft;
    inline void set(int _x, int _y1, int _y2, bool _isleft)
    {
        x = _x;
        y1 = _y1;
        y2 = _y2;
        isleft = _isleft;
    }
} ylines[MAX_LEAF];

bool cmp_YLine(const YLine &l1, const YLine &l2)
{
    if (l1.x != l2.x)
        return l1.x < l2.x;
    if (l1.isleft != l2.isleft)
        return l1.isleft;
    return l1.y1 < l2.y1;
}

struct XLine
{
    int y, x1, x2;
    bool islow;
    inline void set(int _y, int _x1, int _x2, bool _islow)
    {
        y = _y;
        x1 = _x1;
        x2 = _x2;
        islow = _islow;
    }
} xlines[MAX_LEAF];

bool cmp_XLine(const XLine &l1, const XLine &l2)
{
    if (l1.y != l2.y)
        return l1.y < l2.y;
    if (l1.islow != l2.islow)
        return l1.islow;
    return l1.x1 < l2.x1;
}

template <class ForwardIterator, class T>
ForwardIterator my_bin_search(ForwardIterator first,
                              ForwardIterator last,
                              const T &val)
{
    ForwardIterator L = first;
    ForwardIterator R = last - 1;
    while (L <= R)
    {
        ForwardIterator M = L + (R - L) / 2;
        if (!(*M < val || val < *M))
            return M;
        else if (val < *M)
            R = M - 1;
        else
            L = M + 1;
    }
    return last;
}

struct Node
{
    int L, R;
    int len;
    int covers;
    inline int mid() { return (L + R) >> 1; }
} tree[4 * MAX_LEAF];

void BuildTree(int root, int l, int r)
{
    tree[root].L = l;
    tree[root].R = r;
    tree[root].len = 0;
    tree[root].covers = 0;
    if (l == r)
        return;
    int mid = tree[root].mid();
    BuildTree(root << 1, l, mid);
    BuildTree((root << 1) + 1, mid + 1, r);
}

// delim is either y_delim or x_delim
void Insert_Line(int root, int l, int r, int delim[])
{
    if (tree[root].L == l && tree[root].R == r)
    {
        if (tree[root].covers == 0)
            tree[root].len = delim[r + 1] - delim[l];
        tree[root].covers++;
        return;
    }
    int mid = tree[root].mid();
    if (r <= mid)
        Insert_Line(root << 1, l, r, delim);
    else if (l > mid)
        Insert_Line((root << 1) + 1, l, r, delim);
    else
    {
        Insert_Line(root << 1, l, mid, delim);
        Insert_Line((root << 1) + 1, mid + 1, r, delim);
    }
    if (tree[root].covers == 0)
        tree[root].len = tree[root << 1].len +
                         tree[(root << 1) + 1].len;
}

void Delete_Line(int root, int l, int r)
{
    if (tree[root].L == l && tree[root].R == r)
    {
        tree[root].covers--;
        if (tree[root].covers == 0)
        {
            if (l == r)
                tree[root].len = 0;
            else
                tree[root].len = tree[root << 1].len +
                                 tree[(root << 1) + 1].len;
        }
        return;
    }
    int mid = tree[root].mid();
    if (r <= mid)
        Delete_Line(root << 1, l, r);
    else if (l > mid)
        Delete_Line((root << 1) + 1, l, r);
    else
    {
        Delete_Line(root << 1, l, mid);
        Delete_Line((root << 1) + 1, mid + 1, r);
    }
    if (tree[root].covers == 0)
        tree[root].len = tree[root << 1].len +
                         tree[(root << 1) + 1].len;
}

int main(int argc, char const *argv[])
{
    int n_rec;
    cin >> n_rec;
    if (n_rec == 0)
    {
        cout << 0 << endl;
        return 0;
    }
    int x1, x2, y1, y2;
    for (int i = 1; i <= 2 * n_rec; ++i)
    {
        cin >> x1 >> y1 >> x2 >> y2;
        xlines[i].set(y1, x1, x2, true);
        ylines[i].set(x1, y1, y2, true);
        x_delim[i] = x1;
        y_delim[i] = y1;
        ++i;
        xlines[i].set(y2, x1, x2, false);
        ylines[i].set(x2, y1, y2, false);
        x_delim[i] = x2;
        y_delim[i] = y2;
    }
    // nx, ny: # of leaf nodes in interval tree
    sort(x_delim + 1, x_delim + 1 + 2 * n_rec);
    int nx = (unique(x_delim + 1, x_delim + 1 + 2 * n_rec) - (x_delim + 1)) - 1;
    sort(y_delim + 1, y_delim + 1 + 2 * n_rec);
    int ny = (unique(y_delim + 1, y_delim + 1 + 2 * n_rec) - (y_delim + 1)) - 1;
    sort(xlines + 1, xlines + 1 + 2 * n_rec, cmp_XLine);
    sort(ylines + 1, ylines + 1 + 2 * n_rec, cmp_YLine);

    int perim = 0;
    // calculate perimeter of ylines
    BuildTree(1, 1, ny);
    int last_len = 0;
    for (int i = 1; i <= 2 * n_rec; ++i)
    {
        perim += abs(last_len - tree[1].len);
        last_len = tree[1].len;
        int l = my_bin_search(
                    y_delim + 1, y_delim + 1 + ny + 1, ylines[i].y1) -
                y_delim;
        int r = my_bin_search(
                    y_delim + 1, y_delim + 1 + ny + 1, ylines[i].y2) -
                (y_delim + 1);
        if (ylines[i].isleft)
            Insert_Line(1, l, r, y_delim);
        else
            Delete_Line(1, l, r);
    }
    perim += last_len;

    // calculate perimeter of xlines
    BuildTree(1, 1, nx);
    last_len = 0;
    for (int i = 1; i <= 2 * n_rec; ++i)
    {
        perim += abs(last_len - tree[1].len);
        last_len = tree[1].len;
        int l = my_bin_search(
                    x_delim + 1, x_delim + 1 + nx + 1, xlines[i].x1) -
                x_delim;
        int r = my_bin_search(
                    x_delim + 1, x_delim + 1 + nx + 1, xlines[i].x2) -
                (x_delim + 1);
        if (xlines[i].islow)
            Insert_Line(1, l, r, x_delim);
        else
            Delete_Line(1, l, r);
    }
    perim += last_len;

    cout << perim << endl;
    return 0;
}

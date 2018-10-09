#include <iostream>
#include <algorithm>
#define MAX 1010
using namespace std;

struct Node
{
    int val;
    int pos;
};

int lowbit[MAX];
int C[MAX] = {0};
int len;
Node arr[MAX];

bool comp(const Node &n1, const Node &n2)
{
    return n1.val == n2.val ? n1.pos > n2.pos : n1.val < n2.val;
}

int getMax(int k)
{
    // get maximum element of LIS[1~k]
    return k == 0 ? 0 : max(C[k], getMax(k - lowbit[k]));
}

void Update_C(int k, int val)
{
    if (k > len)
        return;
    C[k] = max(C[k], val);
    Update_C(k + lowbit[k], val);
}

int main(int argc, char const *argv[])
{
    cin >> len;
    for (int i = 1; i <= len; ++i)
    {
        lowbit[i] = i & (-i);
        cin >> arr[i].val;
        arr[i].pos = i;
    }
    sort(arr + 1, arr + len + 1, comp);
    int p;
    for (int i = 1; i <= len; ++i)
    {
        p = arr[i].pos;
        Update_C(p, getMax(p) + 1);
    }
    cout << *max_element(C + 1, C + len + 1) << endl;
    return 0;
}

#include <iostream>
#include <cstring>
#define MAXN 30010

using namespace std;

int n, p;
int parent[MAXN];
int sum[MAXN], under[MAXN];

int GetRoot(int a)
{
    int pa = parent[a];
    if (parent[a] != a)
    {
        parent[a] = GetRoot(pa);
        under[a] += under[pa];
    }
    return parent[a];
}

void Merge(int a, int b)
{
    int ra = GetRoot(a), rb = GetRoot(b);
    if (ra != rb)
    {
        parent[rb] = ra;
        under[rb] += sum[ra];
        sum[ra] += sum[rb];
    }
}

int Query(int a)
{
    int r = GetRoot(a);
    return under[a];
}

int main(int argc, char const *argv[])
{
    int i, j, k;
    int x, y;
    char ch;
    for (i = 0; i < MAXN; ++i)
    {
        parent[i] = i;
        sum[i] = 1;
        under[i] = 0;
    }
    cin >> p;
    for (; p > 0; --p)
    {
        cin >> ch;
        if (ch == 'M')
        {
            cin >> x >> y;
            Merge(y, x);
        }
        else
        {
            cin >> x;
            cout << Query(x) << endl;
        }
    }
    return 0;
}

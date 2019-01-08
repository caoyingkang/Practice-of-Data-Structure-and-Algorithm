#include <iostream>
#include <cstring>
#define MAXN 30010

using namespace std;

int n, m;
int parent[MAXN], sum[MAXN];

int GetRoot(int a)
{
    if (parent[a] != a)
    {
        parent[a] = GetRoot(parent[a]);
    }
    return parent[a];
}

void Merge(int a, int b)
{
    int ra = GetRoot(a), rb = GetRoot(b);
    if (ra != rb)
    {
        parent[rb] = ra;
        sum[ra] += sum[rb];
    }
}

int main(int argc, char const *argv[])
{
    int i, j, k;
    int a, b;
    while (cin >> n >> m, n)
    {
        for (i = 0; i < n; ++i)
        {
            parent[i] = i;
            sum[i] = 1;
        }
        for (i = 0; i < m; ++i)
        {
            cin >> k;
            if (k > 0)
            {
                cin >> a;
                for (j = 1; j < k; ++j)
                {
                    cin >> b;
                    Merge(a, b);
                }
            }
        }
        cout << sum[GetRoot(0)] << endl;
    }
    return 0;
}

#include <cstdio>
using namespace std;

const int MAX = 1010;
int parent[MAX];
int table;

int GetRoot(int a)
{
    if (parent[a] == a)
        return a;
    return parent[a] = GetRoot(parent[a]);
}

void Merge(int a, int b)
{
    if (GetRoot(a) == GetRoot(b))
        return;
    parent[parent[b]] = parent[a];
    --table;
}

int main(int argc, char const *argv[])
{
    int T, n, m, x, y;
    scanf("%d", &T);
    for (; T > 0; --T)
    {
        scanf("%d%d", &n, &m);
        table = n;
        for (int i = 1; i <= n; ++i)
        {
            parent[i] = i;
        }
        for (; m > 0; --m)
        {
            scanf("%d%d", &x, &y);
            Merge(x, y);
        }
        printf("%d\n", table);
    }
    return 0;
}

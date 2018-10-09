#include <cstdio>
#include <vector>
using namespace std;

const int MAX = 1010;
int d2;
int parent[MAX];
int x[MAX];
int y[MAX];
bool rep[MAX] = {false};
vector<int> rep_ls;

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
}

inline bool near(int a, int b)
{
    int xd = x[a] - x[b], yd = y[a] - y[b];
    return xd * xd + yd * yd <= d2;
}

int main(int argc, char const *argv[])
{
    int n;
    scanf("%d%d", &n, &d2);
    d2 *= d2;
    for (int i = 1; i <= n; ++i)
    {
        parent[i] = i;
        scanf("%d%d", &x[i], &y[i]);
    }
    char op;
    int p, q;
    getchar();
    while (scanf("%c", &op) == 1)
    {
        if (op == 'O')
        {
            scanf("%d", &p);
            if (rep[p])
            {
                continue;
                getchar();
            }
            //this node is not repaired yet
            for (int i = 0; i < rep_ls.size(); ++i)
            {
                int a = rep_ls[i];
                if (near(p, a))
                    Merge(p, a);
            }
            //renew repair info
            rep[p] = true;
            rep_ls.push_back(p);
        }
        else
        {
            scanf("%d%d", &p, &q);
            if ((!rep[p]) || (!rep[q]) || (GetRoot(p) != GetRoot(q)))
                printf("FAIL\n");
            else
                printf("SUCCESS\n");
        }
        getchar();
    }
    return 0;
}

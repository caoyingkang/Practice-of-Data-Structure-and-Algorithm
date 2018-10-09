#include <cstdio>

const int MAX = 50010;
int parent[MAX];
int rel[MAX] = {0};

int GetRoot(int a)
{
    if (parent[a] == a) //a is a root
        return a;
    //renew info in parent
    int root = GetRoot(parent[a]);
    //renew this node
    rel[a] = (rel[a] + rel[parent[a]]) % 3;
    parent[a] = root;
    return root;
}

int GetRel(int a, int b)
{
    int rel_r2a = (3 - rel[a]) % 3;
    return (rel[b] + rel_r2a) % 3;
}

void Merge(int a, int b, int rel_b2a)
{
    int ra = GetRoot(a), rb = GetRoot(b);
    int rel_b2ra = (rel_b2a + rel[a]) % 3;
    int rel_rb2b = (3 - rel[b]) % 3;
    rel[rb] = (rel_rb2b + rel_b2ra) % 3;
    parent[rb] = ra;
}

int main(int argc, char const *argv[])
{
    int n, k;
    scanf("%d%d", &n, &k);
    //initialization
    int lies = 0;
    for (int i = 1; i <= n; ++i)
    {
        parent[i] = i;
    }

    for (; k > 0; --k)
    {
        int c, x, y;
        scanf("%d%d%d", &c, &x, &y);
        //for each description
        if (x > n || y > n)
        {
            ++lies;
            continue;
        }
        if (x == y)
        {
            if (c == 2)
                ++lies;
            continue;
        }
        int rel_y2x = c == 1 ? 0 : 2;
        if (GetRoot(x) != GetRoot(y))
        {
            Merge(x, y, rel_y2x);
        }
        else if (GetRel(x, y) != rel_y2x)
        {
            ++lies;
        }
    }
    printf("%d", lies);
    return 0;
}

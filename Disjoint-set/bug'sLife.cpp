#include <stdio.h>
#include <iostream>
using namespace std;

const int MAX = 2010;
int parent[MAX];
bool rel[MAX]; //true: same gender as parent

int GetRoot(int a)
{
    if (parent[a] == a)
        return a;
    int root = GetRoot(parent[a]);
    rel[a] = (rel[a] == rel[parent[a]]);
    return parent[a] = root;
}

void Merge(int a, int b)
{
    //assume a and b are in different trees
    //assume a and b have different gender
    int ra = GetRoot(a), rb = GetRoot(b);
    parent[rb] = ra;
    rel[rb] = (rel[a] != rel[b]);
}

int GetRel(int a, int b)
{
    if (GetRoot(a) != GetRoot(b))
        return -1;                   //undetermined
    return rel[a] == rel[b] ? 1 : 0; //1:same,0:different
}

int main(int argc, char const *argv[])
{
    int S;
    scanf("%d", &S);
    for (int s = 1; s <= S; ++s)
    {
        printf("Scenario #%d:\n", s);
        bool susp = false;
        int nb, ni;
        scanf("%d%d", &nb, &ni);
        //initialization
        for (int i = 1; i <= nb; ++i)
        {
            parent[i] = i;
            rel[i] = true;
        }
        //inference
        int x, y;
        for (; ni > 0; --ni)
        {
            scanf("%d%d", &x, &y);
            if (!susp)
            {
                int gold = GetRel(x, y);
                if (gold == -1)
                    Merge(x, y);
                else if (gold == 1)
                    susp = true;
            }
        }
        cout << (susp ? "S" : "No s") << "uspicious bugs found!" << endl;
        if (s != S)
            cout << endl;
    }
    return 0;
}

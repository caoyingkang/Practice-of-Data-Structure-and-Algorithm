#include <cstdio>
#include <vector>
#include <queue>
#define MAX 110

using namespace std;

struct Edge
{
    int e;
    float r;
    float c;
    Edge(int e, float r, float c) : e(e), r(r), c(c) {}
    Edge() {}
};

int n, s;
float V;
vector<vector<Edge>> G;
typedef vector<Edge>::iterator EdgeIter;
float capital[MAX];
bool att[MAX] = {false}; // back[u] is true if u is attainable from s

void setAtt()
{
    queue<int> qu;
    att[s] = true;
    qu.push(s);
    int u, v;
    while (!qu.empty())
    {
        u = qu.front();
        qu.pop();
        for (EdgeIter it = G[u].begin(); it != G[u].end(); ++it)
        {
            v = it->e;
            if (att[v] == true)
                continue;
            att[v] = true;
            qu.push(v);
        }
    }
}

bool Bellman_ford()
{
    for (int i = 1; i <= n; ++i)
        capital[i] = -1;
    capital[s] = V;
    int u, v;
    float r, c, tmp;
    bool renew;
    for (int k = 1; k < n; ++k)
    {
        renew = false;
        for (u = 1; u <= n; ++u)
        {
            if (att[u] == false || capital[u] < 0)
                continue;
            for (EdgeIter it = G[u].begin(); it != G[u].end(); ++it)
            {
                c = it->c;
                if (capital[u] < c)
                    continue;
                v = it->e;
                r = it->r;
                tmp = (capital[u] - c) * r;
                if (tmp > capital[v])
                {
                    if (v == s)
                        return true;
                    capital[v] = tmp;
                    renew = true;
                }
            }
        }
        if (renew == false)
            return false;
    }
    for (u = 1; u <= n; ++u)
    {
        if (att[u] == false || capital[u] < 0)
            continue;
        for (EdgeIter it = G[u].begin(); it != G[u].end(); ++it)
        {
            c = it->c;
            if (capital[u] < c)
                continue;
            v = it->e;
            r = it->r;
            tmp = (capital[u] - c) * r;
            if (tmp > capital[v])
                return true;
        }
    }
    return false;
}

int main(int argc, char const *argv[])
{
    int m;
    int a, b;
    float r, c;
    scanf("%d%d%d%f", &n, &m, &s, &V);
    G.resize(n + 1);
    while (m--)
    {
        scanf("%d%d", &a, &b);
        scanf("%f%f", &r, &c);
        G[a].emplace_back(b, r, c);
        scanf("%f%f", &r, &c);
        G[b].emplace_back(a, r, c);
    }
    setAtt();
    printf(Bellman_ford() ? "YES" : "NO");
    return 0;
}

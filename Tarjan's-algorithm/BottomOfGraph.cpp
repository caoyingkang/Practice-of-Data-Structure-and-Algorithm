#include <cstdio>
#include <cstring>
#include <vector>
#include <stack>
#include <algorithm>
#define MAXN 5010

using namespace std;
typedef vector<int> Edges;

int dfn[MAXN], low[MAXN], color[MAXN];
vector<Edges> G;
stack<int> stk;
bool instack[MAXN] = {false};
int timeCnt, colorCnt;
bool is_bottom_color[MAXN];
int n;

void Tarjan(int u)
{
    dfn[u] = low[u] = ++timeCnt;
    stk.push(u);
    instack[u] = true;
    int v;
    for (int idx = 0; idx < G[u].size(); ++idx)
    {
        v = G[u][idx];
        if (!dfn[v])
        {
            Tarjan(v);
            low[u] = min(low[u], low[v]);
        }
        else if (instack[v])
            low[u] = min(low[u], dfn[v]);
    }
    if (low[u] == dfn[u])
    {
        ++colorCnt;
        while (true)
        {
            v = stk.top();
            stk.pop();
            instack[v] = false;
            color[v] = colorCnt;
            if (v == u)
                break;
        }
    }
}

void Query()
{
    for (int u = 1; u <= n; ++u)
        for (int idx = 0; idx < G[u].size(); ++idx)
            if (color[u] != color[G[u][idx]])
                is_bottom_color[color[u]] = false;
}

int main(int argc, char const *argv[])
{
    int m, u, v;
    while (scanf("%d", &n))
    {
        if (n == 0)
            break;
        memset(dfn, 0, sizeof(dfn));
        G.clear();
        G.resize(n + 1);
        timeCnt = colorCnt = 0;
        memset(is_bottom_color, 1, sizeof(is_bottom_color));
        scanf("%d", &m);
        for (; m > 0; --m)
        {
            scanf("%d%d", &u, &v);
            if (u != v)
                G[u].push_back(v);
        }
        for (int i = 1; i <= n; ++i)
            if (!dfn[i])
                Tarjan(i);
        Query();
        bool flag = false;
        for (int u = 1; u <= n; ++u)
            if (is_bottom_color[color[u]])
            {
                if (flag)
                    printf(" ");
                flag = true;
                printf("%d", u);
            }
        printf("\n");
    }
    return 0;
}

#include <cstdio>
#include <vector>
#include <stack>
#include <algorithm>
#define MAXN 10010

using namespace std;
typedef vector<int> Edges;

int dfn[MAXN] = {0}, low[MAXN];
vector<int> G[MAXN];
stack<int> stk;
bool instack[MAXN] = {0};
int timeCnt = 0, ans = 0;
int n;

void Tarjan(int u)
{
    dfn[u] = low[u] = ++timeCnt;
    stk.push(u);
    instack[u] = true;
    int sz = G[u].size();
    for (int idx = 0; idx < sz; ++idx)
    {
        int v = G[u][idx];
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
        int t, memberCnt = 0;
        while (true)
        {
            ++memberCnt;
            t = stk.top();
            stk.pop();
            instack[t] = false;
            if (t == u)
                break;
        }
        if (memberCnt > 1)
            ++ans;
    }
}

int main()
{
    int m, u, v;
    scanf("%d%d", &n, &m);
    for (; m > 0; --m)
    {
        scanf("%d%d", &u, &v);
        if (u != v)
            G[u].push_back(v);
    }
    for (int i = 1; i <= n; ++i)
        if (!dfn[i])
            Tarjan(i);
    printf("%d", ans);
    return 0;
}

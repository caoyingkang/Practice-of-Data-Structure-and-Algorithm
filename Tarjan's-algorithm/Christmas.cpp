#include <cstdio>
#include <cstring>
#include <vector>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
#define MAXN 30010

using namespace std;
typedef set<int> EdgeSet;
typedef EdgeSet::iterator EdgeIter;

int dfn[MAXN], low[MAXN], color[MAXN], cmft[MAXN];
stack<int> stk;
bool instack[MAXN] = {false};
int scc_cmft[MAXN], dp[MAXN], inCnt[MAXN];
int timeCnt, colorCnt;
int n;
vector<EdgeSet> G;
vector<EdgeSet> DAG;
int ans;

int init()
{
    memset(dfn, 0, sizeof(dfn));
    memset(dp, 0, sizeof(dp));
    memset(inCnt, 0, sizeof(inCnt));
    memset(scc_cmft, 0, sizeof(scc_cmft));
    timeCnt = colorCnt = 0;
    G.clear();
    G.resize(n + 1);
    ans = 0;
}

void Tarjan(int u)
{
    dfn[u] = low[u] = ++timeCnt;
    stk.push(u);
    instack[u] = true;
    for (EdgeIter it = G[u].begin(); it != G[u].end(); ++it)
    {
        if (!dfn[*it])
        {
            Tarjan(*it);
            low[u] = min(low[u], low[*it]);
        }
        else if (instack[*it])
            low[u] = min(low[u], dfn[*it]);
    }
    if (low[u] == dfn[u])
    {
        ++colorCnt;
        int v;
        while (true)
        {
            v = stk.top();
            stk.pop();
            instack[v] = false;
            color[v] = colorCnt;
            scc_cmft[colorCnt] += cmft[v];
            if (v == u)
                break;
        }
    }
}

void BuildDAG()
{
    DAG.clear();
    DAG.resize(colorCnt + 1);
    for (int u = 1; u <= n; ++u)
        for (EdgeIter it = G[u].begin(); it != G[u].end(); ++it)
            if (color[u] != color[*it] &&
                !DAG[color[u]].count(color[*it]))
            {
                DAG[color[u]].insert(color[*it]);
                inCnt[color[*it]]++;
            }
}

void Query()
{
    int curr_scc, chld_scc;
    queue<int> qu;
    for (int i = 1; i <= colorCnt; ++i)
        if (inCnt[i] == 0)
            qu.push(i);
    while (!qu.empty())
    {
        curr_scc = qu.front();
        qu.pop();
        dp[curr_scc] += scc_cmft[curr_scc];
        if (DAG[curr_scc].empty())
        {
            ans = max(ans, dp[curr_scc]);
            continue;
        }
        for (EdgeIter it = DAG[curr_scc].begin(); it != DAG[curr_scc].end(); ++it)
        {
            chld_scc = *it;
            dp[chld_scc] = max(dp[chld_scc], dp[curr_scc]);
            if (--inCnt[chld_scc] == 0)
                qu.push(chld_scc);
        }
    }
}

int main(int argc, char const *argv[])
{
    int m, k, u, v;
    while (scanf("%d%d", &n, &m) == 2)
    {
        init();
        for (int i = 1; i <= n; ++i)
        {
            scanf("%d", &k);
            cmft[i] = k >= 0 ? k : 0;
        }
        for (int i = 1; i <= m; ++i)
        {
            scanf("%d%d", &u, &v);
            ++u;
            ++v;
            if (u != v)
                G[u].insert(v);
        }
        for (int i = 1; i <= n; ++i)
            if (!dfn[i])
                Tarjan(i);
        BuildDAG();
        Query();
        printf("%d\n", ans);
    }
    return 0;
}

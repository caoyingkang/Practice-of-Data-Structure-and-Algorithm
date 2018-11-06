#include <iostream>
#include <cstring>
#include <vector>
#include <set>
#include <stack>
#include <algorithm>
#define MAXN 1010

using namespace std;
typedef set<int> EdgeSet;
typedef EdgeSet::iterator EdgeIter;

int dfn[MAXN], low[MAXN], color[MAXN];
stack<int> stk;
bool instack[MAXN];
vector<EdgeSet> G;
int timeCnt, colorCnt;
vector<EdgeSet> DAG;
int n;

void Tarjan(int u)
{
    dfn[u] = low[u] = ++timeCnt;
    stk.push(u);
    instack[u] = true;
    for (EdgeIter it = G[u].begin(); it != G[u].end(); ++it)
    {
        int v = *it;
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
        int v;
        while ((v = stk.top()) != u)
        {
            stk.pop();
            instack[v] = false;
            color[v] = colorCnt;
        }
        stk.pop();
        instack[v] = false;
        color[v] = colorCnt;
    }
}

void BuildDAG()
{
    DAG.clear();
    DAG.resize(colorCnt + 1);
    for (int u = 1; u <= n; ++u)
    {
        int cu = color[u];
        for (EdgeIter it = G[u].begin(); it != G[u].end(); ++it)
        {
            int cv = color[*it];
            if (cu != cv)
                DAG[cu].insert(cv);
        }
    }
}

bool Query()
{
    bool inflag = false, outflag = false;
    for (int c = 1; c <= colorCnt; ++c)
    {
        if (DAG[c].empty())
        {
            if (outflag)
                return false;
            outflag = true;
        }
        for (EdgeIter it = DAG[c].begin(); it != DAG[c].end(); ++it)
            if (!instack[*it])
                instack[*it] = true;
    }
    for (int c = 1; c <= colorCnt; ++c)
    {
        if (!instack[c])
        {
            if (inflag)
                return false;
            inflag = true;
        }
    }
    return true;
}

int main(int argc, char const *argv[])
{
    int T, m, u, v;
    cin >> T;
    for (; T > 0; --T)
    {
        cin >> n >> m;
        memset(dfn, 0, sizeof(dfn));
        memset(instack, 0, sizeof(instack));
        G.clear();
        G.resize(n + 1);
        timeCnt = colorCnt = 0;
        for (; m > 0; --m)
        {
            cin >> u >> v;
            if (u != v)
                G[u].insert(v);
        }
        for (int i = 1; i <= n; ++i)
            if (!dfn[i])
                Tarjan(i);
        BuildDAG();
        cout << (Query() ? "Yes" : "No") << endl;
    }
    return 0;
}

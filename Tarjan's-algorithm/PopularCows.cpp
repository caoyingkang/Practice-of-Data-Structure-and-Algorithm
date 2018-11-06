#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <algorithm>
#define MAXN 10010

using namespace std;
typedef set<int> EdgeSet;
typedef EdgeSet::iterator EdgeIter;

int dfn[MAXN] = {0};
int low[MAXN] = {0};
int vCnt = 0;
vector<EdgeSet> G(MAXN);
stack<int> stk;
bool instack[MAXN] = {false};
int color[MAXN];
vector<int> colorSize(1);
int colorCnt = 0;
vector<EdgeSet> DAG;

void Tarjan(int u)
{
    dfn[u] = low[u] = ++vCnt;
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
        int sz = 0;
        int t;
        while ((t = stk.top()) != u)
        {
            color[t] = colorCnt;
            ++sz;
            stk.pop();
            instack[t] = false;
        }
        color[t] = colorCnt;
        ++sz;
        stk.pop();
        instack[t] = false;
        colorSize.push_back(sz);
    }
}

void BuildDAG()
{
    DAG.resize(colorCnt + 1);
    for (int u = 1; u <= vCnt; ++u)
        for (EdgeIter it = G[u].begin(); it != G[u].end(); ++it)
            if (color[u] != color[*it])
                DAG[color[u]].insert(color[*it]);
}

int Query()
{
    int ret = 0;
    for (int c = 1; c <= colorCnt; ++c)
        if (DAG[c].empty())
        {
            if (ret > 0)
                return 0;
            ret = colorSize[c];
        }
    return ret;
}

int main(int argc, char const *argv[])
{
    int n, m, u, v;
    cin >> n >> m;
    for (; m > 0; --m)
    {
        cin >> u >> v;
        if (u != v)
            G[u].insert(v);
    }
    int *p;
    while ((p = find(dfn + 1, dfn + n + 1, 0)) != dfn + n + 1)
        Tarjan(p - dfn);
    BuildDAG();
    cout << Query() << endl;
    return 0;
}

#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <algorithm>
#define MAXN 110

using namespace std;
typedef set<int> EdgeSet;
typedef EdgeSet::iterator EdgeIter;

int dfn[MAXN] = {0}, low[MAXN], color[MAXN];
vector<EdgeSet> G;
stack<int> stk;
bool instack[MAXN] = {false};
int timeCnt = 0, colorCnt = 0;
vector<EdgeSet> DAG;
int n;

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
        int t;
        while ((t = stk.top()) != u)
        {
            color[t] = colorCnt;
            stk.pop();
            instack[t] = false;
        }
        color[t] = colorCnt;
        stk.pop();
        instack[t] = false;
    }
}

void BuildDAG()
{
    DAG.resize(colorCnt + 1);
    for (int u = 1; u <= n; ++u)
        for (EdgeIter it = G[u].begin(); it != G[u].end(); ++it)
            if (color[u] != color[*it])
                DAG[color[u]].insert(color[*it]);
}

void Query(int &zeroIn, int &zeroOut)
{
    zeroIn = colorCnt;
    zeroOut = 0;
    for (int c = 1; c <= colorCnt; ++c)
    {
        if (DAG[c].empty())
            ++zeroOut;
        else
            for (EdgeIter it = DAG[c].begin(); it != DAG[c].end(); ++it)
                if (!instack[*it])
                {
                    instack[*it] = true;
                    --zeroIn;
                }
    }
}

int main(int argc, char const *argv[])
{
    int u, v;
    cin >> n;
    G.resize(n + 1);
    for (u = 1; u <= n; ++u)
        while (cin >> v)
        {
            if (v == 0)
                break;
            if (u != v)
                G[u].insert(v);
        }
    for (int i = 1; i <= n; ++i)
        if (!dfn[i])
            Tarjan(i);
    if (colorCnt == 1)
    {
        cout << 1 << endl;
        cout << 0 << endl;
    }
    else
    {
        BuildDAG();
        int zeroIn, zeroOut;
        Query(zeroIn, zeroOut);
        cout << zeroIn << endl;
        cout << max(zeroIn, zeroOut) << endl;
    }
    return 0;
}

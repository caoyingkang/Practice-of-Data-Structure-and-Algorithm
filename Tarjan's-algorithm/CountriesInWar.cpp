#include <cstdio>
#include <cstring>
#include <vector>
#include <set>
#include <stack>
#include <map>
#include <algorithm>
#define MAXN 510
#define INF 0x0fffffff

using namespace std;
typedef set<int> EdgeSet;
typedef EdgeSet::iterator EdgeIter;

int dfn[MAXN], low[MAXN], color[MAXN];
stack<int> stk;
bool instack[MAXN] = {false};
map<int, int> edge2h, DAGedge2h;
int len[MAXN];
bool visited[MAXN], pending[MAXN];
int timeCnt, colorCnt;
int n;
vector<EdgeSet> G;
vector<EdgeSet> DAG;

inline int myHash(int x, int y)
{
    return x * MAXN + y;
}

int init()
{
    memset(dfn, 0, sizeof(dfn));
    edge2h.clear();
    DAGedge2h.clear();
    timeCnt = colorCnt = 0;
    G.clear();
    G.resize(n + 1);
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
            if (color[u] != color[*it])
            {
                int hs = myHash(u, *it),
                    DAG_hs = myHash(color[u], color[*it]);
                if (DAG[color[u]].count(color[*it]))
                    DAGedge2h[DAG_hs] = min(DAGedge2h[DAG_hs], edge2h[hs]);
                else
                {
                    DAG[color[u]].insert(color[*it]);
                    DAGedge2h[DAG_hs] = edge2h[hs];
                }
            }
}

int dijkstra(int src, int dest)
{
    memset(visited, 0, sizeof(visited));
    memset(pending, 0, sizeof(visited));
    pending[src] = true;
    for (int i = 1; i <= colorCnt; ++i)
        len[i] = INF;
    len[src] = 0;
    int u, ulen, v;
    while (true)
    {
        ulen = INF;
        for (int i = 1; i <= colorCnt; ++i)
            if (pending[i] && len[i] < ulen)
            {
                u = i;
                ulen = len[i];
            }
        if (ulen == INF) // pending set is empty
            return -1;
        pending[u] = false;
        visited[u] = true;
        if (u == dest) // find answer
            return len[u];
        for (EdgeIter it = DAG[u].begin(); it != DAG[u].end(); ++it)
        {
            v = *it;
            if (visited[v])
                continue;
            pending[v] = true;
            len[v] = min(len[v], ulen + DAGedge2h[myHash(u, v)]);
        }
    }
}

int main(int argc, char const *argv[])
{
    // freopen("data.in", "r", stdin);
    int m, u, v, h, k, cu, cv, ans;
    while (scanf("%d%d", &n, &m))
    {
        if (n == 0)
            break;
        init();
        for (; m > 0; --m)
        {
            scanf("%d%d%d", &u, &v, &h);
            G[u].insert(v);
            edge2h[myHash(u, v)] = h;
        }
        for (int i = 1; i <= n; ++i)
            if (!dfn[i])
                Tarjan(i);
        BuildDAG();
        scanf("%d", &k);
        for (; k > 0; --k)
        {
            scanf("%d%d", &u, &v);
            cu = color[u];
            cv = color[v];
            if (cu == cv)
                printf("0\n");
            else
            {
                ans = dijkstra(cu, cv);
                if (ans == -1)
                    printf("Nao e possivel entregar a carta\n");
                else
                    printf("%d\n", ans);
            }
        }
        printf("\n");
    }
    return 0;
}

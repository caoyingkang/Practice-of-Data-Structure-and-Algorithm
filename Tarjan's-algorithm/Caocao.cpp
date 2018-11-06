#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
#define MAXN 1010
#define INF 0x0ffffff

using namespace std;

int dfn[MAXN], low[MAXN];
int n, m;
int vCnt;
vector<vector<int>> G;
map<int, int> edge2w;
int min_cut_w;

inline int myHash(int v1, int v2)
{
    return max(v1, v2) * n + min(v1, v2);
}

void Tarjan(int u, int f)
{
    dfn[u] = low[u] = ++vCnt;
    for (int idx = 0; idx < G[u].size(); ++idx)
    {
        int v = G[u][idx];
        if (v == f)
            continue;
        if (!dfn[v])
        {
            Tarjan(v, u);
            if (dfn[u] < low[v])
                min_cut_w = min(min_cut_w, edge2w[myHash(u, v)]);
            low[u] = min(low[u], low[v]);
        }
        else
            low[u] = min(low[u], dfn[v]);
    }
}

int main(int argc, char const *argv[])
{
    int u, v, w;
    while (cin >> n >> m)
    {
        if (n == 0 && m == 0)
            break;
        memset(dfn, 0, sizeof(dfn));
        memset(low, 0, sizeof(low));
        vCnt = 0;
        G.clear();
        G.resize(n + 1);
        edge2w.clear();
        min_cut_w = INF;
        for (; m > 0; --m)
        {
            cin >> u >> v >> w;
            int currhash = myHash(u, v);
            if (edge2w.count(currhash))
                edge2w[currhash] = INF;
            else
            {
                edge2w[currhash] = w;
                G[u].push_back(v);
                G[v].push_back(u);
            }
        }
        Tarjan(1, 0);
        if (find(dfn + 1, dfn + n + 1, 0) != dfn + n + 1)
            cout << 0 << endl;
        else if (min_cut_w == INF)
            cout << -1 << endl;
        else if (min_cut_w == 0)
            cout << 1 << endl;
        else
            cout << min_cut_w << endl;
    }
    return 0;
}

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#define MAXN 1010

using namespace std;

int dfn[MAXN], low[MAXN];
int n;
int networkCnt = 0;
int timeCnt;
vector<vector<int>> G;
typedef vector<int>::iterator EdgeIter;
bool noCut;

struct AnsNode
{
    int u;
    int subs;
    AnsNode(int u, int subs) : u(u), subs(subs) {}
};
vector<AnsNode> ans;

bool operator<(const AnsNode &n1, const AnsNode &n2)
{
    return n1.u < n2.u;
}

void Tarjan(int u, int father)
{
    dfn[u] = low[u] = ++timeCnt;
    int v;
    int subs = 0;
    for (EdgeIter it = G[u].begin(); it != G[u].end(); ++it)
    {
        v = *it;
        if (v == father)
            continue;
        if (!dfn[v])
        {
            Tarjan(v, u);
            low[u] = min(low[u], low[v]);
            if (dfn[u] <= low[v])
                ++subs;
        }
        else if (dfn[u] > dfn[v])
            low[u] = min(low[u], dfn[v]);
    }
    if (father != 0 && subs > 0)
    {
        ans.emplace_back(u, subs + 1);
        noCut = false;
    }
    if (father == 0 && subs > 1)
    {
        ans.emplace_back(u, subs);
        noCut = false;
    }
}

int main(int argc, char const *argv[])
{
    int u, v;
    while (true) // for each network
    {
        n = 0;
        G.clear();
        while (true) // for each input line
        {
            scanf("%d", &u);
            if (u == 0)
                break;
            scanf("%d", &v);
            if (max(u, v) > n)
                G.resize((n = max(u, v)) + 1);
            G[u].push_back(v);
            G[v].push_back(u);
        }
        if (n == 0)
            break;
        printf("Network #%d\n", ++networkCnt);
        memset(dfn, 0, sizeof(dfn));
        timeCnt = 0;
        noCut = true;
        ans.clear();
        Tarjan(1, 0);
        if (noCut)
            printf("  No SPF nodes\n");
        else
        {
            sort(ans.begin(), ans.end());
            for (int i = 0; i < ans.size(); ++i)
                printf("  SPF node %d leaves %d subnets\n", ans[i].u, ans[i].subs);
        }
        printf("\n");
    }
    return 0;
}

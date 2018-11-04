#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
#include <map>
#define MAX_NODES 1010
#define MAX_LEN 1010
#define LETTERS 4
#define INF 0xffffff0

using namespace std;

map<char, int> char2int;
int nodeCount;
int dp[2 * MAX_NODES];

inline int my_dp_hash(int i, int j)
{
    return i % 2 ? j + MAX_NODES : j;
}

struct Node
{
    Node *chld[LETTERS];
    Node *pre;
    bool isbad;
} tree[MAX_NODES];

void initTree()
{
    nodeCount = 2;
    for (int i = 0; i < MAX_NODES; ++i)
    {
        memset(tree[i].chld, 0, sizeof(tree[i].chld));
        tree[i].pre = nullptr;
        tree[i].isbad = false;
    }
    for (int i = 0; i < LETTERS; ++i)
        tree[0].chld[i] = tree + 1;
}

void BuildStr(char *str)
{
    Node *p = tree + 1;
    for (int i = 0; str[i]; ++i)
    {
        if (!p->chld[char2int[str[i]]])
            p->chld[char2int[str[i]]] = tree + (nodeCount++);
        p = p->chld[char2int[str[i]]];
    }
    p->isbad = true;
}

void BuildPre()
{
    tree[1].pre = tree;
    Node *p, *q, *pre;
    queue<Node *> qu;
    qu.push(tree + 1);
    while (!qu.empty())
    {
        p = qu.front();
        qu.pop();
        for (int i = 0; i < LETTERS; ++i)
        {
            q = p->chld[i];
            if (q)
            {
                qu.push(q);
                pre = p->pre;
                while (pre->chld[i] == nullptr)
                    pre = pre->pre;
                q->pre = pre->chld[i];
                if (q->pre->isbad)
                    q->isbad = true;
            }
        }
    }
}

int Query(char *str)
{
    Node *q;
    Node *pj, *pk;
    dp[my_dp_hash(0, 1)] = 0;
    for (int k = 2; k < nodeCount; ++k)
        dp[my_dp_hash(0, k)] = INF;
    int i;
    for (i = 0; str[i]; ++i)
    {
        for (int k = 1; k < nodeCount; ++k)
            dp[my_dp_hash(i + 1, k)] = INF;
        for (int j = 1; j < nodeCount; ++j)
        {
            if ((tree + j)->isbad || dp[my_dp_hash(i, j)] == INF)
                continue;
            pj = tree + j;
            for (int chld_idx = 0; chld_idx < LETTERS; ++chld_idx)
            {
                q = pj;
                while (q->chld[chld_idx] == nullptr)
                    q = q->pre;
                pk = q->chld[chld_idx];
                if (pk->isbad)
                    continue;
                int k = pk - tree;
                dp[my_dp_hash(i + 1, k)] =
                    min(dp[my_dp_hash(i + 1, k)],
                        dp[my_dp_hash(i, j)] + (char2int[str[i]] != chld_idx));
            }
        }
    }
    int ret = INF;
    for (int j = 1; j < nodeCount; ++j)
    {
        if ((tree + j)->isbad || dp[my_dp_hash(i, j)] == INF)
            continue;
        ret = min(ret, dp[my_dp_hash(i, j)]);
    }
    return ret == INF ? -1 : ret;
}

int main(int argc, char const *argv[])
{
    char2int['A'] = 0;
    char2int['G'] = 1;
    char2int['C'] = 2;
    char2int['T'] = 3;
    int t = 0, N;
    char str[MAX_LEN];
    while (true)
    {
        scanf("%d", &N);
        if (N == 0)
            break;
        ++t;
        initTree();
        for (; N > 0; --N)
        {
            scanf("%s", str);
            BuildStr(str);
        }
        BuildPre();
        scanf("%s", str);
        printf("Case %d: %d\n", t, Query(str));
    }
    return 0;
}

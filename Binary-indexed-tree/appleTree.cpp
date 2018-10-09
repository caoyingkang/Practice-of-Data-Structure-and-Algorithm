#include <iostream>
#include <vector>
#include <string>
#define MAX 100010

using namespace std;

vector<int> G[MAX]; //adjacent list
bool hasApple[MAX];
int lowbit[MAX];
int start[MAX];
int off[MAX];
int C[MAX];
int N; // # of forks

int Sum(int k)
{
    return k == 0 ? 0 : C[k] + Sum(k - lowbit[k]);
}

void Update(int k, int upd)
{
    if (k > N)
        return;
    C[k] += upd;
    Update(k + lowbit[k], upd);
}

int DFS(int p, int r, int t)
{
    // r: root of the subtree
    // p: the parent node of r (0 if r==1)
    // t: start[r]
    // return: end[r]
    start[r] = t;
    for (int i = 0; i < G[r].size(); ++i)
    {
        if (G[r][i] == p)
            continue;
        t = DFS(r, G[r][i], t + 1);
    }
    return off[r] = t;
}

int main(int argc, char const *argv[])
{
    cin >> N;
    for (int i = 1; i <= N; ++i)
    {
        hasApple[i] = true;
        C[i] = lowbit[i] = i & (-i);
    }
    int u, v;
    for (int i = 1; i <= N - 1; ++i)
    {
        cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    DFS(0, 1, 1);
    int M; // # of ops
    cin >> M;
    string ch;
    int k;
    for (int i = 1; i <= M; ++i)
    {
        cin >> ch >> k;
        if (ch[0] == 'C')
        {
            Update(start[k], hasApple[k] ? -1 : 1);
            hasApple[k] = !hasApple[k];
        }
        else
        {
            cout << Sum(off[k]) - Sum(start[k] - 1) << endl;
        }
    }
    return 0;
}
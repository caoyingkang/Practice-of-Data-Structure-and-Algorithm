#include <iostream>
#include <cstring>
#include <deque>
#include <set>
#define MAX 240
#define INF 0xffffff0

using namespace std;
int dist[MAX][MAX], G[MAX][MAX];
int K, C, M, n;
bool Visited[MAX];
int Layer[MAX];

void floyd()
{
    for (int k = 1; k <= n; ++k)
        for (int i = 1; i <= n; ++i)
            for (int j = i + 1; j <= n; ++j)
                if (dist[i][k] && dist[k][j])
                    if (dist[i][j] == 0 || dist[i][k] + dist[k][j] < dist[i][j])
                        dist[j][i] = dist[i][j] = dist[i][k] + dist[k][j];
}

int initG(int up)
{
    memset(G, 0, sizeof(G));
    for (int i = K + 1; i <= K + C; ++i)
        G[0][i] = 1;
    for (int i = 1; i <= K; ++i)
        G[i][n + 1] = M;
    for (int i = K + 1; i <= K + C; ++i)
        for (int j = 1; j <= K; ++j)
            if (dist[i][j] && dist[i][j] <= up)
                G[i][j] = 1;
}

bool CountLayer()
{
    deque<int> q;
    memset(Layer, 0xff, sizeof(Layer));
    Layer[0] = 0;
    q.push_back(0);
    while (!q.empty())
    {
        int v = q.front();
        q.pop_front();
        for (int j = 0; j <= n + 1; j++)
            if (G[v][j] > 0 && Layer[j] == -1)
            {
                Layer[j] = Layer[v] + 1;
                if (j == n + 1)
                    return true;
                else
                    q.push_back(j);
            }
    }
    return false;
}

int Dinic()
{
    int i;
    int s;
    int nMaxFlow = 0;
    deque<int> q;
    while (CountLayer())
    {
        q.push_back(0);
        memset(Visited, 0, sizeof(Visited));
        Visited[0] = true;
        while (!q.empty())
        {
            int nd = q.back();
            if (nd == n + 1)
            {
                int nMinC = INF;
                int nMinC_vs;
                for (i = 1; i < q.size(); i++)
                {
                    int vs = q[i - 1];
                    int ve = q[i];
                    if (G[vs][ve] > 0 && nMinC > G[vs][ve])
                    {
                        nMinC = G[vs][ve];
                        nMinC_vs = vs;
                    }
                }
                nMaxFlow += nMinC;
                for (i = 1; i < q.size(); i++)
                {
                    int vs = q[i - 1];
                    int ve = q[i];
                    G[vs][ve] -= nMinC;
                    G[ve][vs] += nMinC;
                }
                while (!q.empty() && q.back() != nMinC_vs)
                {
                    Visited[q.back()] = 0;
                    q.pop_back();
                }
            }
            else
            {
                for (i = 0; i <= n + 1; i++)
                {
                    if (G[nd][i] > 0 && Layer[i] == Layer[nd] + 1 &&
                        !Visited[i])
                    {
                        Visited[i] = true;
                        q.push_back(i);
                        break;
                    }
                }
                if (i > n + 1)
                    q.pop_back();
            }
        }
    }
    return nMaxFlow;
}

int main(int argc, char const *argv[])
{
    cin >> K >> C >> M;
    n = K + C;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            cin >> dist[i][j];
    floyd();
    set<int> answers;
    for (int i = K + 1; i <= K + C; ++i)
        for (int j = 1; j <= K; ++j)
            if (dist[i][j])
                answers.insert(dist[i][j]);
    int low = 0, high = answers.size() - 1;
    int arr[high + 1];
    int idx = 0;
    for (set<int>::iterator it = answers.begin(); it != answers.end(); ++it)
        arr[idx++] = *it;
    while (low != high)
    {
        idx = (low + high) / 2;
        initG(arr[idx]);
        if (Dinic() == C)
            high = idx;
        else
            low = idx + 1;
    }
    cout << arr[low] << endl;
    return 0;
}

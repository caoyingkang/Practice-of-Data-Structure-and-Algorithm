#include <iostream>
#include <cstring>
#include <deque>
#define MAX 410
#define INF 0xffffff0

using namespace std;

int N, M, maxnd;
int G[MAX][MAX];
bool Visited[MAX];
int Layer[MAX];

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
        for (int j = 0; j <= maxnd; j++)
            if (G[v][j] > 0 && Layer[j] == -1)
            {
                Layer[j] = Layer[v] + 1;
                if (j == maxnd)
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
            if (nd == maxnd)
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
                for (i = 0; i <= maxnd; i++)
                {
                    if (G[nd][i] > 0 && Layer[i] == Layer[nd] + 1 &&
                        !Visited[i])
                    {
                        Visited[i] = true;
                        q.push_back(i);
                        break;
                    }
                }
                if (i > maxnd)
                    q.pop_back();
            }
        }
    }
    return nMaxFlow;
}

int main(int argc, char const *argv[])
{
    while (cin >> N >> M)
    {
        maxnd = N + M + 1;
        memset(G, 0, sizeof(G));
        int Si, mi;
        for (int i = 1; i <= N; ++i)
        {
            cin >> Si;
            for (; Si > 0; --Si)
            {
                cin >> mi;
                G[i][N + mi] = 1;
            }
        }
        for (int i = 1; i <= N; ++i)
            G[0][i] = 1;
        for (int i = N + 1; i <= N + M; ++i)
            G[i][maxnd] = 1;
        cout << Dinic() << endl;
    }
    return 0;
}

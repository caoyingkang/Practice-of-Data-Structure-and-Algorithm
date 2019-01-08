#include <iostream>
#include <cstring>
#include <deque>
#include <algorithm>
#include <vector>
#define MAXN 25
#define MAXM 205
#define MAXDIM (MAXM + MAXN + 2)
#define INFINITE 0xffffff

using namespace std;

int M, N;

int C[MAXDIM][MAXDIM];
int B[MAXDIM][MAXDIM];
int G[MAXDIM][MAXDIM];
int copyG[MAXDIM][MAXDIM];
bool Visited[MAXDIM];
int Layer[MAXDIM];
int src, tar;
int dim; // number of vertices in G

bool CountLayer()
{
    int layer = 0;
    deque<int> q;
    memset(Layer, 0xff, sizeof(Layer));
    Layer[src] = 0;
    q.push_back(src);
    while (!q.empty())
    {
        int v = q.front();
        q.pop_front();
        for (int j = 0; j < dim; j++)
        {
            if (G[v][j] > 0 && Layer[j] == -1)
            {
                Layer[j] = Layer[v] + 1;
                if (j == tar)
                    return true;
                else
                    q.push_back(j);
            }
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
        q.push_back(src);
        memset(Visited, 0, sizeof(Visited));
        Visited[src] = 1;
        while (!q.empty())
        {
            int nd = q.back();
            if (nd == tar)
            {
                int nMinC = INFINITE;
                int nMinC_vs;
                for (i = 1; i < q.size(); i++)
                {
                    int vs = q[i - 1];
                    int ve = q[i];
                    if (G[vs][ve] > 0)
                    {
                        if (nMinC > G[vs][ve])
                        {
                            nMinC = G[vs][ve];
                            nMinC_vs = vs;
                        }
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
                for (i = 0; i < dim; i++)
                {
                    if (G[nd][i] > 0 && Layer[i] == Layer[nd] + 1 &&
                        !Visited[i])
                    {
                        Visited[i] = 1;
                        q.push_back(i);
                        break;
                    }
                }
                if (i >= dim)
                    q.pop_back();
            }
        }
    }
    return nMaxFlow;
}

bool SetV(int i, int j, int v, char ch)
{
    if (ch == '=')
    {
        C[i][M + j] = min(C[i][M + j], v);
        B[i][M + j] = max(B[i][M + j], v);
        if (C[i][M + j] != v || B[i][M + j] != v)
            return false;
    }
    else if (ch == '<')
    {
        C[i][M + j] = min(C[i][M + j], v - 1);
        if (C[i][M + j] < B[i][M + j])
            return false;
    }
    else
    {
        B[i][M + j] = max(B[i][M + j], v + 1);
        if (C[i][M + j] < B[i][M + j])
            return false;
    }
    return true;
}

int main(int argc, char const *argv[])
{
    int i, j, k;
    int test;
    int row, col;
    int rowsum, colsum;
    cin >> test;
    for (; test > 0; --test)
    {
        memset(G, 0, sizeof(G));
        memset(C, 0, sizeof(C));
        memset(B, 0, sizeof(B));
        cin >> M >> N;
        for (i = 1; i <= M; ++i)
        {
            cin >> rowsum;
            C[0][i] = B[0][i] = rowsum;
        }
        for (i = 1; i <= N; ++i)
        {
            cin >> colsum;
            C[M + i][N + M + 1] = B[M + i][N + M + 1] = colsum;
        }
        for (i = 1; i <= M; ++i)
            for (j = 1; j <= N; ++j)
            {
                C[i][M + j] = INFINITE;
            }
        int c;
        cin >> c;
        char ch;
        int v;
        bool failed = false;
        for (; c > 0; --c)
        {
            cin >> row >> col >> ch >> v;
            if (failed)
                continue;
            // special cases
            if ((ch == '<' && v <= 0) ||
                (ch == '=' && v < 0))
            {
                failed = true;
                continue;
            }
            if (ch == '>' && v < 0)
                continue;
            // normal cases
            if (row == 0 && col == 0)
                for (i = 1; i <= M; ++i)
                    for (j = 1; j <= N; ++j)
                        if (SetV(i, j, v, ch) == false)
                            failed = true;
            if (row == 0 && col > 0)
                for (i = 1; i <= M; ++i)
                    if (SetV(i, col, v, ch) == false)
                        failed = true;
            if (row > 0 && col == 0)
                for (j = 1; j <= N; ++j)
                    if (SetV(row, j, v, ch) == false)
                        failed = true;
            if (row > 0 && col > 0)
                if (SetV(row, col, v, ch) == false)
                    failed = true;
        }
        if (failed)
        {
            cout << "IMPOSSIBLE\n"
                 << endl;
            continue;
        }

        // build G
        int X = M + N + 2, Y = M + N + 3;
        dim = M + N + 4;
        for (i = 0; i <= M + N + 1; ++i)
            for (j = 0; j <= M + N + 1; ++j)
                if (i != j)
                {
                    G[i][j] = C[i][j] - B[i][j];
                    G[i][X] += B[i][j];
                    G[Y][j] += B[i][j];
                }
        G[M + N + 1][0] = INFINITE;

        memcpy(copyG, G, sizeof(G));

        int tmp = 0;
        for (i = 0; i < dim; ++i)
            if (i != Y)
                tmp += G[Y][i];

        src = Y;
        tar = X;
        int maxflow = Dinic();

        if (tmp != maxflow)
        {
            cout << "IMPOSSIBLE\n"
                 << endl;
            continue;
        }

        G[0][M + N + 1] = G[M + N + 1][0] = 0;
        src = 0;
        tar = M + N + 1;
        Dinic();

        // get answer
        for (i = 1; i <= M; ++i)
            for (j = 1; j <= N; ++j)
                C[i][M + j] = copyG[i][M + j] - G[i][M + j] + B[i][M + j];
        for (i = 1; i <= M; ++i)
        {
            for (j = 1; j <= N; ++j)
                cout << C[i][M + j] << " ";
            cout << endl;
        }
        cout << endl;
    }
    return 0;
}

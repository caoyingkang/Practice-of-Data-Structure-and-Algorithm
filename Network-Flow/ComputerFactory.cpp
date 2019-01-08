#include <iostream>
#include <cstring>
#include <deque>
#include <vector>
#include <string>
#define MAXV 110
#define INF 0xffffff0

using namespace std;

int p, n;
int NodeCnt;
int G[MAXV][MAXV] = {0};
int pastG[MAXV][MAXV];
vector<string> nodestrs(MAXV);
bool Visited[MAXV];
int Layer[MAXV];

bool eq(const string &s1, const string &s2)
{
    for (int i = 0; i < p; ++i)
        if (s2[i] != '2' && s1[i] != s2[i])
            return false;
    return true;
}

bool CountLayer()
{
    deque<int> q;
    memset(Layer, 0xff, sizeof(Layer));
    Layer[1] = 0;
    q.push_back(1);
    while (!q.empty())
    {
        int v = q.front();
        q.pop_front();
        for (int j = 1; j <= NodeCnt; j++)
        {
            if (G[v][j] > 0 && Layer[j] == -1)
            {
                Layer[j] = Layer[v] + 1;
                if (j == NodeCnt)
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
        q.push_back(1);
        memset(Visited, 0, sizeof(Visited));
        Visited[1] = true;
        while (!q.empty())
        {
            int nd = q.back();
            if (nd == NodeCnt)
            {
                int nMinC = INF + 10;
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
                    Visited[q.back()] = false;
                    q.pop_back();
                }
            }
            else
            {
                for (i = 1; i <= NodeCnt; i++)
                    if (G[nd][i] > 0 && Layer[i] == Layer[nd] + 1 && !Visited[i])
                    {
                        Visited[i] = true;
                        q.push_back(i);
                        break;
                    }
                if (i > NodeCnt)
                    q.pop_back();
            }
        }
    }
    return nMaxFlow;
}

int main(int argc, char const *argv[])
{
    cin >> p >> n;
    NodeCnt = 2 * n + 2;
    string zerostr(p, '0');
    string onestr(p, '1');
    int q;
    char ch;
    string s1, s2;
    for (int i = 1; i <= n; ++i)
    {
        s1.clear();
        s2.clear();
        cin >> q;
        for (int j = 1; j <= p; ++j)
        {
            cin >> ch;
            s1.push_back(ch);
        }
        for (int j = 1; j <= p; ++j)
        {
            cin >> ch;
            s2.push_back(ch);
        }
        nodestrs[2 * i] = s1;
        nodestrs[2 * i + 1] = s2;
        G[2 * i][2 * i + 1] = q;
        if (eq(zerostr, s1))
            G[1][2 * i] = INF;
        if (eq(s2, onestr))
            G[2 * i + 1][NodeCnt] = INF;
    }
    for (int i = 3; i < NodeCnt; i += 2)
    {
        s1 = nodestrs[i];
        for (int j = 2; j < NodeCnt; j += 2)
        {
            s2 = nodestrs[j];
            if (eq(s1, s2))
                G[i][j] = INF;
        }
    }
    memcpy(pastG, G, sizeof(pastG));
    int maxflow = Dinic();
    if (maxflow == 0)
    {
        cout << "0 0" << endl;
        return 0;
    }
    vector<int> A, B, W;
    for (int i = 3; i < NodeCnt; i += 2)
        for (int j = 2; j < NodeCnt; j += 2)
            if (pastG[i][j] > 0 && pastG[i][j] > G[i][j])
            {
                A.push_back(i / 2);
                B.push_back(j / 2);
                W.push_back(pastG[i][j] - G[i][j]);
            }
    int sz = A.size();
    cout << maxflow << " " << sz << endl;
    for (int i = 0; i < sz; ++i)
        cout << A[i] << " " << B[i] << " " << W[i] << endl;
    return 0;
}

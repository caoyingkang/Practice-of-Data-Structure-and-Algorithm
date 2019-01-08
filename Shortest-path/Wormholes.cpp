#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#define MAXN 510
#define INF 0xffffff

using namespace std;

int n, m, W, F;
int dist[MAXN];

struct Edge
{
    int s, e, w;
    Edge(int s, int e, int w) : s(s), e(e), w(w) {}
};

vector<Edge> edges;

bool Bellman_Ford(int src)
{
    dist[src] = 0;
    int i, j, k;
    for (i = 1; i <= n; ++i)
    {
        if (i != src)
            dist[i] = INF;
    }
    int s, e, w;
    bool renewed;
    for (k = 1; k < n; ++k)
    {
        renewed = false;
        for (i = 0; i < edges.size(); ++i)
        {
            s = edges[i].s;
            e = edges[i].e;
            w = edges[i].w;
            if (dist[s] != INF && dist[s] + w < dist[e])
            {
                dist[e] = dist[s] + w;
                renewed = true;
            }
        }
        if (renewed == false)
            return false;
    }
    for (i = 0; i < edges.size(); ++i)
    {
        s = edges[i].s;
        e = edges[i].e;
        w = edges[i].w;
        if (dist[s] != INF && dist[s] + w < dist[e])
        {
            return true;
        }
    }
    return false;
}

int main()
{
    int i, j, k;
    int s, e, t;
    cin >> F;
    for (; F > 0; --F)
    {
        edges.clear();
        cin >> n >> m >> W;
        for (i = 1; i <= m; ++i)
        {
            cin >> s >> e >> t;
            edges.push_back(Edge(s, e, t));
            edges.push_back(Edge(e, s, t));
        }
        for (i = 1; i <= W; ++i)
        {
            cin >> s >> e >> t;
            edges.push_back(Edge(s, e, -t));
        }
        cout << (Bellman_Ford(1) ? "YES" : "NO") << endl;
    }
}

#include <iostream>
#include <cstring>
#include <algorithm>
#include <deque>
#include <vector>
#define MAXN 210
#define INFINITE 0xffffff

using namespace std;

int N, M;

int G[MAXN][MAXN];
bool Visited[MAXN];
int Layer[MAXN];
int m;

bool CountLayer()
{
	int layer = 0;
	deque<int> q;
	memset(Layer, 0xff, sizeof(Layer));
	Layer[1] = 0;
	q.push_back(1);
	while (!q.empty())
	{
		int v = q.front();
		q.pop_front();
		for (int j = 1; j <= m; j++)
		{
			if (G[v][j] > 0 && Layer[j] == -1)
			{
				Layer[j] = Layer[v] + 1;
				if (j == m)
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
		Visited[1] = 1;
		while (!q.empty())
		{
			int nd = q.back();
			if (nd == m)
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
				for (i = 1; i <= m; i++)
				{
					if (G[nd][i] > 0 && Layer[i] == Layer[nd] + 1 &&
						!Visited[i])
					{
						Visited[i] = 1;
						q.push_back(i);
						break;
					}
				}
				if (i > m)
					q.pop_back();
			}
		}
	}
	return nMaxFlow;
}

int main()
{
	while (cin >> N >> M)
	{
		int i, j, k;
		int s, e, c;
		memset(G, 0, sizeof(G));
		for (i = 1; i <= N; ++i)
		{
			cin >> s >> e >> c;
			G[s][e] += c;
		}
		m = M;
		cout << Dinic() << endl;
	}
}

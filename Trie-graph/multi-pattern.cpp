#include <cstdio>
#include <queue>
#define MAX 1010
#define LETTERS 26

using namespace std;

struct Node
{
    Node *chld[LETTERS] = {nullptr};
    Node *pre = nullptr;
    bool isbad = false;
} tree[20 * MAX];

int nodeCount = 2;

void BuildStr(char *str)
{
    Node *p = tree + 1;
    for (int i = 0; str[i]; ++i)
    {
        if (!p->chld[str[i] - 'a'])
            p->chld[str[i] - 'a'] = tree + (nodeCount++);
        p = p->chld[str[i] - 'a'];
    }
    p->isbad = true;
}

void BuildPre()
{
    for (int i = 0; i < LETTERS; ++i)
        tree[0].chld[i] = tree + 1;
    tree[1].pre = tree;
    queue<Node *> qu;
    qu.push(tree + 1);
    while (!qu.empty())
    {
        Node *p = qu.front();
        qu.pop();
        for (int i = 0; i < LETTERS; ++i)
        {
            Node *q = p->chld[i];
            if (q)
            {
                qu.push(q);
                Node *pre = p->pre;
                while (pre->chld[i] == nullptr)
                    pre = pre->pre;
                q->pre = pre->chld[i];
                if (q->pre->isbad)
                    q->isbad = true;
            }
        }
    }
}

bool Search(char *str)
{
    Node *p = tree + 1;
    for (int i = 0; str[i]; ++i)
    {
        if (p->isbad)
            return true;
        while (p->chld[str[i] - 'a'] == nullptr)
            p = p->pre;
        p = p->chld[str[i] - 'a'];
    }
    return p->isbad;
}

int main(int argc, char const *argv[])
{
    int n, m;
    scanf("%d", &n);
    char str[MAX];
    for (; n > 0; --n)
    {
        scanf("%s", str);
        BuildStr(str);
    }
    BuildPre();
    scanf("%d", &m);
    for (; m > 0; --m)
    {
        scanf("%s", str);
        printf("%s\n", Search(str) ? "YES" : "NO");
    }
    return 0;
}

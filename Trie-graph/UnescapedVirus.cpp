#include <iostream>
#include <cstring>
#include <queue>
#define MAX 30010
#define LETTERS 2

using namespace std;

struct Node
{
    Node *chld[LETTERS] = {nullptr};
    Node *pre = nullptr;
    bool isbad = false;
    bool searched = false;
    bool instack = false;
} tree[MAX];

int nodeCount;

void initTree()
{
    nodeCount = 2;
    memset(tree, 0, sizeof(tree));
    for (int i = 0; i < LETTERS; ++i)
        tree[0].chld[i] = tree + 1;
}

void BuildStr(char *str)
{
    Node *p = tree + 1;
    for (int i = 0; str[i]; ++i)
    {
        if (!p->chld[str[i] - '0'])
            p->chld[str[i] - '0'] = tree + (nodeCount++);
        p = p->chld[str[i] - '0'];
    }
    p->isbad = true;
}

void BuildPre()
{
    tree[1].pre = tree;
    queue<Node *> qu;
    Node *p, *q, *pre;
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
                while (!pre->chld[i])
                    pre = pre->pre;
                q->pre = pre->chld[i];
                if (q->pre->isbad)
                    q->isbad = true;
            }
        }
    }
}

void modify()
{
    queue<Node *> qu;
    qu.push(tree + 1);
    Node *p;
    while (!qu.empty())
    {
        p = qu.front();
        qu.pop();
        for (int i = 0; i < LETTERS; ++i)
        {
            if (p->chld[i])
                qu.push(p->chld[i]);
            else
                p->chld[i] = p->pre->chld[i];
        }
    }
}

bool SearchLoop(Node *p)
{
    Node *q;
    p->instack = true;
    for (int i = 0; i < LETTERS; ++i)
    {
        q = p->chld[i];
        if (q->isbad || q->searched)
            continue;
        if (q->instack)
            return true;
        if (SearchLoop(q))
            return true;
    }
    p->instack = false;
    p->searched = true;
    return false;
}

int main(int argc, char const *argv[])
{
    char str[MAX];
    int n;
    initTree();
    cin >> n;
    for (; n > 0; --n)
    {
        cin >> str;
        BuildStr(str);
    }
    BuildPre();
    modify();
    cout << (SearchLoop(tree + 1) ? "TAK" : "NIE") << endl;
    return 0;
}

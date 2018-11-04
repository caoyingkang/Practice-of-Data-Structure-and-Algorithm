#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <queue>
#define LEN_PATTERN 1010
#define NUM_PATTERN 250
#define LETTERS 26

using namespace std;

int nodeCount;
int virus_found;

struct Node
{
    Node *chld[LETTERS];
    Node *pre;
    int virus; // -1: safe; >=0: bad; >0: No. of virus
    bool visited;
    inline bool isbad() { return virus >= 0; }
} tree[LEN_PATTERN * NUM_PATTERN];

bool visitNode(Node *p)
{
    if (p == tree + 1)
        return false;
    bool ret = p->visited;
    if (!ret)
        p->visited = true;
    return ret;
}

void init_tree()
{
    nodeCount = 2;
    for (int i = 0; i < LEN_PATTERN * NUM_PATTERN; ++i)
    {
        memset(tree[i].chld, 0, sizeof(tree[i].chld));
        tree[i].pre = nullptr;
        tree[i].virus = -1;
        tree[i].visited = false;
    }
    tree[0].visited = true;
    for (int i = 0; i < LETTERS; ++i)
        tree[0].chld[i] = tree + 1;
}

void BuildStr(const string &str, int virus)
{
    Node *p = tree + 1;
    for (int i = 0; i < str.size(); ++i)
    {
        if (!p->chld[str[i] - 'A'])
            p->chld[str[i] - 'A'] = tree + (nodeCount++);
        p = p->chld[str[i] - 'A'];
        //cout << "str[i]=" << str[i] << " pos:" << p - tree << endl;
    }
    p->virus = virus;
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
                if (!q->isbad() && q->pre->isbad())
                    q->virus = 0;
            }
        }
    }
}

void Search(const string &str, bool forward)
{
    int sz = str.size();
    Node *p = tree + 1;
    for (int i = forward ? 0 : sz - 1;
         forward ? i < sz : i >= 0;
         forward ? ++i : --i)
    {
        while (!p->chld[str[i] - 'A'])
        {
            if (!visitNode(p))
                if (p->virus > 0)
                    virus_found++;
            p = p->pre;
        }
        p = p->chld[str[i] - 'A'];
        if (visitNode(p))
            continue;
        if (p->isbad())
        {
            if (p->virus > 0)
                virus_found++;
            Node *q = p->pre;
            while (q != tree)
            {
                if (visitNode(q))
                    break;
                if (q->virus > 0)
                    virus_found++;
                q = q->pre;
            }
        }
    }
}

void format(const string &cmp_str, string &org_str)
{
    org_str.clear();
    stringstream ss(cmp_str);
    char ch;
    int k;
    while (ss >> ch)
    {
        if (ch >= 'A' && ch <= 'Z')
            org_str.push_back(ch);
        else
        {
            ss >> k >> ch;
            org_str += string(k, ch);
            ss >> ch;
        }
    }
}

int main(int argc, char const *argv[])
{
    int T, n;
    string str, org_str;
    cin >> T;
    for (; T > 0; --T)
    {
        init_tree();
        cin >> n;
        for (int i = 1; i <= n; ++i)
        {
            cin >> str;
            BuildStr(str, i);
        }
        BuildPre();
        cin >> str;
        format(str, org_str);
        virus_found = 0;
        Search(org_str, true);
        Search(org_str, false);
        cout << virus_found << endl;
    }
    return 0;
}

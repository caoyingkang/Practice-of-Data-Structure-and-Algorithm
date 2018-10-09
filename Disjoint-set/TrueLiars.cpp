#include <iostream>
#include <string>
#include <cstring>
#include <set>
#include <map>

using namespace std;

const int MAX = 610;
int count_tree; //number of trees
int parent[MAX];
bool rel[MAX];   //true:same tribe as parent
int homo[MAX];   //only valid at root
int hetero[MAX]; //only valid at root
map<int, int> cache;

int GetRoot(int a)
{
    if (parent[a] == a)
        return a;
    int root = GetRoot(parent[a]);
    rel[a] = rel[a] == rel[parent[a]];
    return parent[a] = root;
}

void Merge(int a, int b, bool r)
{
    if (GetRoot(a) == GetRoot(b))
        return;
    int ra = GetRoot(a), rb = GetRoot(b);
    parent[rb] = ra;
    if (rel[rb] = (int(rel[a]) + int(rel[b]) + int(r)) % 2)
    {
        homo[ra] += homo[rb];
        hetero[ra] += hetero[rb];
    }
    else
    {
        homo[ra] += hetero[rb];
        hetero[ra] += homo[rb];
    }
    --count_tree;
}

inline int myHash(int len, int objsum)
{
    return count_tree * objsum + len;
}

int NumSolution(int *parr, bool *ppick, int len, int objsum)
//return:0(no solution),1(unique solution),2(more than 1 solution)
{
    if (objsum < 0)
        return 0;
    if (objsum == 0)
        return 1;
    //objsum>0, 1<=len<=count_tree
    int hash = myHash(len, objsum);
    if (cache.count(hash))
        return cache[hash];
    if (len == 1)
    {
        if (*parr == objsum)
        {
            *ppick = true;
            return cache[hash] = 1;
        }
        else
            return cache[hash] = 0;
    }
    //objsum>0, 1<len<=count_tree
    int num1 = NumSolution(parr + 1, ppick + 1, len - 1, objsum);         //not pick it
    int num2 = NumSolution(parr + 1, ppick + 1, len - 1, objsum - *parr); //pick it
    if (num1 + num2 >= 2)
        return cache[hash] = 2;
    if (num1 == 0 && num2 == 0)
        return cache[hash] = 0;
    //num1==1,num2==0 or num1==0,num2==1
    if (num2 == 1)
        *ppick = true;
    return cache[hash] = 1;
}

int main(int argc, char const *argv[])
{
    int n, p1, p2, p;
    while (1)
    {
        //for each dataset
        cin >> n >> p1 >> p2;
        if (n == 0 && p1 == 0 && p2 == 0)
            break;
        if (p1 == p2)
        {
            int x, y;
            string a;
            for (; n > 0; --n)
            {
                cin >> x >> y >> a;
            }
            cout << "no" << endl;
            continue;
        }
        //p1!=p2
        //initialization
        p = p1 + p2; //total number of individuals
        count_tree = p;
        for (int i = 1; i <= p; ++i)
        {
            parent[i] = i;
            homo[i] = 1;
            hetero[i] = 0;
        }
        memset(rel, 1, sizeof(rel));
        cache.clear();
        //inference: 1. read QA pairs and merge
        int x, y;
        string a;
        for (; n > 0; --n)
        {
            cin >> x >> y >> a;
            if (count_tree == 1)
                continue;
            Merge(x, y, a[0] == 'y');
        }
        //inference: 2. reduce to a knapsack problem:
        //pick some items from "outnum" to sum up to "objsum"
        bool flag = false; //aware of failure
        int outnum[count_tree + 1];
        int objsum = p1;
        for (int i = 1, idx = 1; i <= p && idx <= count_tree; ++i)
        {
            if (parent[i] != i)
                continue;
            //i is a root
            int temp1 = homo[i], temp2 = hetero[i];
            if (temp1 == temp2)
            {
                flag = true;
                break;
            }
            else if (temp1 < temp2)
            {
                int temp = temp1;
                temp1 = temp2;
                temp2 = temp;
            }
            //temp1>temp2
            outnum[idx++] = temp1 - temp2;
            objsum -= temp2;
        }
        if (flag)
        {
            cout << "no" << endl;
            continue;
        }
        //inference: 3. solve the knapsack problem
        bool pick[count_tree + 1];
        memset(pick, 0, sizeof(pick));
        int num_sol = NumSolution(&outnum[1], &pick[1], count_tree, objsum);
        if (num_sol == 2)
        {
            cout << "no" << endl;
            continue;
        }
        //answer is yes, now print divine tribe
        set<int> divine_roots;
        for (int i = 1, idx = 1; i <= p && idx <= count_tree; ++i)
        {
            if (parent[i] != i)
                continue;
            //i is a root
            if (homo[i] > hetero[i])
            {
                if (pick[idx++])
                    divine_roots.insert(i);
            }
            else
            {
                if (!pick[idx++])
                    divine_roots.insert(i);
            }
        }
        int divine[p1];
        for (int i = 1, idx = 0; i <= p && idx < p1; ++i)
        {
            if (divine_roots.count(GetRoot(i)))
            {
                if (rel[i])
                    divine[idx++] = i;
            }
            else
            {
                if (!rel[i])
                    divine[idx++] = i;
            }
        }
        for (int i = 0; i < p1; ++i)
            cout << divine[i] << endl;
        cout << "end" << endl;
    }
    return 0;
}

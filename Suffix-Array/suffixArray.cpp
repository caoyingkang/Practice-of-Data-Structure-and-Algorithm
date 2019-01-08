#include <iostream>

using namespace std;

const int MAXN = 100010;
int _wa[MAXN], _wb[MAXN], _wv[MAXN], _ws[MAXN]; //auxiliary array
int sa[MAXN];
void buildSA(const char *s, int *sa, int n, int m)
{
    // n: len of string s
    // m: No. of possible different characters in string s at the beginning,
    //    later becomes No. of different j-suffix
    // please append s with '\0' beforehand (set n=strlen(s)+1)
    // e.g. buildSA("banana", sa, 7, 127);
    int i, j, p, a, b, *rk = _wa, *k2sa = _wb, *t;
    for (i = 0; i < m; ++i)
        _ws[i] = 0;
    for (i = 0; i < n; ++i)
        _ws[rk[i] = s[i]]++;
    for (i = 1; i < m; ++i)
        _ws[i] += _ws[i - 1];
    for (i = n - 1; i >= 0; --i)
        sa[--_ws[rk[i]]] = i;
    for (j = p = 1; p < n; j <<= 1, m = p)
    {
        for (p = 0, i = n - j; i < n; ++i)
            k2sa[p++] = i;
        for (i = 0; i < n; ++i)
            if (sa[i] >= j)
                k2sa[p++] = sa[i] - j;
        for (i = 0; i < m; ++i)
            _ws[i] = 0;
        for (i = 0; i < n; ++i)
            _ws[_wv[i] = rk[k2sa[i]]]++;
        for (i = 1; i < m; ++i)
            _ws[i] += _ws[i - 1];
        for (i = n - 1; i >= 0; --i)
            sa[--_ws[_wv[i]]] = k2sa[i];
        t = rk;
        rk = k2sa;
        k2sa = t;
        for (rk[sa[0]] = 0, p = i = 1; i < n; ++i)
        {
            a = sa[i - 1], b = sa[i];
            if (k2sa[a] == k2sa[b] && k2sa[a + j] == k2sa[b + j])
                rk[b] = p - 1;
            else
                rk[b] = p++;
        }
    }
    return;
}

int *Rank = _wa;
void buildRank(int *sa, int *Rank, int n)
{
    for (int i = 0; i < n; ++i)
        Rank[sa[i]] = i;
}

int *height = _wb;
void buildHeight(const char *s, int *sa, int *Rank, int *height, int n)
{
    //make sure Rank[0] > 0: append s with '\0' beforehand
    int i, j, k;
    for (i = k = 0; i < n - 1; height[Rank[i++]] = k)
        for (k ? --k : 0, j = sa[Rank[i] - 1];
             s[i + k] == s[j + k]; ++k)
            ;
}

int main(int argc, char const *argv[])
{
    int sa[MAXN], Rank[MAXN], height[MAXN];
    buildSA("banana\0", sa, 7, 127);
    for (int i = 0; i < 7; i++)
    {
        cout << sa[i] << endl;
    }
    cout << endl;
    buildRank(sa, Rank, 7);
    for (int i = 0; i < 7; i++)
    {
        cout << Rank[i] << endl;
    }
    cout << endl;
    buildHeight("banana\0", sa, Rank, height, 7);
    for (int i = 0; i < 7; i++)
    {
        cout << height[i] << endl;
    }
    cout << endl;
    return 0;
}

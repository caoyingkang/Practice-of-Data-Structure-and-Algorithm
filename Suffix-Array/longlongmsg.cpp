#include <cstdio>
#include <cstring>

using namespace std;
const int MAXN = 100010;
int _wa[2 * MAXN], _wb[2 * MAXN], _wv[2 * MAXN], _ws[2 * MAXN];

void buildSA(const char *s, int *sa, int n, int m)
{
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

void buildRank(int *sa, int *Rank, int n)
{
    for (int i = 0; i < n; ++i)
        Rank[sa[i]] = i;
}

void BuildHeight(const char *s, int *sa, int *Rank, int *height, int n)
{
    int i, j, k;
    for (i = k = 0; i < n - 1; height[Rank[i++]] = k)
        for (k ? --k : 0, j = sa[Rank[i] - 1];
             s[i + k] == s[j + k]; ++k)
            ;
}

int main(int argc, char const *argv[])
{
    char str1[2 * MAXN], str2[MAXN];
    scanf("%s%s", str1, str2);
    int len1 = strlen(str1), len2 = strlen(str2);
    int len = len1 + len2 + 2; // including null zero
    strcat(str1, "-");
    strcat(str1, str2);
    int sa[2 * MAXN];
    buildSA(str1, sa, len, 127);
    int *Rank = _wa, *height = _wb;
    buildRank(sa, Rank, len);
    BuildHeight(str1, sa, Rank, height, len);
    int LCP = 0;
    for (int i = 1; i < len; ++i)
    {
        if ((sa[i - 1] < len1 && sa[i] > len1) ||
            (sa[i - 1] > len1 && sa[i] < len1))
            if (height[i] > LCP)
                LCP = height[i];
    }
    printf("%d", LCP);
    return 0;
}

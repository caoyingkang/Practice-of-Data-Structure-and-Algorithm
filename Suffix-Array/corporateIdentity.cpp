#include <cstdio>
#include <cstring>

using namespace std;
const int MAXN = 4010, MAXL = 210;
const int MAXALL = MAXN * MAXL;
int N;
int _wa[MAXALL], _wb[MAXALL], _wv[MAXALL], _ws[MAXALL];
int all_len, len[MAXN], min_len;
bool covered[MAXN];
int covered_n;

void buildSA(const int *s, int *sa, int n, int m)
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

void buildHeight(const int *s, int *sa, int *Rank, int *height, int n)
{
    int i, j, k;
    for (i = k = 0; i < n - 1; height[Rank[i++]] = k)
        for (k ? --k : 0, j = sa[Rank[i] - 1];
             s[i + k] == s[j + k]; ++k)
            ;
}

void buildLoc(int *Rank, int *len, int *Loc, int n)
{
    int i, j, k = len[0];
    for (j = i = 0; i < n; ++i)
    {
        if (i == k)
        {
            Loc[Rank[i]] = -1;
            k += len[++j] + 1;
        }
        else
            Loc[Rank[i]] = j;
    }
}

int checkAns(int ans, int *height, int *Loc, int n)
{
    memset(covered, 0, sizeof(covered));
    covered_n = 0;
    for (int i = 1; i < n; ++i)
    {
        if (height[i] >= ans)
        {
            if (covered_n == 0)
            {
                covered[Loc[i - 1]] = true;
                covered_n++;
            }
            if (!covered[Loc[i]])
            {
                covered[Loc[i]] = true;
                covered_n++;
            }
            if (covered_n == N)
                return i;
        }
        else
        {
            memset(covered, 0, sizeof(covered));
            covered_n = 0;
        }
    }
    return -1;
}

int main(int argc, char const *argv[])
{
    int l;
    char str[MAXL];
    int all[MAXALL], sa[MAXALL], *Rank = _wa, *height = _wb, *Loc = _wv;
    int i, j, k;
    int ansL, leftL, rightL;
    while (1)
    {
        scanf("%d", &N);
        if (N == 0)
            break;
        all_len = N;
        min_len = MAXL;
        for (j = 0, i = 1; i <= N; ++i)
        {
            scanf("%s", str);
            all_len += (len[i - 1] = l = strlen(str));
            if (l < min_len)
                min_len = l;
            for (k = 0; k < l; ++k)
                all[j++] = int(str[k]);
            all[j++] = i == N ? 0 : 127 + i;
        }
        buildSA(all, sa, all_len, 128 + N);
        buildRank(sa, Rank, all_len);
        buildHeight(all, sa, Rank, height, all_len);
        buildLoc(Rank, len, Loc, all_len);
        leftL = 0;
        rightL = min_len;
        while (leftL != rightL)
        {
            ansL = (leftL + rightL + 1) / 2;
            i = checkAns(ansL, height, Loc, all_len);
            if (i != -1)
                leftL = ansL;
            else
                rightL = ansL - 1;
        }
        ansL = leftL;
        if (ansL > 0)
        {
            i = checkAns(ansL, height, Loc, all_len);
            for (k = (j = sa[i]) + ansL; j < k; ++j)
                printf("%c", char(all[j]));
            printf("\n");
        }
        else
            printf("IDENTITY LOST\n");
    }
    return 0;
}

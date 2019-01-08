#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

const double EPS = 1e-6;

struct CVector
{
    double x, y;
    CVector() : x(0), y(0) {}
    CVector(double x, double y) : x(x), y(y) {}
};

using CPoint = CVector;

struct CSeg
{
    CPoint a, b;
    CSeg() {}
    CSeg(const CPoint &a, const CPoint &b) : a(a), b(b) {}
};

bool isZero(double x)
{
    return -EPS < x && x < EPS;
}

bool FLarger(double a, double b)
{
    return a - b > EPS;
}

bool FLess(double a, double b)
{
    return b - a > EPS;
}

bool FLargerEq(double a, double b)
{
    return FLarger(a, b) || isZero(b - a);
}

bool FLessEq(double a, double b)
{
    return FLess(a, b) || isZero(b - a);
}

CVector operator+(const CVector &p, const CVector &q)
{
    return CVector(p.x + q.x, p.y + q.y);
}

CVector operator-(const CVector &p, const CVector &q)
{
    return CVector(p.x - q.x, p.y - q.y);
}

CVector operator*(double k, const CVector &p)
{
    return CVector(k * p.x, k * p.y);
}

double operator*(const CVector &p, const CVector &q)
{
    return p.x * q.x + p.y * q.y;
}

bool operator==(const CPoint &p, const CPoint &q)
{
    return isZero(p.x - q.x) && isZero(p.y - q.y);
}

int Sign(double a)
{
    return fabs(a) < EPS ? 0 : (a > 0 ? 1 : -1);
}

double operator^(const CVector &p, const CVector &q)
{
    return p.x * q.y - q.x * p.y;
}

bool cmp_xy(const CPoint &p1, const CPoint &p2)
{
    // assume p1 and p2 are not the same point.
    // return true if p1 is left to p2
    // or they have the same x-coordinate but p1 is lower than p2.
    int s = Sign(p1.x - p2.x);
    if (s == -1)
        return true;
    else if (s == 1)
        return false;
    else
        return p1.y < p2.y;
}

int Graham_xy(vector<CPoint> &points, vector<CPoint> &stack)
{
    stack.clear();
    if (points.size() < 3)
        return 0;
    sort(points.begin(), points.end(), cmp_xy);
    stack.push_back(points[0]);
    stack.push_back(points[1]);
    int n = points.size();
    for (int i = 2; i < n; ++i)
    {
        while (stack.size() >= 2)
        {
            CPoint &p2 = *(stack.end() - 1);
            CPoint &p1 = *(stack.end() - 2);
            if (Sign((p2 - p1) ^ (points[i] - p2)) == -1)
                stack.pop_back();
            else
                break;
        }
        stack.push_back(points[i]);
    }
    int sz = stack.size();
    // now the top of the stack must be points[n-1]
    stack.push_back(points[n - 2]);
    for (int i = n - 3; i >= 0; --i)
    {
        while (stack.size() > sz)
        {
            CPoint &p2 = *(stack.end() - 1);
            CPoint &p1 = *(stack.end() - 2);
            if (Sign((p2 - p1) ^ (points[i] - p2)) == -1)
                stack.pop_back();
            else
                break;
        }
        stack.push_back(points[i]);
    }
    stack.pop_back();
    return stack.size();
}

int n;
vector<CPoint> points;
vector<CPoint> stack;

bool check()
{
    int sz = stack.size();
    bool good = false;
    for (int i = 1; i <= sz; ++i)
    {
        if (stack[i - 1] == stack[(i + 1) % sz])
            return false;
        if (isZero((stack[i % sz] - stack[i - 1]) ^
                   (stack[(i + 1) % sz] - stack[i % sz])))
            good = true;
        else if (good)
            good = false;
        else
            return false;
    }
    return true;
}

int main(int argc, char const *argv[])
{
    int t;
    int sz;
    double xi, yi;
    scanf("%d", &t);
    for (; t > 0; --t)
    {
        points.clear();
        scanf("%d", &n);
        for (int i = 0; i < n; ++i)
        {
            scanf("%lf%lf", &xi, &yi);
            points.emplace_back(xi, yi);
        }
        sz = Graham_xy(points, stack);
        if (sz == 0)
            printf("NO\n");
        else if (check())
            printf("YES\n");
        else
            printf("NO\n");
    }
    return 0;
}

#include <cstdio>
#include <vector>
#include <cmath>
#include <utility>
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

int n;
double maxx;
vector<CPoint> uppers(22);

bool isZero(double x)
{
    return -EPS < x && x < EPS;
}

bool FLess(double a, double b)
{
    return b - a > EPS;
}

bool FLessEq(double a, double b)
{
    return FLess(a, b) || isZero(b - a);
}

bool operator==(const CPoint &p, const CPoint &q)
{
    return isZero(p.x - q.x) && isZero(p.y - q.y);
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

double operator^(const CVector &p, const CVector &q)
{
    return p.x * q.y - q.x * p.y;
}

double area(const CVector &p, const CVector &q)
{
    return (p ^ q) / 2; // with direction
}

bool PointInSeg(const CPoint &p, const CSeg &s)
{
    double cross = (s.a - p) ^ (s.a - s.b);
    if (!isZero(cross))
        return false;
    if (FLessEq(min(s.a.x, s.b.x), p.x) &&
        FLessEq(p.x, max(s.a.x, s.b.x)) &&
        FLessEq(min(s.a.y, s.b.y), p.y) &&
        FLessEq(p.y, max(s.a.y, s.b.y)))
        return true;
    else
        return false;
}

double length(const CVector &p)
{
    return sqrt(p * p);
}

double dist(const CPoint &p, const CSeg &s)
{
    return fabs((p - s.a) ^ (s.b - s.a)) / length(s.b - s.a);
}

pair<int, CPoint> CrossPoint(const CSeg &s1, const CSeg &s2)
{
    // get the crosspoint of two segments and
    // leave hint about locations of segments in the returned first entry
    // for instance: pair<int, CPoint> result = CrossPoint(s1, s2);
    // result.first:
    // 0 formal cross
    // 1 one pair of endpoints coincide,
    //   but two segs don't parallel or coincide
    // 2 one endpoint of s1 lies on the interior of s2,
    //   but two segs don't parallel or coincide
    // 3 one endpoint of s2 lies on the interior of s1,
    //   but two segs don't parallel or coincide
    // 4 two segs disjoint,
    //   but two corresponding lines have one cross point,
    //   and it is neither on s1 nor on s2
    // 5 parallel
    // 6 coincide and have common points
    // 7 coincide but don't have common points
    // 8 two segs disjoint,
    //   but two corresponding lines have one cross point lying on s1
    // 9 two segs disjoint,
    //   but two corresponding lines have one cross point lying on s2
    // except for case 5 and 7, result.second is the cross point
    CPoint p1 = s1.a;
    CPoint p2 = s1.b;
    CPoint p3 = s2.a;
    CPoint p4 = s2.b;
    double a1 = area(p3 - p1, p4 - p1);
    double a2 = area(p4 - p2, p3 - p2);
    if (((p2 - p1) ^ (p3 - p1)) * ((p2 - p1) ^ (p4 - p1)) < -EPS &&
        ((p4 - p3) ^ (p1 - p3)) * ((p4 - p3) ^ (p2 - p3)) < -EPS)
    { // formal cross
        return make_pair(0, p1 + a1 / (a1 + a2) * (p2 - p1));
    }
    if (!(isZero((p2 - p1) ^ (p4 - p3)))) // neither parallel nor coincide
    {
        if (p1 == p3 || p1 == p4)
            return make_pair(1, p1);
        if (p2 == p3 || p2 == p4)
            return make_pair(1, p2);
        if (PointInSeg(p1, s2))
            return make_pair(2, p1);
        if (PointInSeg(p2, s2))
            return make_pair(2, p2);
        if (PointInSeg(p3, s1))
            return make_pair(3, p3);
        if (PointInSeg(p4, s1))
            return make_pair(3, p4);
        CPoint crosspnt = p1 + a1 / (a1 + a2) * (p2 - p1);
        if (PointInSeg(crosspnt, s1))
            return make_pair(8, crosspnt);
        if (PointInSeg(crosspnt, s2))
            return make_pair(9, crosspnt);
        return make_pair(4, crosspnt);
    }
    if (!isZero(dist(p1, s2))) // parallel
        return make_pair(5, CPoint());
    // coincide
    if (PointInSeg(p1, s2))
        return make_pair(6, p1);
    if (PointInSeg(p2, s2))
        return make_pair(6, p2);
    if (PointInSeg(p3, s1))
        return make_pair(6, p3);
    if (PointInSeg(p4, s1))
        return make_pair(6, p4);
    return make_pair(7, CPoint());
}

bool check(int idx1, int idx2, const CPoint &p1, const CPoint &p2)
{
    int i;
    CPoint q1, q2;
    pair<int, CPoint> r;
    CSeg ps(p1, p2);
    for (i = 0; i < n; ++i)
    {
        if (i == idx1 || i == idx2)
            continue;
        q1 = q2 = uppers[i];
        q2.y -= 1;
        r = CrossPoint(ps, CSeg(q1, q2));
        if (r.first == 0 || r.first == 3 || r.first == 9)
            continue;
        break;
    }
    if (i == n)
        return true;
    if (i < idx1 || i < idx2)
        return false;
    CPoint t;
    t = uppers[i - 1];
    r = CrossPoint(ps, CSeg(t, q1));
    maxx = max(maxx, r.second.x);
    t.y -= 1;
    r = CrossPoint(ps, CSeg(t, q2));
    maxx = max(maxx, r.second.x);
    return false;
}

main(int argc, char const *argv[])
{
    double xi, yi;
    while (scanf("%d", &n), n)
    {
        for (int i = 0; i < n; ++i)
            scanf("%lf%lf", &uppers[i].x, &uppers[i].y);
        maxx = uppers[0].x;
        bool good = false;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (i == j)
                    continue;
                CPoint p1 = uppers[i], p2 = uppers[j];
                p2.y -= 1;
                if (check(i, j, p1, p2))
                {
                    good = true;
                    break;
                }
            }
            if (good)
                break;
        }
        if (good)
            printf("Through all the pipe.\n");
        else
            printf("%.2lf\n", maxx);
    }
    return 0;
}

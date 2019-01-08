#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

const double EPS = 1e-8;
const double delta = 0.9 * 1e-5;

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

int Sign(double a)
{
    return fabs(a) < EPS ? 0 : (a > 0 ? 1 : -1);
}

struct CVector
{
    double x, y;
    CVector() : x(0), y(0) {}
    CVector(double x, double y) : x(x), y(y) {}
};

using CPoint = CVector;

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

double operator^(const CVector &p, const CVector &q)
{
    return p.x * q.y - q.x * p.y;
}

struct CSeg
{
    CPoint a, b;
    CSeg() {}
    CSeg(const CPoint &a, const CPoint &b) : a(a), b(b) {}
};

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

using Polygon = vector<CPoint>;

int CutConvexPolygon(const Polygon &src, Polygon &result, CPoint a, CPoint b)
{
    // cut convex polygon with line a->b, store the result polygon on the left side of a->b
    // points in Polygon should be sorted clockwise or counterclockwise
    // return the number of vertices in result
    int n = src.size();
    result.clear();
    for (int i = 0; i < n; ++i)
    {
        const CPoint &c = src[i];
        const CPoint &d = src[(i + 1) % n];
        if (Sign((b - a) ^ (c - a)) >= 0)
            result.push_back(c);
        pair<int, CPoint> r = CrossPoint(CSeg(c, d), CSeg(a, b));
        if (r.first == 0 || r.first == 3 || r.first == 8)
            result.push_back(r.second);
    }
    return result.size();
}

int n;
vector<CPoint> points;

void move_line(CPoint &p1, CPoint &p2, double d)
{
    CVector v(p1.y - p2.y, p2.x - p1.x);
    v = d / length(v) * v;
    p1 = p1 + v;
    p2 = p2 + v;
}

bool check(double guess)
{
    Polygon poly1 = points, poly2;
    CPoint p1, p2;
    for (int i = 0; i < n; ++i)
    {
        p1 = points[i];
        p2 = points[(i + 1) % n];
        move_line(p1, p2, guess);
        if (CutConvexPolygon(poly1, poly2, p1, p2) == 0)
            return false;
        swap(poly1, poly2);
    }
    return true;
}

int main(int argc, char const *argv[])
{
    double xi, yi;
    double lower, upper, ans;
    while (scanf("%d", &n), n)
    {
        points.clear();
        for (int i = 0; i < n; ++i)
        {
            scanf("%lf%lf", &xi, &yi);
            points.emplace_back(xi, yi);
        }
        lower = 0;
        upper = 10000;
        while (fabs(lower - upper) >= delta)
        {
            ans = (lower + upper) / 2;
            if (check(ans))
                lower = ans;
            else
                upper = ans;
        }
        printf("%.6lf\n", (lower + upper) / 2);
    }
    return 0;
}

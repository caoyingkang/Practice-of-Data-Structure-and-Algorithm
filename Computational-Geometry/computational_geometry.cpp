#include <cmath>
#include <utility>
#include <algorithm>
#include <cassert>
#include <vector>
#include <iostream>
using namespace std;

const double PI = acos(-1);
const double dINF = 1e+20;
const double EPS = 1e-6;

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
    return fabs(a) < EPS ? 0 : a > 0 ? 1 : -1;
}

struct CVector
{
    double x, y;
    CVector() : x(0), y(0) {}
    CVector(double x, double y) : x(x), y(y) {}
};

using CPoint = CVector;

struct CLine
{
    CPoint a, b;
    CLine() {}
    CLine(const CPoint &a, const CPoint &b) : a(a), b(b) {}
    // two-point form of straight-line equation:
    // (y-y1)*(x2-x1) = (x-x1)*(y2-y1)
    bool sameX() const
    {
        return isZero(a.x - b.x);
    }
    bool sameY() const
    {
        return isZero(a.y - b.y);
    }
    double getX(double y)
    {
        assert(!sameY());
        return (y - a.y) / (b.y - a.y) * (b.x - a.x) + a.x;
    }
    double getY(double x)
    {
        assert(!sameX());
        return (x - a.x) / (b.x - a.x) * (b.y - a.y) + a.y;
    }
};

struct CSeg
{
    CPoint a, b;
    CSeg() {}
    CSeg(const CPoint &a, const CPoint &b) : a(a), b(b) {}
};

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

double length(const CVector &p)
{
    return sqrt(p * p);
}

double length2(const CVector &p)
{
    return p * p;
}

CVector unit(const CVector &p)
{
    return 1 / length(p) * p;
}

double project(const CVector &p, const CVector &q)
{
    return p * unit(q); // with direction
}

double project_unit(const CVector &p, const CVector &n)
{
    // n is normalized
    return p * n; // with direction
}

double operator^(const CVector &p, const CVector &q)
{
    return p.x * q.y - q.x * p.y;
}

double area(const CVector &p, const CVector &q)
{
    return (p ^ q) / 2; // with direction
}

double dist(const CPoint &p, const CPoint &q)
{
    return length(p - q);
}

double dist2(const CPoint &p, const CPoint &q)
{
    return length2(p - q);
}

double dist(const CPoint &p, const CLine &l)
{
    return fabs((p - l.a) ^ (l.b - l.a)) / length(l.b - l.a);
}

double dist2(const CPoint &p, const CLine &l)
{
    double cross = (p - l.a) ^ (l.b - l.a);
    return cross * cross / length2(l.b - l.a);
}

double dist(const CLine &l, const CLine &m)
{
    // assure that l and m are parallel
    return dist(l.a, m);
}

double dist2(const CLine &l, const CLine &m)
{
    // assure that l and m are parallel
    return dist2(l.a, m);
}

double dist(const CPoint &p, const CSeg &s)
{
    return fabs((p - s.a) ^ (s.b - s.a)) / length(s.b - s.a);
}

double dist2(const CPoint &p, const CSeg &s)
{
    double cross = (p - s.a) ^ (s.b - s.a);
    return cross * cross / length2(s.b - s.a);
}

CPoint rotate(const CPoint &b, const CPoint &a, double alpha)
{
    // rotate b counterclockwise of alpha radians about a
    // return the result point c
    CVector p = b - a;
    return CPoint(a.x + (p.x * cos(alpha) - p.y * sin(alpha)),
                  a.y + (p.x * sin(alpha) + p.y * cos(alpha)));
}

int sideOfLine(const CPoint &p, const CPoint &a, const CPoint &b)
{
    double result = (b - a) ^ (p - a);
    if (isZero(result))
        return 0; // p is on line a->b
    else if (result > 0)
        return 1; // p is on the left side of line a->b
    else
        return -1; // p is on the right side of line a->b
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

CLine Vertical(const CPoint &p, const CLine &l)
{
    return CLine(p, p + CVector(l.a.y - l.b.y,
                                l.b.x - l.a.x));
}

CPoint foot(const CPoint &p, const CLine &l)
{
    CVector n = unit(l.b - l.a);
    return l.a + project_unit(p - l.a, n) * n;
}

double angle(const CLine &l, const CLine &m)
{
    // the angle of two lines (between 0 and PI/2)
    return acos(fabs(
        project(l.b - l.a, m.b - m.a) / length(l.b - l.a)));
}

pair<int, CPoint> CrossPoint(const CLine &l, const CLine &m)
{
    // get the crosspoint of two lines and
    // leave hint about coincide or parallel lines in the returned first entry
    // for instance: pair<int, CPoint> result = CrossPoint(l1, l2);
    // result.first:
    // 0 intersect with unique point
    // 1 coincide
    // 2 parallel
    double a1 = area(m.a - l.a, l.b - l.a);
    double a2 = area(l.b - l.a, m.b - l.a);
    if (isZero(a1 + a2))
    {
        if (isZero(dist(l, m)))
            return make_pair(1, CPoint());
        else
            return make_pair(2, CPoint());
    }
    return make_pair(0, m.a + a1 / (a1 + a2) * (m.b - m.a));
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

bool cmp_yx(const CPoint &p1, const CPoint &p2)
{
    // assume p1 and p2 are not the same point.
    // return true if p1 is lower than p2
    // or they are at the same height but p1 is left to p2.
    int s = Sign(p1.y - p2.y);
    if (s == -1)
        return true;
    else if (s == 1)
        return false;
    else
        return p1.x < p2.x;
}

int icmp_yx(const CPoint &p1, const CPoint &p2)
{
    // return:
    // -1: if p1 is lower than p2
    //     or they are at the same height but p1 is left to p2.
    // 1: if p1 is upper than p2
    //    or they are at the same height but p1 is right to p2.
    // 0: if they are the same point
    int s = Sign(p1.y - p2.y);
    if (s == -1)
        return -1;
    else if (s == 1)
        return 1;
    else
        return Sign(p1.x - p2.x);
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

int icmp_xy(const CPoint &p1, const CPoint &p2)
{
    // return:
    // -1: if p1 is left to p2
    //     or they have the same x-coordinate but p1 is lower than p2.
    // 1: if p1 is right to p2
    //    or they have the same x-coordinate but p1 is above p2.
    // 0: if they are the same point
    int s = Sign(p1.x - p2.x);
    if (s == -1)
        return -1;
    else if (s == 1)
        return 1;
    else
        return Sign(p1.y - p2.y);
}

struct Cmp_polar
{
    CPoint p0; // reference point
    Cmp_polar(const CPoint &p) : p0(p) {}
    bool operator()(const CPoint &p1, const CPoint &p2) const
    {
        // assume p1 and p2 are not the same point.
        // return true if the polar angle of p1 with respect to p0 is smaller than p2
        // or they have the same polar angle but p1 is closer to p0 than to p2.
        int s = Sign((p1 - p0) ^ (p2 - p0));
        if (s == 1)
            return true;
        else if (s == -1)
            return false;
        else
            return dist2(p0, p1) < dist2(p0, p2) ? true : false;
    }
};

int Graham_polar(vector<CPoint> &points, vector<CPoint> &stack)
{
    // assume no two points are the same point.
    // the convex set of points are stored counterclockwise in stack.
    // those points that are not the vertices of the convex polygon but
    // lie on the edge of the convex polygon is discarded.
    // return the number of vertices of the convec polygon.
    // Note: after this function points may be changed
    stack.clear();
    if (points.size() < 3)
        return 0;
    // sort by Cartesian coordinates, the lowest-leftmost point will be points[0]
    sort(points.begin(), points.end(), cmp_xy);
    // sort by polar coordinates
    sort(points.begin() + 1, points.end(), Cmp_polar(points[0]));
    stack.push_back(points[0]);
    stack.push_back(points[1]);
    int n = points.size();
    for (int i = 2; i < n; ++i)
    {
        while (stack.size() >= 2)
        {
            CPoint &p2 = *(stack.end() - 1);
            CPoint &p1 = *(stack.end() - 2);
            if (Sign((p2 - p1) ^ (points[i] - p2)) <= 0)
                stack.pop_back();
            else
                break;
        }
        stack.push_back(points[i]);
    }
    return stack.size();
}

int Graham_xy(vector<CPoint> &points, vector<CPoint> &stack)
{
    // assume no two points are the same point.
    // the convex set of points are stored counterclockwise in stack.
    // those points that are not the vertices of the convex polygon but
    // lie on the edge of the convex polygon are included.
    // return the number of points in the convex set.
    // Note: after this function points may be changed
    stack.clear();
    if (points.size() < 3)
        return 0;
    // sort by Cartesian coordinates, the lowest-leftmost point will be points[0]
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

int main(int argc, char const *argv[])
{
    cout << "hello world" << endl;
    return 0;
}

#include <cstdio>
#include <cmath>
#include <utility>
using namespace std;

const double EPS = 1e-6;

int N;

bool isZero(double x)
{
    return -EPS < x && x < EPS;
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

struct CLine
{
    CPoint a, b;
    CLine() {}
    CLine(const CPoint &a, const CPoint &b) : a(a), b(b) {}
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

double operator^(const CVector &p, const CVector &q)
{
    return p.x * q.y - q.x * p.y;
}

double area(const CVector &p, const CVector &q)
{
    return (p ^ q) / 2; // with direction
}

double length2(const CVector &p)
{
    return p * p;
}

double dist2(const CPoint &p, const CLine &l)
{
    double cross = (p - l.a) ^ (l.b - l.a);
    return cross * cross / length2(l.b - l.a);
}

double dist2(const CLine &l, const CLine &m)
{
    // assure that l and m are parallel
    return dist2(l.a, m);
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
        if (isZero(dist2(l, m)))
            return make_pair(1, CPoint());
        else
            return make_pair(2, CPoint());
    }
    return make_pair(0, m.a + a1 / (a1 + a2) * (m.b - m.a));
}

int main(int argc, char const *argv[])
{
    scanf("%d", &N);
    double x1, y1, x2, y2, x3, y3, x4, y4;
    printf("INTERSECTING LINES OUTPUT\n");
    for (int i = 1; i <= N; ++i)
    {
        scanf("%lf%lf%lf%lf%lf%lf%lf%lf", &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4);
        CLine l1(CPoint(x1, y1), CPoint(x2, y2));
        CLine l2(CPoint(x3, y3), CPoint(x4, y4));
        pair<int, CPoint> result = CrossPoint(l1, l2);
        if (result.first == 1)
            printf("LINE\n");
        else if (result.first == 2)
        {
            printf("NONE\n");
        }
        else
        {
            printf("POINT %.2lf %.2lf\n", result.second.x, result.second.y);
        }
    }
    printf("END OF OUTPUT");
    return 0;
}

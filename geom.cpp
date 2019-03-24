// Mostly copied from standford ICPC notebook

#include <bits/stdc++.h>

using ll = long long;

using namespace std;

// If the algorithm is exact ll is used. If it is inexact double is used.
// If it is both then T is used.

// Tips for geometry
// 1 Remember special cases
// 1.1 coincident (e.g. point on line)
// 1.2 colinear
// 1.3 Paralell lines (no intersection or gradient, division by zero)
// 2. Remember when multiplying inequalities about possibilites of negatives
// 3. Use <= for EPS to allow EPS to be 0.
// 4. Avoid gradient use cross product instead


// BEGIN TESTED GEOMETRY

// If dealing with integers make EPS 0
#define EPS 0
#define EPSD 1e-12
#define INF LLONG_MAX

// b in [a, c).
template<typename T> bool between(T a, T b, T c) { return a <= b && b < c; }
// b in [a, c].
template<typename T> bool betweenincl(T a, T b, T c) { return a <= b && b <= c; }
// b in [a, c] or [c, a]
template<typename T> bool betweeneither(T a, T b, T c) { return between(a, b, c) || between(c, b, a); }


// On complex number we already have
// abs, norm, arg, conj and polar
template<typename T> using pt = complex<T>;

const pt<double> Idb = pt<double>(0.0, 1.0);
const pt<ll> Ill = pt<ll>(0ll, 1ll);

// standards compliant magic!
template<typename T> T& x(pt<T> &p) {
    return reinterpret_cast<T*>(&p)[0];
}
template<typename T> T& y(pt<T> &p) {
    return reinterpret_cast<T*>(&p)[1];
}

template<typename T> T dot(const pt<T> a, const pt<T> b) {
    return real(conj(a) * b);
}
template<typename T> T cross(pt<T> a, pt<T> b) {
    return imag(conj(a) * b);
}

// returns true if p is strictly inside the rectangle with corners a and b
// returns false if p is strictly outside
// returns true or false otherwise
template<typename T> bool pt_in_rect(pt<T> p, pt<T> a, pt<T> b) {
    return betweeneither(x(a), x(p), x(b)) && betweeneither(y(a), y(p), y(b));
}

// determine if lines from a to b and c to d are parallel or collinear
// n^2 overflow
template <typename T> bool lines_parallel(pt<T> a, pt<T> b, pt<T> c, pt<T> d) {
    return abs(cross(b-a, c-d)) <= EPS;
}

// determine if point is in a possibly non-convex polygon (by William Randolph
// Franklin modified by Yaseen Mowzer); returns true for strictly interior
// points, false for strictly exterior points, and false or true for the
// remaining points.
// n^2 overflow
template<typename T>
bool pt_in_pgon(pt<T> q, vector<pt<T>> &p) {
    bool c = false;
    for (int i = 0; i < (int) p.size(); i++) {
        pt<T> a = p[i], b = p[(i + 1) % p.size()];
        if (y(a) > y(b)) swap(a, b);
        pt<T> d = b - a;
        if (between(y(a), y(q), y(b))
            && x(q) * y(d) < x(a) * y(d) + x(d) * (y(q) - y(a))) {
            c = !c;
        }
    }
    return c;
}

// determine if point is on the boundary of a polygon
// n^2 overflow
bool pt_on_pgon(pt<ll> q, vector<pt<ll>> &p) {
    for (int i = 0; i < (int) p.size(); i++) {
        pt<ll> a = p[i], b = p[(i + 1) % p.size()];
        if (norm(a - q) <= EPS) return true; // Coincident with a
        if (x(a) < x(b)) swap(a, b);
        if (pt_in_rect(q, a, b)) {
            if (lines_parallel(a, b, a, q)) {
                return true;
            }
        }

    }
    return false;
}

int other_main()
{
    ios::sync_with_stdio(false);
    vector<pt<double>> points {{-1, -1}, {1, -2}, {2, 0}, {2, 2}, {-1, 1}};
    assert(pt_in_pgon({0, 0}, points) == true);
    assert(pt_in_pgon({-2, 0}, points) == false);
    assert(pt_in_pgon({0, 2}, points) == false);
    assert(pt_in_pgon({0, -2}, points) == false);

    // https://codeforces.com/contest/1030/problem/B
    int N, D, M;
    cin >> N >> D;
    vector<pt<ll>> rect {{0, D}, {D, 0}, {N, N - D}, {N - D, N}};
    cin >> M;
    for (int i = 0; i < M; ++i) {
        pt<ll> p;
        cin >> x(p) >> y(p);
        cout << ((pt_on_pgon(p, rect) || pt_in_pgon(p, rect)) ? "YES\n" : "NO\n");
    }
    return 0;
}

// END TESTED GEOMETRY

// project point z onto line through a and b
// assuming a != b (if a == b division by zero will occur).
pt<double> proj_pt_on_line(pt<double> a, pt<double> b, pt<double> z) {
    return a + (b-a)*dot(z-a, b-a)/dot(b-a, b-a);
}

// project point c onto line segment through a and b
pt<double> proj_pt_on_seg(pt<double> a, pt<double> b, pt<double> c) {
    double r = dot(b-a,b-a);
  if (fabs(r) < EPS) return a;
  r = dot(c-a, b-a)/r;
  if (r < 0) return a;
  if (r > 1) return b;
  return a + (b-a)*r;
}

// compute distance between point (x,y,z) and plane ax+by+cz=d
double dist_pt_plane(double x, double y, double z,
                     double a, double b, double c, double d)
{
  return fabs(a*x+b*y+c*z-d)/sqrt(a*a+b*b+c*c);
}


template<typename T> bool lines_collinear(pt<T> a, pt<T> b, pt<T> c, pt<T> d) {
    return lines_parallel(a, b, c, d)
        && fabs(cross(a-b, a-c)) <= EPS
        && fabs(cross(c-d, c-a)) <= EPS;
}

// determine if line segment from a to b intersects with
// line segment from c to d
template<typename T>
bool seg_intersect(pt<T> a, pt<T> b, pt<T> c, pt<T> d) {
    if (lines_collinear(a, b, c, d)) {
        if (norm(a - c) <= EPS || norm(a - d) <= EPS ||
            norm(b - c) <= EPS || norm(b - d) <= EPS) return true;
        if (dot(c-a, c-b) > 0 && dot(d-a, d-b) > 0 && dot(c-b, d-b) > 0)
            return false;
        return true;
    }
    if (cross(d-a, b-a) * cross(c-a, b-a) > 0) return false;
    if (cross(a-c, d-c) * cross(b-c, d-c) > 0) return false;
    return true;
}

// compute intersection of line passing through a and b
// with line passing through c and d, assuming that unique
// intersection exists; for segment intersection, check if
// segments intersect first
pt<double> line_intersection(pt<double> a, pt<double> b, pt<double> c, pt<double> d) {
    b=b-a; d=c-d; c=c-a;
    assert(dot(b, b) > EPS && dot(d, d) > EPS);
    return a + b*cross(c, d)/cross(b, d);
}

// compute center of circle given three points
pt<double> circle_center(pt<double> a, pt<double> b, pt<double> c) {
    b=(a+b)/2.0;
    c=(a+c)/2.0;
    return line_intersection(b, b+(a-b)*(-Idb), c, c+(a-c)*(-Idb));
}

// determine if point is on the boundary of a polygon
bool pt_on_pgon(vector<pt<double>> &p, pt<double> q) {
    for (int i = 0; i < p.size(); i++)
        if (norm(proj_pt_on_seg(p[i], p[(i+1)%p.size()], q) -  q) <= EPS)
            return true;
    return false;
}

// compute intersection of line through points a and b with
// circle centered at c with radius r > 0
// WARNING: FAILS TESTS!
vector<pt<double>> circle_line_intersection(pt<double> a, pt<double> b, pt<double> c, double r) {
    vector<pt<double>> ret;
    b = b-a;
    a = a-c;
    double A = dot(b, b);
    double B = dot(a, b);
    double C = dot(a, a) - r*r;
    double D = B*B - A*C;
    if (D < -EPSD) return ret;
    ret.push_back(c+a+b*(-B+sqrt(D+EPSD))/A);
    if (D > EPSD)
        ret.push_back(c+a+b*(-B-sqrt(D))/A);
    return ret;
}

// compute intersection of circle centered at a with radius r
// with circle centered at b with radius R
vector<pt<double>> circle_circle_intersection(pt<double> a, pt<double> b, double r, double R) {
    vector<pt<double>> ret;
    double d = sqrt(norm(a - b));
    if (d > r+R || d+min(r, R) < max(r, R)) return ret;
    double x = (d*d-R*R+r*r)/(2*d);
    double y = sqrt(r*r-x*x);
    pt<double> v = (b-a)/d;
    ret.push_back(a+v*x + v*Idb*y);
    if (y > 0)
        ret.push_back(a+v*x - v*Idb*y);
    return ret;
}

// This code computes the area or centroid of a (possibly nonconvex)
// polygon, assuming that the coordinates are listed in a clockwise or
// counterclockwise fashion.  Note that the centroid is often known as
// the "center of gravity" or "center of mass".
double signed_area(vector<pt<double>> &p) {
    double area = 0;
    for(int i = 0; i < p.size(); i++) {
        int j = (i + 1) % p.size();
        area += cross(p[i], p[j]);
    }
    return area / 2.0;
}

double area(vector<pt<double>> &p) {
    return fabs(signed_area(p));
}

pt<double> centroid(vector<pt<double>> &p) {
    pt<double> c(0.0 , 0.0);
    double scale = 6.0 * signed_area(p);
    for (int i = 0; i < p.size(); i++){
        int j = (i+1) % p.size();
        c = c + (p[i]+p[j])*cross(p[i], p[j]);
    }
    return c / scale;
}

// tests whether or not a given polygon (in CW or CCW order) is simple
template<typename T>
bool is_simple(const vector<pt<T>> &p) {
    for (int i = 0; i < p.size(); i++) {
        for (int k = i+1; k < p.size(); k++) {
            int j = (i+1) % p.size();
            int l = (k+1) % p.size();
            if (i == l || j == k) continue;
            if (seg_intersect(p[i], p[j], p[k], p[l]))
                return false;
        }
    }
    return true;
}

int secondmain() {
    using PT = pt<double>;
    // expected: (-5,2)
    cerr << pt<ll>(2,5)*Ill << endl;

    // expected: (5,-2)
    cerr << pt<ll>(2,5)*(-Ill) << endl;

    // expected: (-5,2)
    cerr << pt<double>(2,5) * polar<double>(1.0, M_PI/2) << endl;

    // expected: (5,2)
    cerr << proj_pt_on_line(PT(-5,-2), PT(10,4), PT(3,7)) << endl;

    // expected: (5,2) (7.5,3) (2.5,1)
    cerr << proj_pt_on_seg(PT(-5,-2), PT(10,4), PT(3,7)) << " "
         << proj_pt_on_seg(PT(7.5,3), PT(10,4), PT(3,7)) << " "
         << proj_pt_on_seg(PT(-5,-2), PT(2.5,1), PT(3,7)) << endl;

    // expected: 6.78903
    cerr << dist_pt_plane(4,-4,3,2,-2,5,-8) << endl;

    // expected: 1 0 1
    cerr << lines_parallel(PT(1,1), PT(3,5), PT(2,1), PT(4,5)) << " "
         << lines_parallel(PT(1,1), PT(3,5), PT(2,0), PT(4,5)) << " "
         << lines_parallel(PT(1,1), PT(3,5), PT(5,9), PT(7,13)) << endl;

    // expected: 0 0 1
    cerr << lines_collinear(PT(1,1), PT(3,5), PT(2,1), PT(4,5)) << " "
         << lines_collinear(PT(1,1), PT(3,5), PT(2,0), PT(4,5)) << " "
         << lines_collinear(PT(1,1), PT(3,5), PT(5,9), PT(7,13)) << endl;

    // expected: 1 1 1 0
    cerr << seg_intersect(PT(0,0), PT(2,4), PT(3,1), PT(-1,3)) << " "
         << seg_intersect(PT(0,0), PT(2,4), PT(4,3), PT(0,5)) << " "
         << seg_intersect(PT(0,0), PT(2,4), PT(2,-1), PT(-2,1)) << " "
         << seg_intersect(PT(0,0), PT(2,4), PT(5,5), PT(1,7)) << endl;

    // expected: (1,2)
    cerr << line_intersection(PT(0,0), PT(2,4), PT(3,1), PT(-1,3)) << endl;

    // expected: (1,1)
    cerr << circle_center(PT(-3,4), PT(6,1), PT(4,5)) << endl;

    vector<PT> v;
    v.push_back(PT(0,0));
    v.push_back(PT(5,0));
    v.push_back(PT(5,5));
    v.push_back(PT(0,5));

    // expected: 1 1 1 0 0
    cerr << pt_in_pgon(PT(2,2), v) << " "
         << pt_in_pgon(PT(2,0), v) << " "
         << pt_in_pgon(PT(0,2), v) << " "
         << pt_in_pgon(PT(5,2), v) << " "
         << pt_in_pgon(PT(2,5), v) << endl;

    // expected: 0 1 1 1 1
    cerr << pt_on_pgon(v, PT(2,2)) << " "
         << pt_on_pgon(v, PT(2,0)) << " "
         << pt_on_pgon(v, PT(0,2)) << " "
         << pt_on_pgon(v, PT(5,2)) << " "
         << pt_on_pgon(v, PT(2,5)) << endl;

    // expected: (1,6)
    //           (5,4) (4,5)
    //           blank line
    //           (4,5) (5,4)
    //           blank line
    //           (4,5) (5,4)
    vector<PT> u = circle_line_intersection(PT(0,6), PT(2,6), PT(1,1), 5);
    for (int i = 0; i < u.size(); i++) cerr << u[i] << " "; cerr << endl;
    u = circle_line_intersection(PT(0,9), PT(9,0), PT(1,1), 5);
    for (int i = 0; i < u.size(); i++) cerr << u[i] << " "; cerr << endl;
    u = circle_line_intersection(PT(1,1), PT(10,10), 5, 5);
    for (int i = 0; i < u.size(); i++) cerr << u[i] << " "; cerr << endl;
    u = circle_line_intersection(PT(1,1), PT(8,8), 5, 5);
    for (int i = 0; i < u.size(); i++) cerr << u[i] << " "; cerr << endl;
    u = circle_line_intersection(PT(1,1), PT(4.5,4.5), 10, sqrt(2.0)/2.0);
    for (int i = 0; i < u.size(); i++) cerr << u[i] << " "; cerr << endl;
    u = circle_line_intersection(PT(1,1), PT(4.5,4.5), 5, sqrt(2.0)/2.0);
    for (int i = 0; i < u.size(); i++) cerr << u[i] << " "; cerr << endl;

    // area should be 5.0
    // centroid should be (1.1666666, 1.166666)
    PT pa[] = { PT(0,0), PT(5,0), PT(1,1), PT(0,5) };
    vector<PT> p(pa, pa+4);
    PT c = centroid(p);
    cerr << "Area: " << area(p) << endl;
    cerr << "Centroid: " << c << endl;

    return 0;
}

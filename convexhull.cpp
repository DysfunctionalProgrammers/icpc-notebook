// Mostly copied from stanford ICPC notebook

// Compute the 2D convex hull of a set of points using the monotone chain
// algorithm.  Eliminate redundant points from the hull if REMOVE_REDUNDANT is
// #defined.
//
// Running time: O(n log n)
//
//   INPUT:   a vector of input points, unordered.
//   OUTPUT:  a vector of points in the convex hull, counterclockwise, starting
//            with bottommost/leftmost point

#include <bits/stdc++.h>
#include "geom.cpp"

using namespace std;

using PT = pt<double>;

//#define REMOVE_REDUNDANT

double area2(PT a, PT b, PT c) {
    vector<PT> s = {a, b, c};
    return signed_area(s);
}

bool operator<(PT a, PT b) {
    return make_pair(y(a), x(a)) < make_pair(y(b), x(b));
}
bool operator<=(PT a, PT b) {
    return make_pair(y(a), x(a)) <= make_pair(y(b), x(b));
}

#ifdef REMOVE_REDUNDANT
template <typename T> bool between(PT a, PT b, PT c) {
    return (fabs(area2(a,b,c)) <= EPS && (x(a)-x(b))*(x(c)-x(b)) <= 0 && (y(a)-y(b))*(y(c)-y(b)) <= 0);
}
#endif

void ConvexHull(vector<PT> &pts) {
    sort(pts.begin(), pts.end(), [] (PT a, PT b) {return a < b;});
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    vector<PT> up, dn;
    for (int i = 0; i < pts.size(); i++) {
        while (up.size() > 1 && area2(up[up.size()-2], up.back(), pts[i]) >= 0) up.pop_back();
        while (dn.size() > 1 && area2(dn[dn.size()-2], dn.back(), pts[i]) <= 0) dn.pop_back();
        up.push_back(pts[i]);
        dn.push_back(pts[i]);
    }
    pts = dn;
    for (int i = (int) up.size() - 2; i >= 1; i--) pts.push_back(up[i]);

#ifdef REMOVE_REDUNDANT
    if (pts.size() <= 2) return;
    dn.clear();
    dn.push_back(pts[0]);
    dn.push_back(pts[1]);
    for (int i = 2; i < pts.size(); i++) {
        if (between(dn[dn.size()-2], dn[dn.size()-1], pts[i])) dn.pop_back();
        dn.push_back(pts[i]);
    }
    if (dn.size() >= 3 && between(dn.back(), dn[0], dn[1])) {
        dn[0] = dn.back();
        dn.pop_back();
    }
    pts = dn;
#endif
}

// BEGIN CUT
// The following code solves SPOJ problem #26: Build the Fence (BSHEEP)

struct cmp {
    bool operator()(PT a, PT b) {
        return a < b;
    }
};

int main() {
    int t;
    scanf("%d", &t);
    for (int caseno = 0; caseno < t; caseno++) {
        int n;
        scanf("%d", &n);
        vector<PT> v(n);
        for (int i = 0; i < n; i++) scanf("%lf%lf", &x(v[i]), &y(v[i]));
        vector<PT> h(v);
        map<PT, int, cmp> index;
        for (int i = n-1; i >= 0; i--) index[v[i]] = i+1;
        ConvexHull(h);

        double len = 0;
        for (int i = 0; i < h.size(); i++) {
            double dx = x(h[i]) - x(h[(i+1)%h.size()]);
            double dy = y(h[i]) - y(h[(i+1)%h.size()]);
            len += sqrt(dx*dx+dy*dy);
        }

        if (caseno > 0) printf("\n");
        printf("%.2f\n", len);
        for (int i = 0; i < h.size(); i++) {
            if (i > 0) printf(" ");
            printf("%d", index[h[i]]);
        }
        printf("\n");
    }
}

// END CUT

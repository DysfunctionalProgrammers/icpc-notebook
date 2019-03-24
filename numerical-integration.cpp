#include <bits/stdc++.h>

using namespace std;

double f(double x) {
	return pow(M_E, -x * x); // dummy
}

// error = - (b - a)^3/(12N^2) * f''(c) for some c in [a, b].
double integrate(double a, double b, int n) {
	double res = 0.5 * f(a) + 0.5 * f(b);
	for (int i = 1; i < n; ++i) {
		res += f(a + (b - a) * i / n);
	}
	return res * (b - a) / n;
}

// Useful for binary searches
bool accurate_to(int decimal_places, double high, double low) {
    double p = pow(10, decimal_places);
    return round(high * p) == round(low * p);
}

int main() {
	cout << fixed << setprecision(10) << integrate(-10,10,10000) << '\n';
	cout << fixed << setprecision(10) << integrate(-10,10,1000) << '\n';
	cout << fixed << setprecision(10) << integrate(-10,10,100) << '\n';
	cout << fixed << setprecision(10) << integrate(-10,10,10) << '\n';
}

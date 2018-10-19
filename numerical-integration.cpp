#include <bits/stdc++.h>

using namespace std;

double f(double x) {
	return pow(M_E, -x * x); // dummy
}

double integrate(double a, double b, int n) {
	double res = 0.5 * f(a) + 0.5 * f(b);
	for (int i = 1; i < n; ++i) {
		res += f(a + (b - a) * i / n);
	}
	return res * (b - a) / n;
}

int main() {
	cout << fixed << setprecision(10) << integrate(-10,10,10000) << '\n';
	cout << fixed << setprecision(10) << integrate(-10,10,1000) << '\n';
	cout << fixed << setprecision(10) << integrate(-10,10,100) << '\n';
	cout << fixed << setprecision(10) << integrate(-10,10,10) << '\n';
}
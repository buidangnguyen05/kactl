#include "../utilities/template.h"

#include "../../content/numerical/IntegrateAdaptive.h"

int main() {
	mt19937 rng(1);
	auto ok = [](d got, d want, d tol) {
		if (abs(got - want) > tol * max(1.0, abs(want))) {
			cout << "got " << got << " want " << want << endl;
			assert(false);
		}
	};
	// random polynomials up to degree 6, exact antiderivative
	rep(it,0,1000) {
		int deg = rng() % 7;
		vector<d> c(deg + 1);
		for (d& x : c) x = (int)(rng() % 21) - 10;
		d a = (int)(rng() % 21) - 10, b = a + 1 + rng() % 10;
		auto f = [&](d x) { d r = 0; for (int i = deg; i >= 0; i--) r = r*x + c[i]; return r; };
		auto F = [&](d x) { d r = 0; for (int i = deg; i >= 0; i--) r = r*x + c[i] / (i+1); return r*x; };
		d want = F(b) - F(a);
		ok(quad(a, b, f), want, 1e-6);
		ok(quadN(a, b, f), want, 1e-6);
	}
	// smooth transcendental functions
	ok(quad(0, M_PI, [](d x) { return sin(x); }), 2, 1e-9);
	ok(quadN(0, M_PI, [](d x) { return sin(x); }), 2, 1e-9);
	ok(quad(0, 1, [](d x) { return exp(x); }), exp(1) - 1, 1e-9);
	ok(quadN(0, 1, [](d x) { return exp(x); }), exp(1) - 1, 1e-9);
	// sphere volume via nested quad, as in the usage example (discontinuous integrand)
	d sphere = quad(-1, 1, [](d x) {
		return quad(-1, 1, [&](d y) {
		return quad(-1, 1, [&](d z) {
		return x*x + y*y + z*z < 1; });});});
	ok(sphere, 4 * M_PI / 3, 1e-3);
	// narrow spike: adaptive misses it from the first samples, quadN catches it
	auto spike = [](d x) { return exp(-1e4 * (x - 0.3) * (x - 0.3)); };
	d spikeArea = sqrt(M_PI / 1e4);
	ok(quadN(-1, 1, spike, 1000), spikeArea, 1e-6);
	cout << "Tests passed!" << endl;
}

/**
 * Author: Simon Lindholm
 * Date: 2015-02-11
 * License: CC0
 * Source: Wikipedia
 * Description: Fast integration using an adaptive Simpson's rule.
 *  quadN is plain Simpson with $2n$ equal steps (error $\propto h^4$): use it
 *  when $f$ has narrow spikes that adaptive sampling could miss, or split
 *  the integral at known discontinuities instead.
 * Usage:
	double sphereVolume = quad(-1, 1, [](double x) {
	return quad(-1, 1, [\&](double y) {
	return quad(-1, 1, [\&](double z) {
	return x*x + y*y + z*z < 1; });});});
 * Status: stress-tested
 */
#pragma once

typedef double d;
#define S(a,b) (f(a) + 4*f((a+b) / 2) + f(b)) * (b-a) / 6

template <class F>
d rec(F& f, d a, d b, d eps, d S) {
	d c = (a + b) / 2;
	d S1 = S(a, c), S2 = S(c, b), T = S1 + S2;
	if (abs(T - S) <= 15 * eps || b - a < 1e-10)
		return T + (T - S) / 15;
	return rec(f, a, c, eps / 2, S1) + rec(f, c, b, eps / 2, S2);
}
template<class F>
d quad(d a, d b, F f, d eps = 1e-8) {
	return rec(f, a, b, eps, S(a, b));
}
template<class F> d quadN(d a, d b, F f, int n = 1000) {
	d h = (b - a) / 2 / n, v = f(a) + f(b);
	rep(i,1,n*2) v += f(a + i*h) * (i&1 ? 4 : 2);
	return v * h / 3;
}

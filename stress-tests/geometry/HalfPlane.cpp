#include "../utilities/template.h"

#include "../../content/geometry/HalfPlane.h"
#include "../../content/geometry/PolygonCut.h"
#include "../../content/geometry/PolygonArea.h"

int main() {
	mt19937 rng(4);
	int nonempty = 0, empty = 0, bounded = 0;
	const double B = 50;
	auto area = [](vector<P> v) { return sz(v) >= 3 ? polygonArea2(v) / 2 : 0.0; };
	rep(it,0,400000) {
		int n = rng() % 8, C = 5;
		bool fewDirs = rng() % 2; // many parallel / antiparallel lines
		vector<L> h;
		rep(i,0,n) {
			P a(int(rng() % (2*C+1)) - C, int(rng() % (2*C+1)) - C), d;
			do {
				if (fewDirs) { int k = rng() % 4; d = vector<P>{P(1,0), P(0,1), P(-1,0), P(1,1)}[k] * (1 + rng() % 2); }
				else d = P(int(rng() % 7) - 3, int(rng() % 7) - 3);
				if (fewDirs && rng() % 2) d = d * -1;
			} while (d.x == 0 && d.y == 0);
			h.push_back({a, a + d});
		}
		// brute: clip the box, keeping the left side of each line
		vector<P> poly = {P(-B, -B), P(B, -B), P(B, B), P(-B, B)};
		for (L l : h) poly = polygonCut(poly, l[1], l[0]);
		double want = area(poly);
		vector<P> got = halfPlanes(h, B);
		double a = area(got);
		if (abs(a - want) > 1e-6) {
			cout << "want " << want << " got " << a << endl;
			for (L l : h) cout << l[0] << " -> " << l[1] << endl;
			return 1;
		}
		if (want < 1e-6) { empty++; continue; }
		nonempty++;
		// every returned vertex satisfies every half-plane
		for (P p : got) for (L l : h)
			assert((l[1] - l[0]).cross(p - l[0]) > -1e-6);
		// if the region does not touch the box it is bounded, so a huge box
		// must give the same area (brute force is too imprecise at 1e9)
		bool inside = true;
		for (P p : poly) inside &= max(abs(p.x), abs(p.y)) < B - 1;
		if (inside) {
			bounded++;
			double big = area(halfPlanes(h, 1e9));
			assert(abs(big - want) < 1e-6 * max(1.0, want));
		}
	}
	cout << "Tests passed! (" << nonempty << " nonempty, " << bounded
		<< " bounded, " << empty << " empty)" << endl;
}

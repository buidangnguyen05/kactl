#include "../utilities/template.h"

#include "../../content/geometry/MinkowskiSum.h"
#include "../../content/geometry/ConvexHull.h"

int main() {
	mt19937 rng(5);
	int sizes = 0;
	rep(it,0,100000) {
		auto gen = [&]() {
			int n = 1 + rng() % 9, C = 1 + rng() % 10;
			vector<P> v;
			rep(i,0,n) v.push_back(P(int(rng() % (2*C+1)) - C, int(rng() % (2*C+1)) - C));
			return convexHull(v);
		};
		vector<P> a = gen(), b = gen();
		vector<P> sums;
		for (P p : a) for (P q : b) sums.push_back(p + q);
		vector<P> want = convexHull(sums), got = minkowski(a, b);
		// same polygon, same CCW order, possibly rotated
		assert(sz(got) == sz(want));
		int k = find(all(got), want[0]) - got.begin();
		assert(k < sz(got));
		rep(i,0,sz(want)) assert(got[(i + k) % sz(got)] == want[i]);
		sizes += sz(got);
	}
	cout << "Tests passed! (" << sizes << " vertices checked)" << endl;
}

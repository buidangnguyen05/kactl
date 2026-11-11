#include "../utilities/template.h"

#include "../../content/geometry/SegmentsIntersect.h"

int main() {
	mt19937 rng(3);
	int found = 0, none = 0;
	rep(it,0,200000) {
		int n = 1 + rng() % 8, C = 2 + rng() % 8;
		bool vert = rng() % 3 == 0;
		vector<array<P, 2>> s;
		while (sz(s) < n) {
			P a(rng() % C, rng() % C), b(rng() % C, rng() % C);
			if (vert && rng() % 2) b.x = a.x;
			if (a == b) continue;
			s.push_back({a, b});
		}
		bool any = false;
		rep(i,0,n) rep(j,0,i)
			if (sz(segInter(s[i][0], s[i][1], s[j][0], s[j][1]))) any = true;
		auto [a, b] = segmentsIntersect(s);
		if (!any) { assert(a == -1 && b == -1); none++; continue; }
		assert(a != -1 && a != b);
		assert(sz(segInter(s[a][0], s[a][1], s[b][0], s[b][1])));
		found++;
	}
	// larger: n segments with no intersections (disjoint short pieces on a grid)
	rep(it,0,200) {
		int n = 1 + rng() % 300;
		vector<array<P, 2>> s;
		rep(i,0,n) {
			ll x = 10 * (rng() % 1000), y = 10 * i;
			s.push_back({P(x, y), P(x + 1 + rng() % 8, y + (ll)(rng() % 9))});
		}
		assert(segmentsIntersect(s) == pii(-1, -1));
	}
	cout << "Tests passed! (" << found << " intersecting, " << none << " disjoint)" << endl;
}

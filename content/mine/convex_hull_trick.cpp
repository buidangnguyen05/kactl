// Convex hull trick in DP
// Slopes and queries sorted in decreasing order, querying maximum

// Slopes increasing: Flip comparison
// Querying minimum: Flip comparison
// Queries sorted in reverse order of slopes: Check the back instead of the front

struct Line {
	ll A, B;
	Line() : A(0), B(0) {}
	Line (ll _a, ll _b) {
		A = _a, B = _b;
	}
};

struct ConvexHullTrick {
	deque<Line> q;

	long double G (Line x, Line y) {
		return 1.0 * (x.B - y.B) / (y.A - x.A);
	}

	ll calc(Line x, ll v) {
		return x.A * v + x.B;
	}

	void add(ll x, ll y) {
		Line cur = Line(x, y);
		if (q.size() && q.back().A == x && q.back().B >= y) return;
		while (q.size() && q.back().A == x && q.back().B <= y) q.pop_back();
		while (q.size() > 1 && G(cur, q[q.size() - 1]) >= G(q[q.size() - 1], q[q.size() - 2])) q.pop_back();
		q.push_back(cur);
	}

	ll get(ll x) {
		while (q.size() > 1 && calc(q[0], x) <= calc(q[1], x)) q.pop_front();
		return calc(q[0], x);
	}
};
/**
 * Author: Bui Dang Nguyen
 * Date: 2026-11-11
 * License: CC0
 * Source: https://cp-algorithms.com/geometry/halfplane-intersection.html
 * Description: Intersection of half-planes, each the region left of (or on)
 *  the directed line h[i][0] $\to$ h[i][1], clipped to the box $[-B,B]^2$.
 *  Returns the convex polygon CCW, or empty if the intersection is empty
 *  (a zero-area result may come back as a degenerate polygon).
 *  Used for: polygon kernel, area of a feasible region, binary search
 *  on an inward offset (largest inscribed circle).
 * Time: O(N \log N)
 * Status: stress-tested
 */
#pragma once

#include "lineIntersection.h"

typedef Point<double> P;
typedef array<P, 2> L;
vector<P> halfPlanes(vector<L> h, double B = 1e9) {
	P c[4] = {P(-B, -B), P(B, -B), P(B, B), P(-B, B)};
	rep(i,0,4) h.push_back({c[i], c[(i+1) % 4]});
	auto dir = [](L l) { return l[1] - l[0]; };
	sort(all(h), [&](L a, L b) {
		return dir(a).angle() < dir(b).angle(); });
	auto out = [&](L l, P p) { return dir(l).cross(p - l[0]) < -1e-9; };
	auto X = [](L a, L b) {
		return lineInter(a[0], a[1], b[0], b[1]).second; };
	deque<L> q;
	auto bad = [&](L l, int i, int j) { return out(l, X(q[i], q[j])); };
	for (L l : h) {
		while (sz(q) > 1 && bad(l, sz(q)-1, sz(q)-2)) q.pop_back();
		while (sz(q) > 1 && bad(l, 0, 1)) q.pop_front();
		if (sz(q) && abs(dir(l).cross(dir(q.back()))) < 1e-9) {
			if (dir(l).dot(dir(q.back())) < 0) return {};
			if (!out(l, q.back()[0])) continue;
			q.pop_back();
		}
		q.push_back(l);
	}
	while (sz(q) > 2 && bad(q[0], sz(q)-1, sz(q)-2)) q.pop_back();
	while (sz(q) > 2 && bad(q.back(), 0, 1)) q.pop_front();
	if (sz(q) < 3) return {};
	vector<P> r;
	rep(i,0,sz(q)) r.push_back(X(q[i], q[(i+1) % sz(q)]));
	return r;
}

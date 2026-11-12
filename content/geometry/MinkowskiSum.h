/**
 * Author: Bui Dang Nguyen
 * Date: 2026-11-11
 * License: CC0
 * Source: https://cp-algorithms.com/geometry/minkowski.html
 * Description: Minkowski sum $\{a+b : a \in A, b \in B\}$ of two convex polygons,
 *  given CCW without collinear points (e.g.\ from convexHull); returned the same way.
 *  Polygons collide iff $A \oplus (-B)$ contains the origin (inHull); their
 *  distance is the distance from the origin to $A \oplus (-B)$.
 * Time: O(N + M)
 * Status: stress-tested
 */
#pragma once

#include "Point.h"

typedef Point<ll> P;
vector<P> minkowski(vector<P> a, vector<P> b) {
	auto prep = [](vector<P>& v) {
		rotate(v.begin(), min_element(all(v), [](P p, P q) {
			return pair(p.y, p.x) < pair(q.y, q.x); }), v.end());
		v.push_back(v[0]); v.push_back(v[1 % (sz(v) - 1)]);
	};
	int n = sz(a), m = sz(b), i = 0, j = 0;
	prep(a); prep(b);
	vector<P> r;
	while (i < n || j < m) {
		r.push_back(a[i] + b[j]);
		auto c = (a[i+1] - a[i]).cross(b[j+1] - b[j]);
		if (c >= 0 && i < n) i++;
		if (c <= 0 && j < m) j++;
	}
	return r;
}

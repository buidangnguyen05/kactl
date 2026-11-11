/**
 * Author: Bui Dang Nguyen
 * Date: 2026-11-11
 * License: CC0
 * Source: Shamos-Hoey; https://cp-algorithms.com/geometry/intersecting_segments.html
 * Description: Returns indices of some pair of segments that intersect
 *  (touching counts), or $\{-1,-1\}$ if none do. Segments must have positive length.
 *  Sweep line keeping active segments sorted by $y$; only neighbours are checked.
 *  Watch overflow: products of three coordinates, like segInter.
 * Time: O(N \log N)
 * Status: stress-tested
 */
#pragma once

#include "SegmentIntersection.h"

typedef Point<ll> P;
pii segmentsIntersect(vector<array<P, 2>> s) {
	vector<array<ll, 3>> ev;
	rep(i,0,sz(s)) {
		if (s[i][1] < s[i][0]) swap(s[i][0], s[i][1]);
		ev.push_back({s[i][0].x, 0, i});
		ev.push_back({s[i][1].x, 1, i});
	}
	sort(all(ev));
	auto y = [&](int i, ll x) { // y of segment i at x, as fraction
		auto [p, q] = s[i]; ll d = q.x - p.x;
		if (!d) return pair(p.y, 1LL);
		return pair(p.y * d + (q.y - p.y) * (x - p.x), d);
	};
	auto cmp = [&](int i, int j) {
		ll x = max(s[i][0].x, s[j][0].x);
		auto [a, b] = y(i, x); auto [c, d] = y(j, x);
		return a * d < c * b;
	};
	auto hit = [&](int i, int j) {
		return sz(segInter(s[i][0], s[i][1], s[j][0], s[j][1])) > 0; };
	set<int, decltype(cmp)> st(cmp);
	vector<decltype(st)::iterator> at(sz(s));
	for (auto [x, t, i] : ev) {
		if (!t) {
			auto it = st.lower_bound(i);
			if (it != st.end() && hit(*it, i)) return {*it, i};
			if (it != st.begin() && hit(*prev(it), i)) return {*prev(it), i};
			at[i] = st.insert(it, i);
		} else {
			auto it = st.erase(at[i]);
			if (it != st.begin() && it != st.end() && hit(*prev(it), *it))
				return {*prev(it), *it};
		}
	}
	return {-1, -1};
}

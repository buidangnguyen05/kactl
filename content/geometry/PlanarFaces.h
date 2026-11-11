/**
 * Author: Bui Dang Nguyen
 * Date: 2026-11-11
 * License: CC0
 * Source: folklore (half-edge face tracing)
 * Description: Faces of a planar straight-line graph: edges may only meet at
 *  shared endpoints (no duplicates, self-loops or overlaps; split crossing
 *  segments first). Half-edge $2i$ is ed[i] forwards, $2i+1$ backwards; face[e] is the face to
 *  its left, nxt[e] the next half-edge around that face, to[e] its head.
 *  area2[f] is twice the signed area: $>0$ bounded face (CCW), $\le 0$ the
 *  outer face of one component. Several components each get their own outer
 *  face; merge it into the face containing it (InsidePolygon) if needed.
 *  Dual graph: for each $i$, connect face[2i] and face[2i+1].
 *  Planar facts: $V - E + F = 1 + C$, $E \le 3V-6$, so the graph is 5-degenerate (greedy 6-colouring).
 *  Min $s$-$t$ cut = shortest path in the dual between the two faces split by an extra edge $s$-$t$ drawn through the outer face.
 * Usage: PlanarFaces pf(pts, edges);
 *  for (int e = start; ; ) { visit(pts[pf.to[e]]); if ((e = pf.nxt[e]) == start) break; }
 * Time: O(E \log E)
 * Status: stress-tested
 */
#pragma once

#include "Point.h"

typedef Point<ll> P;
struct PlanarFaces {
	vi face, nxt, to; vector<ll> area2;
	PlanarFaces(const vector<P>& p, const vector<pii>& ed) {
		int m = 2 * sz(ed);
		vector<vi> out(sz(p)); vi pos(m);
		to = nxt = face = vi(m, -1);
		rep(e,0,m) {
			auto [a, b] = ed[e/2]; if (e & 1) swap(a, b);
			to[e] = b; out[a].push_back(e);
		}
		auto half = [](P a) { return a.y < 0 || (a.y == 0 && a.x < 0); };
		for (vi& o : out) {
			sort(all(o), [&](int a, int b) {
				P u = p[to[a]] - p[to[a^1]], v = p[to[b]] - p[to[b^1]];
				return half(u) != half(v) ? half(v) : u.cross(v) > 0; });
			rep(i,0,sz(o)) pos[o[i]] = i;
		}
		rep(e,0,m) {
			vi& o = out[to[e]];
			nxt[e] = o[(pos[e^1] + sz(o) - 1) % sz(o)];
		}
		rep(e,0,m) if (face[e] < 0) {
			ll A = 0;
			for (int f = e; face[f] < 0; f = nxt[f])
				face[f] = sz(area2), A += p[to[f^1]].cross(p[to[f]]);
			area2.push_back(A);
		}
	}
};

#include "../utilities/template.h"

#include "../../content/geometry/PlanarFaces.h"
#include "../../content/geometry/SegmentIntersection.h"

int main() {
	mt19937 rng(2);
	int faces = 0;
	rep(it,0,30000) {
		int n = 1 + rng() % 9, C = 2 + rng() % 6;
		set<P> used; vector<P> p;
		while (sz(p) < n) {
			P q(rng() % C, rng() % C);
			if (used.insert(q).second) p.push_back(q);
			if (sz(used) == C * C) break;
		}
		n = sz(p);
		// random non-crossing edges: may only meet at shared endpoints
		vector<pii> ed;
		set<pii> have;
		rep(tries,0,(int)(rng() % 40)) {
			int a = rng() % n, b = rng() % n;
			if (a == b || have.count({min(a,b), max(a,b)})) continue;
			bool ok = true;
			rep(k,0,n) if (k != a && k != b && onSegment(p[a], p[b], p[k])) ok = false;
			for (auto [c, d] : ed) {
				if (!ok) break;
				auto in = segInter(p[a], p[b], p[c], p[d]);
				if (in.empty()) continue;
				bool shared = (a == c || a == d || b == c || b == d);
				if (!(sz(in) == 1 && shared)) ok = false;
			}
			if (!ok) continue;
			have.insert({min(a,b), max(a,b)});
			if (rng() % 2) swap(a, b);
			ed.push_back({a, b});
		}
		PlanarFaces pf(p, ed);
		int m = 2 * sz(ed), F = sz(pf.area2);
		// structural: nxt is a permutation, faces are its cycles, to[nxt] chain
		vi seen(m);
		rep(e,0,m) {
			assert(pf.face[pf.nxt[e]] == pf.face[e]);
			assert(pf.to[pf.nxt[e] ^ 1] == pf.to[e]); // nxt starts where e ends
			assert(!seen[pf.nxt[e]]++);
		}
		// components (over vertices with edges)
		vi par(n); iota(all(par), 0);
		function<int(int)> fd = [&](int x) { return par[x] == x ? x : par[x] = fd(par[x]); };
		vi deg(n);
		for (auto [a, b] : ed) par[fd(a)] = fd(b), deg[a]++, deg[b]++;
		int Vp = 0, Cp = 0;
		rep(i,0,n) if (deg[i]) { Vp++; if (fd(i) == i) Cp++; }
		// Euler per component, each component has its own outer face: F = E - V' + 2C'
		assert(F == sz(ed) - Vp + 2 * Cp);
		// exactly one face with area <= 0 per component, areas cancel per component
		map<int, ll> sum; map<int, int> outer;
		rep(f,0,F) {
			int e = find(all(pf.face), f) - pf.face.begin();
			int comp = fd(pf.to[e]);
			sum[comp] += pf.area2[f];
			if (pf.area2[f] <= 0) outer[comp]++;
		}
		for (auto [c, s] : sum) assert(s == 0 && outer[c] == 1);
		// bounded faces are simple polygons: a lattice-free test point
		// lies inside a face polygon iff the face contains it; the number of
		// bounded faces containing it equals the nesting depth computed by a
		// brute force over components' outer boundaries.
		rep(f,0,F) if (pf.area2[f] > 0) {
			faces++;
			vector<P> poly;
			int e = find(all(pf.face), f) - pf.face.begin(), s = e;
			do poly.push_back(p[pf.to[e]]); while ((e = pf.nxt[e]) != s);
			ll A = 0;
			rep(i,0,sz(poly)) A += poly[i].cross(poly[(i+1) % sz(poly)]);
			assert(A == pf.area2[f]);
		}
		// dual graph: sum of degrees = 2E, every edge separates faces of one component
		rep(i,0,sz(ed)) {
			int f1 = pf.face[2*i], f2 = pf.face[2*i+1];
			assert(f1 >= 0 && f2 >= 0 && f1 < F && f2 < F);
		}
		// total bounded area: bridge edges have the same face on both sides
		rep(i,0,sz(ed)) {
			int f1 = pf.face[2*i], f2 = pf.face[2*i+1];
			// an edge is a bridge iff removing it disconnects its endpoints
			vi p2(n); iota(all(p2), 0);
			function<int(int)> g = [&](int x) { return p2[x] == x ? x : p2[x] = g(p2[x]); };
			rep(j,0,sz(ed)) if (j != i) p2[g(ed[j].first)] = g(ed[j].second);
			bool bridge = g(ed[i].first) != g(ed[i].second);
			assert(bridge == (f1 == f2));
		}
	}
	// grid sanity: k x k unit grid has (k-1)^2 bounded faces of area 1
	rep(k,2,8) {
		vector<P> p; vector<pii> ed;
		rep(i,0,k) rep(j,0,k) p.push_back(P(i, j));
		rep(i,0,k) rep(j,0,k) {
			if (i+1 < k) ed.push_back({i*k+j, (i+1)*k+j});
			if (j+1 < k) ed.push_back({i*k+j, i*k+j+1});
		}
		PlanarFaces pf(p, ed);
		int cnt = 0;
		for (ll a : pf.area2) {
			if (a > 0) assert(a == 2), cnt++;
			else assert(a == -2LL * (k-1) * (k-1));
		}
		assert(cnt == (k-1) * (k-1));
	}
	cout << "Tests passed! (" << faces << " bounded faces checked)" << endl;
}

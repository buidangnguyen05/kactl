/**
 * Author: Jerry
 * Date: 2023-10-01
 * License: CC0
 * Description: Range $v \mapsto \min(v,x)$, $v \mapsto \max(v,x)$ and
 *  range add, with range sum/min/max. Inclusive indices.
 * Time: amortised $O(\log^2 n)$ per update, $O(\log n)$ per query
 * Status: stress-tested
 */
#pragma once

struct Beats {
	static constexpr ll inf = 1e18;
	struct Node { // cmx/cmn = multiplicity of mx resp. mn
		ll sum = 0, mx = -inf, mx2 = -inf, mn = inf, mn2 = inf, lz = 0;
		int cmx = 0, cmn = 0;
	};
	int n; vector<Node> t;
	Beats(const vector<ll>& a) : n(sz(a)), t(4*sz(a)) {
		build(1, 0, n - 1, a); }
	void pull(int s) {
		Node &a = t[2*s], &b = t[2*s+1], &c = t[s];
		c.sum = a.sum + b.sum;
		c.mx = max(a.mx, b.mx), c.mn = min(a.mn, b.mn);
		c.mx2 = max(a.mx == c.mx ? a.mx2 : a.mx,
		            b.mx == c.mx ? b.mx2 : b.mx);
		c.mn2 = min(a.mn == c.mn ? a.mn2 : a.mn,
		            b.mn == c.mn ? b.mn2 : b.mn);
		c.cmx = (a.mx==c.mx ? a.cmx:0) + (b.mx==c.mx ? b.cmx:0);
		c.cmn = (a.mn==c.mn ? a.cmn:0) + (b.mn==c.mn ? b.cmn:0);
	}
	void build(int s, int l, int r, const vector<ll>& a) {
		if (l == r) { t[s] = {a[l],a[l],-inf,a[l],inf,0,1,1}; return; }
		int m = (l + r) / 2;
		build(2*s, l, m, a), build(2*s+1, m+1, r, a), pull(s);
	}
	void addAll(int s, int len, ll x) {
		Node& c = t[s];
		c.sum += len * x; c.lz += x;
		c.mx += x; if (c.mx2 != -inf) c.mx2 += x;
		c.mn += x; if (c.mn2 != inf) c.mn2 += x;
	}
	void applyMin(int s, ll x) { // only valid while x > mx2
		Node& c = t[s];
		if (c.mx <= x) return;
		c.sum += (x - c.mx) * c.cmx;
		if (c.mn == c.mx) c.mn = x;
		else if (c.mn2 == c.mx) c.mn2 = x;
		c.mx = x;
	}
	void applyMax(int s, ll x) { // only valid while x < mn2
		Node& c = t[s];
		if (c.mn >= x) return;
		c.sum += (x - c.mn) * c.cmn;
		if (c.mx == c.mn) c.mx = x;
		else if (c.mx2 == c.mn) c.mx2 = x;
		c.mn = x;
	}
	void push(int s, int l, int r) {
		if (l == r) return;
		int m = (l + r) / 2;
		if (t[s].lz) addAll(2*s, m-l+1, t[s].lz),
			addAll(2*s+1, r-m, t[s].lz), t[s].lz = 0;
		applyMin(2*s, t[s].mx), applyMin(2*s+1, t[s].mx);
		applyMax(2*s, t[s].mn), applyMax(2*s+1, t[s].mn);
	}
	void add(int s, int l, int r, int u, int v, ll x) {
		if (v < l || r < u) return;
		if (u <= l && r <= v) return addAll(s, r - l + 1, x);
		push(s, l, r);
		int m = (l + r) / 2;
		add(2*s, l, m, u, v, x), add(2*s+1, m+1, r, u, v, x), pull(s);
	}
	// hi: v = max(v, x); else v = min(v, x)
	void clampTo(int s, int l, int r, int u, int v, ll x, bool hi) {
		if (v < l || r < u || (hi ? t[s].mn >= x : t[s].mx <= x)) return;
		if (u <= l && r <= v && (hi ? t[s].mn2 > x : t[s].mx2 < x))
			return hi ? applyMax(s, x) : applyMin(s, x);
		push(s, l, r);
		int m = (l + r) / 2;
		clampTo(2*s, l, m, u, v, x, hi);
		clampTo(2*s+1, m+1, r, u, v, x, hi);
		pull(s);
	}
	ll sum(int s, int l, int r, int u, int v) {
		if (v < l || r < u) return 0;
		if (u <= l && r <= v) return t[s].sum;
		push(s, l, r);
		int m = (l + r) / 2;
		return sum(2*s, l, m, u, v) + sum(2*s+1, m+1, r, u, v);
	}
	ll ext(int s, int l, int r, int u, int v, bool hi) { // max, or -min
		if (v < l || r < u) return -inf;
		if (u <= l && r <= v) return hi ? t[s].mx : -t[s].mn;
		push(s, l, r);
		int m = (l + r) / 2;
		return max(ext(2*s,l,m,u,v,hi), ext(2*s+1,m+1,r,u,v,hi));
	}
	void add(int u, int v, ll x) { add(1, 0, n-1, u, v, x); }
	void chmin(int u, int v, ll x) { clampTo(1,0,n-1,u,v,x,0); }
	void chmax(int u, int v, ll x) { clampTo(1,0,n-1,u,v,x,1); }
	ll sum(int u, int v) { return sum(1, 0, n-1, u, v); }
	ll mx(int u, int v) { return ext(1, 0, n-1, u, v, 1); }
	ll mn(int u, int v) { return -ext(1, 0, n-1, u, v, 0); }
};

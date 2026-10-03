/**
 * Author: buidangnguyen05
 * Description: Pointwise minimum of lines $y = ax+b$ over $x \in [0,n)$.
 *  \texttt{addSeg} restricts a line to $[u,v]$. \texttt{Line::INF} where
 *  uncovered; for maximum insert $(-a,-b)$ and negate.
 * Time: \texttt{add}/\texttt{query} $O(\log n)$, \texttt{addSeg} $O(\log^2 n)$
 * Status: stress-tested
 */
#pragma once

struct Line {
	static constexpr ll INF = 1e18;
	ll a = 0, b = INF;
	ll operator()(ll x) const { return a * x + b; }
};

struct LiChao {
	int n; vector<Line> t;
	LiChao(int n) : n(n), t(4 * n) {}
	void add(int s, int l, int r, Line f) {
		int m = (l + r) / 2;
		bool lef = f(l) < t[s](l), mid = f(m) < t[s](m);
		if (mid) swap(t[s], f);
		if (l == r) return;
		if (lef != mid) add(2*s, l, m, f);
		else add(2*s + 1, m + 1, r, f);
	}
	void addSeg(int s, int l, int r, int u, int v, Line f) {
		if (v < l || r < u) return;
		if (u <= l && r <= v) return add(s, l, r, f);
		int m = (l + r) / 2;
		addSeg(2*s, l, m, u, v, f);
		addSeg(2*s + 1, m + 1, r, u, v, f);
	}
	ll query(int s, int l, int r, int x) {
		ll res = t[s](x);
		if (l == r) return res;
		int m = (l + r) / 2;
		return min(res, x <= m ? query(2*s, l, m, x)
		                       : query(2*s + 1, m + 1, r, x));
	}
	void add(Line f) { add(1, 0, n - 1, f); }
	void addSeg(Line f, int u, int v) { addSeg(1, 0, n-1, u, v, f); }
	ll query(int x) { return query(1, 0, n - 1, x); }
};

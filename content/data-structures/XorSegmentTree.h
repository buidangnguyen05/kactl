/**
 * Author: awk, PurpleCrayon
 * Source: https://codeforces.com/blog/entry/105723
 * Description: \texttt{get(u,v,x)} merges $a[u \oplus x] \dots
 *  a[v \oplus x]$ in index order, for any mask $x$. Merge must be
 *  associative, need not commute. \texttt{update} is a point add,
 *  additive merges only. $n$ must be a power of two.
 * Time: O(\log n) per query, $O(n \log n)$ memory
 * Status: stress-tested, commutative and not
 */
#pragma once

template<class T> struct XorSegtree { // T = int: sum; string: concat
	int n, K; vector<vector<T>> st; vector<T> lz;
	XorSegtree(const vector<T>& a) : n(sz(a)), K(__lg(n)), st(4*n),
		lz(4*n) { build(1, 0, n - 1, a); }
	void build(int s, int l, int r, const vector<T>& a) {
		if (l == r) { st[s] = {a[l]}; return; }
		int m = (l + r) / 2, h = (r - l + 1) / 2;
		build(2*s, l, m, a), build(2*s+1, m+1, r, a);
		st[s].resize(2 * h);
		rep(i,0,2*h) { // mask bit set => the two halves swap
			int j = i & (h - 1);
			st[s][i] = i < h ? st[2*s][j] + st[2*s+1][j]
			                 : st[2*s+1][j] + st[2*s][j];
		}
	}
	void update(int s, int l, int r, int p, T v) { // a[p] += v
		lz[s] = lz[s] + v;
		if (l == r) return;
		int m = (l + r) / 2;
		if (p <= m) update(2*s, l, m, p, v);
		else update(2*s+1, m+1, r, p, v);
	}
	T get(int s, int l, int r, int u, int v, int x, int d) {
		if (l > v || r < u) return T();
		if (u <= l && r <= v) return lz[s] + st[s][x & ((1 << d) - 1)];
		int m = (l + r) / 2;
		if (x >> (d - 1) & 1) {
			int o = m + 1 - l;
			return get(2*s+1, m+1, r, u+o, v+o, x, d-1)
			     + get(2*s, l, m, u-o, v-o, x, d-1);
		}
		return get(2*s, l, m, u, v, x, d-1)
		     + get(2*s+1, m+1, r, u, v, x, d-1);
	}
	void update(int p, T v) { update(1, 0, n-1, p, v); }
	T get(int u, int v, int x) { return get(1, 0, n-1, u, v, x, K); }
};

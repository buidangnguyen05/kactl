/**
 * Author: buidangnguyen05
 * License: CC0
 * Description: Persistent segment tree over $[0, n)$; \texttt{upd}
 *  returns a new root, old roots stay valid. Node 0 is the empty tree.
 *  With \texttt{rt[i] = upd(rt[i-1], a[i], 1)}, \texttt{kth(rt[l],
 *  rt[r], k)} is the kth smallest of $a[l+1..r]$.
 * Time: $O(\log n)$ per operation, $O(n \log n)$ memory
 * Status: stress-tested
 */
#pragma once

struct PST {
	vi lc, rc; vector<ll> s; int n;
	PST(int n) : n(n) { node(); } // node 0 = empty tree
	int node(int l = 0, int r = 0, ll v = 0) {
		lc.push_back(l), rc.push_back(r), s.push_back(v);
		return sz(s) - 1;
	}
	int upd(int p, int i, ll v, int l, int r) { // a[i] += v
		if (r - l == 1) return node(0, 0, s[p] + v);
		int m = (l + r) / 2, a, b;
		if (i < m) a = upd(lc[p], i, v, l, m), b = rc[p];
		else a = lc[p], b = upd(rc[p], i, v, m, r);
		return node(a, b, s[a] + s[b]);
	}
	ll qry(int p, int ql, int qr, int l, int r) { // sum of [ql, qr)
		if (!p || qr <= l || r <= ql) return 0;
		if (ql <= l && r <= qr) return s[p];
		int m = (l + r) / 2;
		return qry(lc[p], ql, qr, l, m) + qry(rc[p], ql, qr, m, r);
	}
	int kth(int p, int q, ll k, int l, int r) { // 1-indexed k
		if (r - l == 1) return l;
		int m = (l + r) / 2;
		ll c = s[lc[q]] - s[lc[p]];
		return k <= c ? kth(lc[p], lc[q], k, l, m)
		              : kth(rc[p], rc[q], k - c, m, r);
	}
	int upd(int p, int i, ll v) { return upd(p, i, v, 0, n); }
	ll qry(int p, int ql, int qr) { return qry(p, ql, qr, 0, n); }
	int kth(int p, int q, ll k) { return kth(p, q, k, 0, n); }
};

/**
 * Author: caterpillow, frex-e
 * License: CC0
 * Source: https://github.com/frex-e/kactl
 * Description: Connectivity under edge insert/delete, offline. Divide
 *  and conquer over time with a rollback DSU. \texttt{toggle} flips an
 *  edge, \texttt{query} records the component count, \texttt{solve}
 *  returns answers in order. Each call uses one of the $q$ slots.
 * Time: O(q \log q \log n)
 * Status: stress-tested
 */
#pragma once

#include "UnionFindRollback.h"

struct DynCon {
	RollbackUF uf;
	vector<vector<pii>> seg;
	map<pii, int> alive; // edge -> time it was inserted
	int n, q, t = 0;
	DynCon(int n, int nq) : uf(n), n(n), q(1) {
		while (q < max(nq, 1)) q *= 2;
		seg.resize(2 * q);
	}
	void addSeg(int l, int r, pii e) { // e alive on [l, r)
		for (l += q, r += q; l < r; l /= 2, r /= 2) {
			if (l & 1) seg[l++].push_back(e);
			if (r & 1) seg[--r].push_back(e);
		}
	}
	void toggle(int u, int v) {
		if (u > v) swap(u, v);
		pii e{u, v};
		auto it = alive.find(e);
		if (it == alive.end()) alive[e] = t;
		else addSeg(it->second, t, e), alive.erase(it);
		t++;
	}
	void query() { seg[q + t++].push_back({-1, -1}); }
	void dfs(int i, vi& res) {
		int t0 = uf.time();
		for (auto [a, b] : seg[i]) if (a >= 0) uf.join(a, b);
		for (auto [a, b] : seg[i])
			if (a < 0) res.push_back(n - uf.time() / 2);
		if (i < q) dfs(2*i, res), dfs(2*i + 1, res);
		uf.rollback(t0);
	}
	vi solve() { // number of components at each query, in order
		for (auto [e, s] : alive) addSeg(s, t, e);
		alive.clear();
		vi res; dfs(1, res); return res;
	}
};

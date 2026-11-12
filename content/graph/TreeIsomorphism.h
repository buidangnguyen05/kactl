/**
 * Author: Bui Dang Nguyen
 * Date: 2026-11-11
 * License: CC0
 * Source: AHU canonical forms
 * Description: Canonical ids for trees, exact (no hashing): two rooted trees are
 *  isomorphic iff treeId gives equal ids, as long as they share the same ids map.
 *  Unrooted trees: compare unrootedId, which roots at the one or two centroids.
 *  Recursive; deep trees need a big stack.
 * Time: O(N \log N)
 * Status: stress-tested
 */
#pragma once

map<vi, int> ids;
int treeId(const vector<vi>& g, int u, int p = -1) {
	vi c;
	for (int v : g[u]) if (v != p) c.push_back(treeId(g, v, u));
	sort(all(c));
	return ids.emplace(c, sz(ids)).first->second;
}
pii unrootedId(const vector<vi>& g) {
	int n = sz(g); vi s(n), cen;
	auto dfs = [&](auto& f, int u, int p) -> void {
		int mx = 0; s[u] = 1;
		for (int v : g[u]) if (v != p)
			f(f, v, u), s[u] += s[v], mx = max(mx, s[v]);
		if (2 * max(mx, n - s[u]) <= n) cen.push_back(u);
	};
	dfs(dfs, 0, -1);
	int a = treeId(g, cen[0]), b = sz(cen) > 1 ? treeId(g, cen[1]) : a;
	return minmax(a, b);
}

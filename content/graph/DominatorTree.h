/**
 * Author: Benq, frex-e
 * License: CC0
 * Source: https://github.com/frex-e/kactl, orig. bqi343/USACO
 * Description: $a$ dominates $b$ iff every path $root \to b$ passes
 *  through $a$; immediate dominators form a tree, so deleting $a$
 *  disconnects exactly its subtree. All vertices must be reachable.
 *  \texttt{ans[u]} lists u's children.
 * Time: O(M \log N)
 * Status: stress-tested
 */
#pragma once

struct Dominator {
	int n, co = 0;
	vector<vi> adj, ans, radj, child, sdomChild;
	vi label, rlabel, sdom, dom, par, bes;
	int get(int x) { // DSU keeping the min-sdom vertex on the path
		if (par[x] != x) {
			int t = get(par[x]); par[x] = par[par[x]];
			if (sdom[t] < sdom[bes[x]]) bes[x] = t;
		}
		return bes[x];
	}
	void dfs(int x) { // DFS tree, relabelled 0..co-1 in preorder
		label[x] = co; rlabel[co] = x;
		sdom[co] = par[co] = bes[co] = co;
		++co;
		for (int y : adj[x]) {
			if (label[y] < 0)
				dfs(y), child[label[x]].push_back(label[y]);
			radj[label[y]].push_back(label[x]);
		}
	}
	Dominator(const vector<vi>& g, int root) : n(sz(g)), adj(g),
			ans(n), radj(n), child(n), sdomChild(n), label(n, -1),
			rlabel(n), sdom(n), dom(n), par(n), bes(n) {
		dfs(root);
		for (int i = co - 1; i >= 0; --i) {
			for (int j : radj[i]) sdom[i] = min(sdom[i], sdom[get(j)]);
			if (i) sdomChild[sdom[i]].push_back(i);
			for (int j : sdomChild[i]) {
				int k = get(j);
				dom[j] = sdom[j] == sdom[k] ? sdom[j] : k;
			}
			for (int j : child[i]) par[j] = i;
		}
		rep(i,1,co) {
			if (dom[i] != sdom[i]) dom[i] = dom[dom[i]];
			ans[rlabel[dom[i]]].push_back(rlabel[i]);
		}
	}
};

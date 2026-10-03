/**
 * Author: buidangnguyen05
 * Description: Edge-biconnected components: what is left once every
 *  bridge is removed. Each undirected edge needs its own id, shared by
 *  both directions. Contracting components gives the bridge tree.
 * Time: O(E + V)
 * Status: stress-tested
 */
#pragma once

vector<vector<pii>> adj; // adj[u] = list of {v, edge id}
vi num, comp, stk; vector<vi> comps; int T;

int dfsEbcc(int u, int pe) {
	int low = num[u] = ++T;
	stk.push_back(u);
	for (auto [v, e] : adj[u]) if (e != pe)
		low = min(low, num[v] ? num[v] : dfsEbcc(v, e));
	if (low == num[u]) { // the edge up from u is a bridge
		comps.emplace_back();
		for (int v = -1; v != u;) {
			comp[v = stk.back()] = sz(comps) - 1;
			comps.back().push_back(v), stk.pop_back();
		}
	}
	return low;
}
void ebcc(int n) {
	num.assign(n, 0), comp.assign(n, -1), comps.clear();
	stk.clear(), T = 0;
	rep(i,0,n) if (!num[i]) dfsEbcc(i, -1);
}

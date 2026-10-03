#include "../utilities/template.h"
#include "../../content/graph/DominatorTree.h"

// reachable set from root avoiding `ban` (ban = -1 to ban nothing)
vector<char> reach(const vector<vi>& g, int root, int ban) {
	int n = sz(g); vector<char> vis(n, 0);
	if (root == ban) return vis;
	vi st{root}; vis[root] = 1;
	while (sz(st)) {
		int u = st.back(); st.pop_back();
		for (int v : g[u]) if (v != ban && !vis[v]) vis[v]=1, st.push_back(v);
	}
	return vis;
}

int main() {
	srand(67);
	rep(it,0,3000) {
		int n = rand() % 7 + 1;
		vector<vi> g(n);
		rep(i,0,n) rep(j,0,n) if (i != j && rand() % 3 == 0)
			g[i].push_back(j);
		int root = rand() % n;
		auto base = reach(g, root, -1);
		// restrict to the reachable subgraph, as the algorithm requires
		vi id(n, -1), back_;
		rep(i,0,n) if (base[i]) id[i] = sz(back_), back_.push_back(i);
		int m = sz(back_);
		vector<vi> h(m);
		rep(i,0,n) if (base[i]) for (int j : g[i]) if (base[j])
			h[id[i]].push_back(id[j]);
		Dominator d(h, id[root]);
		// brute force: idom[v] is the deepest a != v with v unreachable
		// when a is removed; equivalently check the parent relation
		rep(u,0,m) for (int v : d.ans[u]) {
			assert(u != v);
			auto r = reach(h, id[root], u);
			assert(!r[v]); // u really dominates v
		}
		// every non-root vertex has exactly one parent
		vi cnt(m, 0);
		rep(u,0,m) for (int v : d.ans[u]) cnt[v]++;
		rep(v,0,m) assert(cnt[v] == (v != id[root]));
		// u is the *immediate* dominator: no other dominator is deeper
		rep(u,0,m) for (int v : d.ans[u]) rep(w,0,m)
			if (w != u && w != v && w != id[root]) {
				auto r = reach(h, id[root], w);
				if (!r[v]) { // w also dominates v, so w must dominate u
					auto r2 = reach(h, id[root], w);
					assert(!r2[u]);
				}
			}
	}
	cout<<"Tests passed!"<<endl;
}

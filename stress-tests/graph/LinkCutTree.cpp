#include "../utilities/template.h"

#include "../../content/graph/LinkCutTree.h"

int ra() {
	static unsigned x;
	x *= 1385629541;
	x += 1442695040;
	return (int)(x >> 1);
}

// Reference forest. Everything is O(n) per call, no quadratic walks.
struct Brute {
	int n;
	vector<vi> adj;
	vi rt, par, ord;
	vector<char> vis;
	vector<ll> val;
	Brute(int n_) : n(n_), adj(n_+1), rt(n_+1), par(n_+1), vis(n_+1,0),
	                val(n_+1,0) { rep(i,1,n+1) rt[i] = i; }

	void del(int a, int b) { adj[a].erase(find(all(adj[a]), b)); }
	void build(int u) { // BFS the component from its root, fills par and ord
		int r = rt[u]; ord.assign(1, r); par[r] = 0; vis[r] = 1;
		rep(i,0,sz(ord)) for (int y : adj[ord[i]]) if (!vis[y])
			vis[y] = 1, par[y] = ord[i], ord.push_back(y);
		for (int x : ord) vis[x] = 0;
	}
	vi subtree(int u) { // needs a prior build()
		vi st(1, u); vis[u] = 1;
		rep(i,0,sz(st)) for (int y : adj[st[i]])
			if (!vis[y] && par[y] == st[i]) vis[y] = 1, st.push_back(y);
		for (int x : st) vis[x] = 0;
		return st;
	}
	vi toroot(int u) { vi p; for (; u; u = par[u]) p.push_back(u); return p; }
	vi path(int u, int v) {
		build(u); vi a = toroot(u), b = toroot(v), res, tmp;
		set<int> s(all(a)); int l = 0;
		for (int x : b) if (s.count(x)) { l = x; break; }
		for (int x : a) { res.push_back(x); if (x == l) break; }
		for (int x : b) { if (x == l) break; tmp.push_back(x); }
		reverse(all(tmp)); for (int x : tmp) res.push_back(x);
		return res;
	}
	bool conn(int u, int v) { return rt[u] == rt[v]; }
	void mkroot(int u) { build(u); for (int x : ord) rt[x] = u; }
	void link(int u, int v) { int r = rt[u];
		adj[u].push_back(v); adj[v].push_back(u);
		build(u); for (int x : ord) rt[x] = r; }
	void cut(int u) { build(u); int p = par[u];
		del(u,p); del(p,u); rt[u] = u;
		build(u); for (int x : ord) rt[x] = u; }
	int parent(int u) { build(u); return par[u]; }
	int depth(int u) { build(u); return sz(toroot(u)); }
	int lca(int u, int v) { build(u);
		vi a = toroot(u); set<int> s(all(a));
		for (int x : toroot(v)) if (s.count(x)) return x;
		return 0; }
	int compsize(int u) { build(u); return sz(ord); }
	ll compsum(int u) { build(u); ll s = 0;
		for (int x : ord) s += val[x];
		return s; }
	int subsize(int u) { build(u); return sz(subtree(u)); }
	ll subsum(int u) { build(u); ll s = 0;
		for (int x : subtree(u)) s += val[x];
		return s; }
	ll query(int u, int v) { ll s = 0;
		for (int x : path(u,v)) s += val[x];
		return s; }
	void pathupd(int u, int v, ll x) { for (int y : path(u,v)) val[y] += x; }
	void subupd(int u, ll x) { build(u);
		for (int y : subtree(u)) val[y] += x; }
};

// Preconditions are enforced by the structure, not just documented.
void testGuards() {
	LCT t(5);
	assert(t.link(1,2) && t.link(2,3));
	assert(!t.link(1,3) && !t.link(3,1));   // would close a cycle
	assert(!t.link(1,1));
	assert(!t.cut(1));                      // already a root
	assert(t.cut(3) && !t.cut(3));
	assert(t.lca(1,3) == 0 && t.lca(4,5) == 0); // different trees
	t.upd(1,10); t.upd(2,20); t.upd(3,30);
	assert(t.comp_sum(1) == 30 && t.comp_sum(3) == 30); // rejects were no-ops
	assert(t.link(3,1) && t.comp_sum(2) == 60 && t.lca(1,3) == 3);
}

// One random op against the reference. seedPath makes trees deep.
void testAgainstBrute(int n, int ops, int iters, bool seedPath) {
	rep(it,0,iters) {
		Brute B(n); LCT L(n);
		if (seedPath) rep(i,2,n/2+1) { B.link(i-1,i); assert(L.link(i-1,i)); }
		rep(q,0,ops) {
			// weighted: subtree updates and structural churn are the
			// combination that exercises the pending-add bookkeeping
			static const int W[] = {0,0,1,1,2,2,2,3,4,4,5,5,5,5,
			                        6,7,8,9,10,10,11,12,12,13,13};
			int u = ra() % n + 1, v = ra() % n + 1;
			int op = W[ra() % (int)(sizeof(W)/sizeof(*W))];
			ll x = ra() % 21 - 10;
			switch (op) {
			case 0: if (u != v && !B.conn(u,v)) {
			          B.link(u,v); assert(L.link(u,v)); }
			        else assert(!L.link(u,v));
			        break;
			case 1: if (B.parent(u)) { B.cut(u); assert(L.cut(u)); }
			        else assert(!L.cut(u));
			        break;
			case 2: B.mkroot(u); L.make_root(u); break;
			case 3: B.val[u] += x; L.upd(u,x); break;
			case 4: if (B.conn(u,v)) { B.pathupd(u,v,x); L.path_upd(u,v,x); }
			        break;
			case 5: B.subupd(u,x); L.sub_upd(u,x); break;
			case 6: assert(B.rt[u] == L.find_root(u)); break;
			case 7: assert(B.parent(u) == L.parent(u)); break;
			case 8: assert(B.depth(u) == L.depth(u)); break;
			case 9: assert(B.compsize(u) == L.comp_size(u)); break;
			case 10: assert(B.compsum(u) == L.comp_sum(u)); break;
			case 11: assert(B.subsize(u) == L.sub_size(u)); break;
			case 12: assert(B.subsum(u) == L.sub_sum(u)); break;
			default: assert(B.conn(u,v) == L.connected(u,v));
			         if (B.conn(u,v)) {
			           assert(B.query(u,v) == L.query(u,v));
			           assert(B.lca(u,v) == L.lca(u,v)); }
			}
		}
	}
}

int main() {
	testGuards();
	testAgainstBrute(3, 500, 4000, false);    // degenerate
	testAgainstBrute(12, 1500, 3000, false);  // dense churn
	testAgainstBrute(60, 4000, 300, true);    // deep paths
	testAgainstBrute(400, 6000, 30, true);    // medium
	testAgainstBrute(4000, 6000, 3, true);    // large
	cout<<"Tests passed!"<<endl;
}

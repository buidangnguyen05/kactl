#include "../utilities/template.h"
#include "../../content/graph/TwoConnectedComponents.h"

int find(vi& p, int x) { return p[x]==x ? x : p[x]=find(p,p[x]); }

int main() {
	srand(23);
	rep(it,0,3000) {
		int n = rand() % 8 + 1, m = rand() % 12;
		vector<pii> ed;
		rep(i,0,m) {
			int a = rand()%n, b = rand()%n;
			if (a != b) ed.push_back({a,b});
		}
		m = sz(ed);
		adj.assign(n, {});
		rep(i,0,m) {
			adj[ed[i].first].push_back({ed[i].second, i});
			adj[ed[i].second].push_back({ed[i].first, i});
		}
		ebcc(n);
		// brute force: an edge is a bridge iff deleting it splits its
		// endpoints; 2ecc = components after deleting every bridge
		vi p(n); rep(i,0,n) p[i]=i;
		rep(i,0,m) {
			vi q(n); rep(j,0,n) q[j]=j;
			rep(j,0,m) if (j != i)
				q[find(q,ed[j].first)] = find(q,ed[j].second);
			bool bridge = find(q,ed[i].first) != find(q,ed[i].second);
			if (!bridge) p[find(p,ed[i].first)]=find(p,ed[i].second);
		}
		rep(a,0,n) rep(b,0,n)
			assert((comp[a]==comp[b]) == (find(p,a)==find(p,b)));
		int tot = 0;
		for (auto& c : comps) tot += sz(c);
		assert(tot == n);
	}
	cout<<"Tests passed!"<<endl;
}

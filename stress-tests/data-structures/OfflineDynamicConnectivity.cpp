#include "../utilities/template.h"
#include "../../content/data-structures/UnionFindRollback.h"
#include "../../content/data-structures/OfflineDynamicConnectivity.h"

int main() {
	srand(59);
	rep(it,0,2000) {
		int n = rand() % 7 + 1, q = rand() % 20 + 1;
		DynCon dc(n, q);
		set<pii> cur;
		vi want;
		rep(k,0,q) {
			if (rand() % 3 == 0) {
				dc.query();
				RollbackUF uf(n);
				for (auto [a,b] : cur) uf.join(a,b);
				want.push_back(n - uf.time() / 2);
			} else {
				int u = rand()%n, v = rand()%n;
				if (u == v) { dc.query();
					RollbackUF uf(n);
					for (auto [a,b] : cur) uf.join(a,b);
					want.push_back(n - uf.time() / 2);
					continue; }
				if (u > v) swap(u, v);
				dc.toggle(u, v);
				if (cur.count({u,v})) cur.erase({u,v});
				else cur.insert({u,v});
			}
		}
		assert(dc.solve() == want);
	}
	cout<<"Tests passed!"<<endl;
}

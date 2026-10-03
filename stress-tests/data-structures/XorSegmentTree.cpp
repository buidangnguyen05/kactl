#include "../utilities/template.h"
#include "../../content/data-structures/XorSegmentTree.h"

int main() {
	srand(71);
	rep(it,0,400) { // commutative: sums, with point updates
		int K = rand() % 5, n = 1 << K;
		vector<int> a(n); rep(i,0,n) a[i] = rand() % 100;
		XorSegtree<int> t(a);
		rep(q,0,25) {
			if (rand() % 3 == 0) {
				int p = rand()%n, v = rand()%20 - 10;
				t.update(p, v); a[p] += v;
			}
			int u = rand()%n, v = rand()%n, x = rand()%n;
			if (u > v) swap(u, v);
			int want = 0;
			rep(i,u,v+1) want += a[i ^ x];
			assert(t.get(u, v, x) == want);
		}
	}
	rep(it,0,200) { // NON-commutative: string concat, order matters
		int K = rand() % 4 + 1, n = 1 << K;
		vector<string> a(n);
		rep(i,0,n) a[i] = string(1, char('a' + rand() % 26));
		XorSegtree<string> t(a);
		rep(u,0,n) rep(v,u,n) rep(x,0,n) {
			string want;
			rep(i,u,v+1) want += a[i ^ x];
			assert(t.get(u, v, x) == want);
		}
	}
	cout<<"Tests passed!"<<endl;
}

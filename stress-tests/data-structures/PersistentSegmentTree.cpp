#include "../utilities/template.h"
#include "../../content/data-structures/PersistentSegmentTree.h"

int main() {
	srand(53);
	rep(it,0,500) {
		int n = rand() % 12 + 1;
		vi a(n); rep(i,0,n) a[i] = rand() % n;
		PST t(n);
		vi rt(n + 1, 0);
		rep(i,0,n) rt[i+1] = t.upd(rt[i], a[i], 1);
		// old versions stay valid
		rep(v,0,n+1) rep(l,0,n+1) rep(r,l,n+1) {
			ll want = 0;
			rep(i,0,v) want += (l <= a[i] && a[i] < r);
			assert(t.qry(rt[v], l, r) == want);
		}
		// kth smallest of a[l..r)
		rep(l,0,n) rep(r,l+1,n+1) {
			vi b(a.begin()+l, a.begin()+r);
			sort(all(b));
			rep(k,1,sz(b)+1) assert(t.kth(rt[l], rt[r], k) == b[k-1]);
		}
	}
	cout<<"Tests passed!"<<endl;
}

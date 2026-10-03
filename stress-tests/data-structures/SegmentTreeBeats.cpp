#include "../utilities/template.h"
#include "../../content/data-structures/SegmentTreeBeats.h"

int main() {
	srand(5);
	rep(it,0,2000) {
		int n = rand() % 12 + 1;
		vector<ll> a(n);
		rep(i,0,n) a[i] = rand() % 21 - 10;
		Beats b(a);
		rep(q,0,60) {
			int u = rand() % n, v = rand() % n;
			if (u > v) swap(u, v);
			ll x = rand() % 31 - 15;
			int op = rand() % 6;
			if (op == 0) {
				b.add(u, v, x);
				rep(i,u,v+1) a[i] += x;
			} else if (op == 1) {
				b.chmin(u, v, x);
				rep(i,u,v+1) a[i] = min(a[i], x);
			} else if (op == 2) {
				b.chmax(u, v, x);
				rep(i,u,v+1) a[i] = max(a[i], x);
			} else {
				ll s = 0, mx = -Beats::inf, mn = Beats::inf;
				rep(i,u,v+1) s += a[i], mx = max(mx,a[i]),
					mn = min(mn,a[i]);
				assert(b.sum(u,v) == s);
				assert(b.mx(u,v) == mx);
				assert(b.mn(u,v) == mn);
			}
		}
		rep(i,0,n) assert(b.sum(i,i) == a[i]);
	}
	cout<<"Tests passed!"<<endl;
}

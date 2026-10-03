#include "../utilities/template.h"
#include "../../content/data-structures/WaveletTree.h"

int main() {
	srand(13);
	rep(it,0,400) {
		int n = rand() % 12 + 1;
		int lo = rand() % 21 - 15, hi = lo + rand() % 12;
		vi a(n);
		rep(i,0,n) a[i] = lo + rand() % (hi - lo + 1);
		vi orig = a;
		WaveletTree w;
		w.build(1, all(a), lo, hi);
		rep(i,1,n+1) rep(j,i,n+1) {
			vi b(orig.begin() + i - 1, orig.begin() + j);
			sort(all(b));
			rep(k,lo-2,hi+3) {
				ll want = 0;
				for (int x : b) want += (x <= k);
				assert(w.rank(k, i, j) == want);
			}
			ll run = 0;
			rep(k,0,sz(b)+1) {
				assert(w.sumSmallest(k, i, j) == run);
				if (k < sz(b)) run += b[k];
			}
			// kth smallest via difference
			rep(k,1,sz(b)+1)
				assert(w.sumSmallest(k,i,j) - w.sumSmallest(k-1,i,j)
					== b[k-1]);
		}
	}
	cout<<"Tests passed!"<<endl;
}

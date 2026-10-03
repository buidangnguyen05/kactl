#include "../utilities/template.h"
#include "../../content/data-structures/2DSparseTable.h"

int main() {
	srand(2);
	rep(it,0,300) {
		int n = rand() % 9 + 1, m = rand() % 9 + 1;
		vector<vi> a(n, vi(m));
		rep(i,0,n) rep(j,0,m) a[i][j] = rand() % 50;
		ST2D st(a);
		rep(i1,0,n) rep(i2,i1,n) rep(j1,0,m) rep(j2,j1,m) {
			int want = INT_MIN;
			rep(i,i1,i2+1) rep(j,j1,j2+1) want = max(want, a[i][j]);
			assert(st.query(i1,j1,i2,j2) == want);
		}
	}
	cout<<"Tests passed!"<<endl;
}

/**
 * Author: Epiphyllum
 * Description: Range max over a rectangle of a static grid, in O(1).
 *  Inclusive corners. Memory $nm \log n \log m$ ints.
 * Time: build $O(nm \log n \log m)$, query $O(1)$
 * Status: stress-tested
 */
#pragma once

struct ST2D {
	vector<vector<vector<vi>>> t;
	ST2D(const vector<vi>& a) {
		int n = sz(a), m = sz(a[0]);
		int kn = __lg(n) + 1, km = __lg(m) + 1;
		t.assign(kn, vector<vector<vi>>(km, vector<vi>(n, vi(m))));
		t[0][0] = a;
		rep(y,1,km) rep(i,0,n) rep(j,0,m - (1 << y) + 1)
			t[0][y][i][j] = max(t[0][y-1][i][j],
				t[0][y-1][i][j + (1 << (y-1))]);
		rep(x,1,kn) rep(y,0,km) rep(i,0,n - (1 << x) + 1) rep(j,0,m)
			t[x][y][i][j] = max(t[x-1][y][i][j],
				t[x-1][y][i + (1 << (x-1))][j]);
	}
	int query(int i1, int j1, int i2, int j2) {
		int x = __lg(i2 - i1 + 1), y = __lg(j2 - j1 + 1);
		int p = i2 - (1 << x) + 1, q = j2 - (1 << y) + 1;
		return max({t[x][y][i1][j1], t[x][y][p][j1],
			t[x][y][i1][q], t[x][y][p][q]});
	}
};

/**
 * Author: Epiphyllum
 * Description: Self-explanatory.
 * Time: O(n \cdot \log_2^2 n)
 */

#pragma once

void st_prepare() {
	repn(i, 1, n) repn(j, 1, m) st[0][0][i][j] = f[i][j];
	for (int i = 1; (1 << i) <= n; i++)
		repn(x, 1 << i, n) repn(y, 1, m)
			st[i][0][x][y] = max(st[i - 1][0][x][y], st[i - 1][0][x - (1 << (i - 1))][y]);
	for (int i = 0; (1 << i) <= n; i++)
		for(int j =1; (1 << j) <= m; j++)
			repn(x, 1 << i, n) repn(y, 1 << j, m) 
				st[i][j][x][y] = max(st[i][j - 1][x][y], st[i][j - 1][x][y - (1 << (j - 1))]);
}
int query_max(int a, int b, int c, int d) {
	int k1 = 31 - __builtin_clz(c - a + 1);
	int k2 = 31 - __builtin_clz(d - b + 1);
	return max(max(st[k1][k2][c][d], st[k1][k2][a + (1 << k1) - 1][d]),
		max(st[k1][k2][c][b + (1 << k2) - 1],st[k1][k2][a + (1 << k1) - 1][b + (1 << k2) - 1]));
}
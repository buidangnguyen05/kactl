/**
 * Author: buidangnguyen05
 * Description: Divide and conquer over a tree. Every vertex lies in
 *  $O(\log n)$ components, so linear work per component totals $n\log n$.
 * Time: O(n \log n) times the cost of the work at each centroid
 * Status: stress-tested
 */
#pragma once

vector<vi> adj;
vi sub; vector<bool> used;

int calc(int x, int p) {
	sub[x] = 1;
	for (int i : adj[x])
		if (i != p && !used[i]) sub[x] += calc(i, x);
	return sub[x];
}
int find(int x, int p, int n) {
	for (int i : adj[x])
		if (i != p && !used[i] && sub[i] * 2 > n)
			return find(i, x, n);
	return x;
}
void centroid(int x) {
	int c = find(x, -1, calc(x, -1));
	used[c] = 1;
	// ... solve for paths through c here, using calc(c, -1) sizes ...
	for (int i : adj[c]) if (!used[i]) centroid(i);
}

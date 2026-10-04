#include "../utilities/template.h"

#include "../../content/graph/CentroidDecomposition.h"

// Vertices reachable from x without passing through blocked ones.
vi comp(int x, const vector<bool>& blocked) {
	vi out{x}, seen(sz(adj));
	seen[x] = 1;
	rep(k,0,sz(out)) for (int y : adj[out[k]])
		if (!blocked[y] && !seen[y]) seen[y] = 1, out.push_back(y);
	return out;
}

void randomTree(int n) {
	adj.assign(n, {}), sub.assign(n, 0), used.assign(n, false);
	rep(i,1,n) {
		int p = rand() % 3 ? rand() % i : i - 1; // mix of bushy and path-like
		adj[p].push_back(i), adj[i].push_back(p);
	}
}

int main() {
	srand(3);
	rep(it,0,4000) {
		int n = rand() % 60 + 1;
		randomTree(n);
		rep(i,0,n) used[i] = rand() % 4 == 0; // a forest of leftover components
		rep(x,0,n) if (!used[x]) {
			vi C = comp(x, used);
			int m = calc(x, -1);
			assert(m == sz(C));
			int c = find(x, -1, m);
			assert(!used[c] && count(all(C), c));
			vector<bool> blk = used;
			blk[c] = true;
			for (int y : adj[c]) if (!blk[y]) assert(2 * sz(comp(y, blk)) <= m);
		}
		fill(all(used), false);
		centroid(rand() % n);
		assert(count(all(used), true) == n); // every vertex is a centroid once
	}
	randomTree(100000);
	adj.assign(100000, {});
	rep(i,1,100000) adj[i-1].push_back(i), adj[i].push_back(i-1); // a long path
	centroid(0);
	assert(count(all(used), true) == 100000);
	cout << "Tests passed!" << endl;
}

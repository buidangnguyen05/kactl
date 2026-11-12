#include "../utilities/template.h"

#include "../../content/graph/TreeIsomorphism.h"

typedef vector<pii> Edges;
vector<vi> adj(int n, const Edges& e) {
	vector<vi> g(n);
	for (auto [a, b] : e) g[a].push_back(b), g[b].push_back(a);
	return g;
}
// brute force: try all relabelings
bool iso(int n, const Edges& e1, const Edges& e2, int r1 = -1, int r2 = -1) {
	set<pii> s2;
	for (auto [a, b] : e2) s2.insert(minmax(a, b));
	vi p(n); iota(all(p), 0);
	do {
		if (r1 != -1 && p[r1] != r2) continue;
		bool ok = true;
		for (auto [a, b] : e1) if (!s2.count(minmax(p[a], p[b]))) { ok = false; break; }
		if (ok) return true;
	} while (next_permutation(all(p)));
	return false;
}

int main() {
	mt19937 rng(6);
	int same = 0, diff = 0;
	auto randTree = [&](int n) {
		Edges e;
		rep(i,1,n) e.push_back({(int)(rng() % i), i});
		vi p(n); iota(all(p), 0); shuffle(all(p), rng);
		for (auto& [a, b] : e) a = p[a], b = p[b];
		return e;
	};
	rep(it,0,30000) {
		int n = 1 + rng() % 7;
		Edges e1 = randTree(n), e2 = randTree(n);
		if (rng() % 2) { // isomorphic copy
			vi p(n); iota(all(p), 0); shuffle(all(p), rng);
			e2 = e1;
			for (auto& [a, b] : e2) a = p[a], b = p[b];
		}
		auto g1 = adj(n, e1), g2 = adj(n, e2);
		bool want = iso(n, e1, e2);
		assert((unrootedId(g1) == unrootedId(g2)) == want);
		(want ? same : diff)++;
		// rooted
		int r1 = rng() % n, r2 = rng() % n;
		assert((treeId(g1, r1) == treeId(g2, r2)) == iso(n, e1, e2, r1, r2));
	}
	// large path (recursion depth) and star
	int n = 50000;
	Edges path, path2;
	rep(i,1,n) path.push_back({i-1, i});
	rep(i,1,n) path2.push_back({n-i, n-i-1});
	assert(unrootedId(adj(n, path)) == unrootedId(adj(n, path2)));
	cout << "Tests passed! (" << same << " isomorphic, " << diff << " not)" << endl;
}

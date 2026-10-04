#include "../utilities/template.h"

#include "../../content/various/MoOnTree.h"

// The path is kept in a deque: end 0 adds/removes at the front (u's side),
// end 1 at the back (v's side), and del asserts that the removed vertex is at
// that end. With snap set, calc() saves the path so it can be compared
// against the true u..v path in order.
vi vals;
int sum, ops;
deque<int> path;
bool snap;
vector<vi> snaps;
void add(int i, int end) {
	sum += vals[i], ops++;
	if (end == 0) path.push_front(i);
	else path.push_back(i);
}
void del(int i, int end) {
	sum -= vals[i], ops++;
	assert(!path.empty());
	if (end == 0) assert(path.front() == i), path.pop_front();
	else assert(path.back() == i), path.pop_back();
}
int calc() {
	if (!snap) return sum;
	snaps.push_back(vi(all(path)));
	return sz(snaps) - 1;
}

void testTr(int n, int q, bool checkOrder) {
	vector<array<int, 2>> queries(q);
	for (auto& pa : queries) pa[0] = rand() % n, pa[1] = rand() % n;
	vi par(n), val(n);
	rep(i,1,n) par[i] = rand() % i;
	rep(i,0,n) val[i] = rand() % 1000;
	vector<vi> ed(n);
	rep(i,1,n) ed[par[i]].push_back(i), ed[i].push_back(par[i]);
	vals = val, sum = ops = 0, path.clear();
	snap = checkOrder, snaps.clear();
	vi res = moTree(queries, ed);
	vi seen(n);
	rep(i,0,q) {
		// Tree depth is logarithmic, so walk to the LCA naively
		int u = queries[i][0], v = queries[i][1];
		for (int at = u; ; at = par[at]) { seen[at] = 1; if (!at) break; }
		vi up, down;
		int l = v;
		while (!seen[l]) down.push_back(l), l = par[l];
		for (int at = u; ; at = par[at]) { seen[at] = 0; if (!at) break; }
		for (int at = u; at != l; at = par[at]) up.push_back(at);
		up.push_back(l);
		up.insert(up.end(), down.rbegin(), down.rend()); // u .. lca .. v
		if (checkOrder) assert(snaps[res[i]] == up);
		else {
			int s = 0;
			for (int x : up) s += val[x];
			assert(res[i] == s);
		}
	}
}

int main() {
	srand(2);
	rep(it,0,10) rep(n,1,15) rep(q,0,n*n) testTr(n, q, true);
	rep(it,0,200) testTr(200, 300, true);
	testTr(100'000, 100'000, false);
	testTr(1000, 100'000, false);
	testTr(100'000, 1000, false);
	cout << "Tests passed!" << endl;
}

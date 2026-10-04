#include "../utilities/template.h"

#include "../../content/various/MoRollback.h"

// Count distinct and max frequency, add-only. The window [L, R] is tracked
// through lg too, so add() can check dir (1 = right end, 0 = left end).
vi a;
vector<ll> cnt;
ll d, mx, L, R;
vector<array<ll, 4>> snaps;
void reset() { fill(all(cnt), 0), d = mx = 0, L = 1, R = 0; }
void add(int i, int dir) {
	if (L > R) lg.set(L, i), lg.set(R, i);
	else if (dir) assert(i == R + 1), lg.set(R, i);
	else assert(i == L - 1), lg.set(L, i);
	lg.set(cnt[a[i]], cnt[a[i]] + 1);
	if (cnt[a[i]] == 1) lg.set(d, d + 1);
	if (cnt[a[i]] > mx) lg.set(mx, cnt[a[i]]);
}
ll get() {
	snaps.push_back({L, R, d, mx});
	return sz(snaps) - 1;
}

void test(int n, int nq, int maxv, int check) {
	a.resize(n);
	for (int& x : a) x = rand() % maxv;
	cnt.assign(maxv, 0), snaps.clear();
	vector<QR> q(nq);
	rep(i,0,nq) {
		int l = rand() % n, r = rand() % n;
		q[i] = {min(l, r), max(l, r), i};
	}
	vector<ll> res = moRB(q, n);
	assert(lg.log.empty() && !lg.on);
	rep(i,0,min(nq, check)) {
		auto [l, r, id] = q[i];
		map<int, int> f;
		rep(k,l,r+1) f[a[k]]++;
		ll m = 0;
		for (auto [v, c] : f) m = max(m, (ll)c);
		assert((snaps[res[id]] == array<ll, 4>{l, r, sz(f), m}));
	}
}

int main() {
	srand(6);
	rep(it,0,3000) test(rand() % 40 + 1, rand() % 60 + 1, rand() % 10 + 1, 1000);
	test(100000, 100000, 1000, 200);
	cout << "Tests passed!" << endl;
}

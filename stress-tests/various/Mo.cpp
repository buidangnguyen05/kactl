#include "../utilities/template.h"

#include "../../content/various/Mo.h"

// The window [L, R] is tracked explicitly: add/del must touch one of its ends.
vi a, cnt;
int L, R, distinct;
ll sum;
vector<array<ll, 4>> snaps;
void add(int i) {
	if (L > R) L = R = i;
	else if (i == R + 1) R++;
	else assert(i == L - 1), L--;
	distinct += !cnt[a[i]]++, sum += a[i];
}
void del(int i) {
	assert(L <= R);
	if (i == R) R--;
	else assert(i == L), L++;
	distinct -= !--cnt[a[i]], sum -= a[i];
}
ll get() {
	snaps.push_back({L, R, distinct, sum});
	return sz(snaps) - 1;
}

void test(int n, int nq, int maxv, int check) {
	a.resize(n);
	for (int& x : a) x = rand() % maxv;
	cnt.assign(maxv, 0), L = 0, R = -1, distinct = 0, sum = 0, snaps.clear();
	vector<Q> q(nq);
	rep(i,0,nq) {
		int l = rand() % n, r = rand() % n;
		q[i] = {min(l, r), max(l, r), i};
	}
	vector<ll> res = mo(q, n);
	rep(i,0,min(nq, check)) {
		auto [l, r, id] = q[i];
		set<int> s(a.begin() + l, a.begin() + r + 1);
		ll tot = accumulate(a.begin() + l, a.begin() + r + 1, 0LL);
		assert((snaps[res[id]] == array<ll, 4>{l, r, sz(s), tot}));
	}
}

int main() {
	srand(4);
	rep(it,0,3000) test(rand() % 40 + 1, rand() % 60 + 1, rand() % 10 + 1, 1000);
	test(100000, 100000, 1000, 200);
	cout << "Tests passed!" << endl;
}

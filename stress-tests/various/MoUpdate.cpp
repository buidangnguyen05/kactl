#include "../utilities/template.h"

#include "../../content/various/MoUpdate.h"

// in[] catches adding an index twice or removing one that isn't there; the
// index sum checks the window is exactly [l, r].
vi a, cnt, in;
ll distinct, sum, isum;
vector<array<ll, 3>> snaps;
void add(int i) {
	assert(!in[i]), in[i] = 1;
	distinct += !cnt[a[i]]++, sum += a[i], isum += i;
}
void del(int i) {
	assert(in[i]), in[i] = 0;
	distinct -= !--cnt[a[i]], sum -= a[i], isum -= i;
}
ll get() {
	snaps.push_back({distinct, sum, isum});
	return sz(snaps) - 1;
}

void test(int n, int events, int maxv, int check) {
	a.resize(n);
	for (int& x : a) x = rand() % maxv;
	vi cur = a; // brute force copy, updated in order
	cnt.assign(maxv, 0), in.assign(n, 0), distinct = sum = isum = 0;
	ops.clear(), snaps.clear();
	vector<Q3> qs;
	vector<array<ll, 3>> expect;
	rep(e,0,events) {
		if (rand() % 2) {
			int p = rand() % n, x = rand() % maxv;
			ops.push_back({p, x}), cur[p] = x;
		} else {
			int l = rand() % n, r = rand() % n;
			if (l > r) swap(l, r);
			qs.push_back({l, r, sz(ops), sz(qs)});
			if (sz(expect) < check) {
				set<int> s(cur.begin() + l, cur.begin() + r + 1);
				ll tot = accumulate(cur.begin() + l, cur.begin() + r + 1, 0LL);
				expect.push_back({sz(s), tot, (ll)(l + r) * (r - l + 1) / 2});
			}
		}
	}
	vector<ll> res = mo3D(qs, a);
	rep(i,0,sz(expect)) assert(snaps[res[i]] == expect[i]);
}

int main() {
	srand(5);
	rep(it,0,3000) test(rand() % 30 + 1, rand() % 80 + 1, rand() % 8 + 1, 1000);
	test(20000, 40000, 500, 300);
	cout << "Tests passed!" << endl;
}

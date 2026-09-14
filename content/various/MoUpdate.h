/**
 * Author: itsjerr
 * Description: Mo's algorithm, with point updates.
 * Usage: ops.push_back({p, x});
 * qs.push_back({l, r, sz(ops), i});
 * vector<ll> ans = mo3D(qs, a);
 * Time: O(n^{5/3})
 * Status: stress-tested against brute force
 */
#pragma once

void add(int i);    /// e.g. count distinct: if (!cnt[a[i]]++) d++;
void del(int i);	/// e.g. count distinct: if (!--cnt[a[i]]) d--;
ll get();			/// e.g. count distinct: return d;

struct Upd { int p, x; };        /// a[p] = x
vector<Upd> ops;
struct Q3 { int l, r, t, id; };
vector<ll> mo3D(vector<Q3> q, vi& a) {
	int n = sz(a), B = max(1, (int)cbrt((double)n * n) + 1);
	sort(all(q), [&](Q3 x, Q3 y) {
		if (x.l / B != y.l / B) return x.l < y.l;
		if (x.r / B != y.r / B) return (x.l / B) & 1 ? x.r > y.r : x.r < y.r;
		return (x.r / B) & 1 ? x.t > y.t : x.t < y.t; });
	vector<ll> ans(sz(q));
	int l = 0, r = -1, t = 0;
	auto doUpd = [&](int i) {
		Upd& o = ops[i];
		bool in = l <= o.p && o.p <= r;
		if (in) del(o.p);
		swap(a[o.p], o.x);
		if (in) add(o.p);
	};
	for (Q3& x : q) {
		while (t < x.t) doUpd(t++);
		while (t > x.t) doUpd(--t);
		while (r < x.r) add(++r);
		while (l > x.l) add(--l);
		while (r > x.r) del(r--);
		while (l < x.l) del(l++);
		ans[x.id] = get();
	}
	return ans;
}

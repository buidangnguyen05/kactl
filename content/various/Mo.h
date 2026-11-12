/**
 * Author: itsjerr
 * Description: Mo's algorithm for offline range queries. Works well when
 * range can easily be extended in constant time.
 * Time: O((n + q)\sqrt q)
 * Status: stress-tested against brute force
 */
#pragma once

void add(int i);   // e.g. count distinct: if (!cnt[a[i]]++) d++;
void del(int i);   // e.g. count distinct: if (!--cnt[a[i]]) d--;
ll get();          // e.g. count distinct: return d;

struct Q { int l, r, id; };
vector<ll> mo(vector<Q> q, int n) {
	int B = max(1, (int)(n / max(1.0, sqrt((double)sz(q)))));
	sort(all(q), [&](Q a, Q b) {
		if (a.l / B != b.l / B) return a.l < b.l;
		return (a.l / B) & 1 ? a.r > b.r : a.r < b.r; });
	vector<ll> ans(sz(q));
	int l = 0, r = -1;
	for (Q& x : q) {
		while (r < x.r) add(++r);
		while (l > x.l) add(--l);
		while (r > x.r) del(r--);
		while (l < x.l) del(l++);
		ans[x.id] = get();
	}
	return ans;
}

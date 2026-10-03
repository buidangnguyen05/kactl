/**
 * Author: buidangnguyen05
 * License: CC0
 * Description: Long division of two \texttt{Big}s, one base-$10^9$ limb
 *  at a time. Quotient truncates toward zero, remainder takes the sign
 *  of the dividend.
 * Time: $O(nm \log B)$
 * Status: stress-tested against \_\_int128
 */
#pragma once

#include "BigNum.h"

vi mulTo(const vi& x, int k) { // |x| * k, k < B
	vi r; ll c = 0;
	rep(i,0,sz(x))
		c += (ll)x[i] * k, r.push_back(int(c % Big::B)), c /= Big::B;
	for (; c; c /= Big::B) r.push_back(int(c % Big::B));
	while (sz(r) && !r.back()) r.pop_back();
	return r;
}
Big divmod(const Big& x, const Big& d, Big& r) { // r = remainder
	assert(sz(d.a)); // division by zero
	Big q; q.a.assign(sz(x.a), 0); r = Big();
	for (int i = sz(x.a); i--;) {
		r.a.insert(r.a.begin(), x.a[i]), r.trim();
		int lo = 0, hi = Big::B - 1;
		while (lo < hi) { // largest k with |d| * k <= |r|
			int m = (lo + hi + 1) / 2;
			if (Big::cmp(mulTo(d.a, m), r.a) <= 0) lo = m; else hi = m-1;
		}
		q.a[i] = lo, Big::addTo(r.a, mulTo(d.a, lo), -1);
	}
	q.sg = x.sg * d.sg, r.sg = x.sg;
	q.trim(), r.trim(); return q;
}
Big operator/(const Big& x, const Big& y) {
	Big r; return divmod(x, y, r); }
Big operator%(const Big& x, const Big& y) {
	Big r; divmod(x, y, r); return r; }

/**
 * Author: buidangnguyen05
 * License: CC0
 * Description: Division of a \texttt{Big} by a positive \texttt{int}.
 *  Quotient truncates toward zero, remainder takes the sign of the
 *  dividend.
 * Time: $O(n)$
 * Status: stress-tested against \_\_int128
 */
#pragma once

#include "BigNum.h"

Big divmod(const Big& x, int d, int& r) { // d > 0
	Big q; q.a.resize(sz(x.a)); r = 0;
	for (int i = sz(x.a); i--;) {
		ll cur = (ll)r * Big::B + x.a[i];
		q.a[i] = int(cur / d), r = int(cur % d);
	}
	q.sg = x.sg, r *= x.sg; q.trim(); return q;
}
Big operator/(const Big& x, int d) { int r; return divmod(x, d, r); }
int operator%(const Big& x, int d) { int r; divmod(x, d, r); return r; }

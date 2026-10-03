/**
 * Author: Jerry
 * Date: 2023-10-01
 * License: CC0
 * Description: Given $f(0), \dots, f(d)$ of a degree-$d$ polynomial,
 *  evaluates $f(x)$ mod a prime $> d$. Since $\sum_{i=1}^{x} i^k$ has
 *  degree $k+1$, pass its first $k+2$ prefix sums to sum $k$th powers.
 * Time: O(d \log \text{mod})
 * Status: stress-tested
 */
#pragma once

#include "../number-theory/ModPow.h"

ll lagrange(const vector<ll>& y, ll x) {
	int d = sz(y) - 1;
	if (x <= d) return y[(int)x];
	vector<ll> pre(d + 2, 1), suf(d + 2, 1), f(d + 1, 1);
	rep(i,0,d+1) pre[i+1] = pre[i] * ((x - i) % mod) % mod;
	for (int i = d; i >= 0; i--)
		suf[i] = suf[i+1] * ((x - i) % mod) % mod;
	rep(i,1,d+1) f[i] = f[i-1] * i % mod;
	ll ans = 0;
	rep(i,0,d+1) {
		ll t = y[i] * (pre[i] * suf[i+1] % mod) % mod
			* modpow(f[i] * f[d-i] % mod, mod - 2) % mod;
		ans = ((d - i) & 1 ? ans - t : ans + t) % mod;
	}
	return (ans + mod) % mod;
}

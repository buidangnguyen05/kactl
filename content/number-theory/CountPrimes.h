/**
 * Author: Lucy_Hedgehog
 * Source: https://projecteuler.net/thread=10;page=5
 * Description: Counts primes $\le n$. Replacing the $-1$ init and the
 *  \texttt{sp} subtraction by an additive $f$ gives $\sum_{p\le n} f(p)$.
 * Time: O(n^{3/4}), memory $O(\sqrt n)$
 * Status: stress-tested against a sieve up to $10^6$, spot-checked to $10^{11}$
 */
#pragma once

ll countPrimes(ll n) {
	if (n < 2) return 0;
	ll v = (ll)sqrtl(n);
	while (v * v > n) v--;
	while ((v+1) * (v+1) <= n) v++;
	// s[i] = pi(i), l[i] = pi(n / i), both still counting composites
	vector<ll> s(v + 1), l(v + 1);
	rep(i,1,v+1) s[i] = i - 1, l[i] = n / i - 1;
	rep(p,2,v+1) {
		if (s[p] == s[p-1]) continue; // p is composite
		ll sp = s[p-1], q = (ll)p * p;
		rep(i,1,(int)min((ll)v, n / q) + 1) {
			ll d = (ll)i * p;
			l[i] -= (d <= v ? l[d] : s[n / d]) - sp;
		}
		for (ll i = v; i >= q; i--) s[i] -= s[i / p] - sp;
	}
	return l[1];
}

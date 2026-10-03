/**
 * Author: Bui Dang Nguyen
 * Date: 2026-10-04
 * License: CC0
 * Source: folklore (guessing holonomic sequences by linear algebra)
 * Description: Guesses a P-recursive relation $\sum_{j=0}^{k} P_j(i)\,a_{i-j} = 0$
 * ($k \le i < n$, $\deg P_j \le d$) from the first $n$ terms of a sequence mod a prime.
 * findPRec gives a nullspace vector of the $(n-k) \times (k+1)(d+1)$ system as
 * $P[j][e]$ = coefficient of $i^e$ in $P_j$, or empty if none. guessPRec tries all
 * $(k,d)$ by increasing $m = (k+1)(d+1)$ on all but the last 5 terms, and keeps the
 * first relation that predicts those 5, so it needs about $m+k+5$ terms.
 * nextTerm gives $a_i$ from $a_{i-k..i-1}$, or $-1$ if $P_0(i) = 0$. Values in $[0, mod)$.
 * Usage: auto P = guessPRec(a); // check sz(P) > 0
 * while (sz(a) < N) a.push_back(nextTerm(P, a, sz(a)));
 * Time: O(n m^2) per $(k,d)$ tried, nextTerm O(m + \log mod).
 * Status: stress-tested
 */
#pragma once

#include "../number-theory/ModPow.h"

typedef vector<vector<ll>> Rel;
Rel findPRec(const vector<ll>& a, int k, int d) {
	int n = sz(a) - k, m = (k+1) * (d+1), r = 0;
	vector<vector<ll>> A(n, vector<ll>(m));
	rep(i,0,n) rep(j,0,k+1) {
		ll p = a[i+k-j];
		rep(e,0,d+1) A[i][j*(d+1)+e] = p, p = p * (i+k) % mod;
	}
	vi piv;
	rep(c,0,m) {
		int s = r;
		while (s < n && !A[s][c]) s++;
		if (s == n) { // free column: read off a nullspace vector
			Rel P(k+1, vector<ll>(d+1));
			P[c/(d+1)][c%(d+1)] = 1;
			rep(t,0,r) P[piv[t]/(d+1)][piv[t]%(d+1)] = (mod - A[t][c]) % mod;
			return P;
		}
		swap(A[r], A[s]);
		ll v = modpow(A[r][c], mod-2);
		for (ll& x : A[r]) x = x * v % mod;
		rep(t,0,n) if (t != r && A[t][c]) {
			ll f = A[t][c];
			rep(u,c,m) A[t][u] = (A[t][u] - f * A[r][u] % mod + mod) % mod;
		}
		piv.push_back(c), r++;
	}
	return {};
}

ll nextTerm(const Rel& P, const vector<ll>& a, int i) {
	ll s = 0, q = 0;
	rep(j,0,sz(P)) {
		ll v = 0;
		for (int e = sz(P[j]); e--;) v = (v * i + P[j][e]) % mod;
		if (j) s = (s + v * a[i-j]) % mod;
		else q = v;
	}
	return q ? (mod - s) * modpow(q, mod-2) % mod : -1;
}

Rel guessPRec(const vector<ll>& a) {
	int n = sz(a) - 5; // last 5 terms are held out for checking
	vector<ll> b(a.begin(), a.begin() + max(n, 0));
	rep(m,1,n) rep(k,0,m) if (m % (k+1) == 0 && n-k >= m) {
		Rel P = findPRec(b, k, m/(k+1) - 1);
		bool ok = !P.empty();
		rep(i,n,sz(a)) ok &= nextTerm(P, a, i) == a[i];
		if (ok) return P;
	}
	return {};
}

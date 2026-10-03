/**
 * Author: Bui Dang Nguyen
 * Date: 2026-10-04
 * License: CC0
 * Source: folklore (guessing holonomic sequences by linear algebra)
 * Description: Guesses a P-recursive relation $\sum_{j=0}^{k} P_j(i)\,a_{i-j} = 0$
 * ($k \le i < n$, $\deg P_j \le d$) from the first $n$ terms of a sequence mod a prime.
 * findPRec returns $P[j][e]$ = coefficient of $i^e$ in $P_j$, or empty if none.
 * guessPRec tries every $d \le D$ and any order $k$, by increasing $m = (k+1)(d+1)$,
 * fitting all but the last 5 terms and keeping the first relation that predicts
 * those 5; it needs about $m+k+5$ terms. A low $D$ may force a longer relation.
 * nextTerm gives $a_i$ from $a_{i-k..i-1}$, or $-1$ if $P_0(i) = 0$. Values in $[0, mod)$.
 * Usage: Mat P = guessPRec(a, 3); // check sz(P) > 0
 * while (sz(a) < N) a.push_back(nextTerm(P, a, sz(a)));
 * Time: O(n m^2) per $(k,d)$ tried, nextTerm O(m + \log mod).
 * Status: stress-tested
 */
#pragma once

#include "SolveLinearMod.h"

Mat findPRec(const vector<ll>& a, int k, int d) {
	int n = sz(a) - k, w = d + 1;
	Mat A(n, vector<ll>((k+1) * w));
	rep(i,0,n) rep(j,0,k+1) {
		ll p = a[i+k-j];
		rep(e,0,w) A[i][j*w+e] = p, p = p * (i+k) % mod;
	}
	Mat N = nullspace(A), P(k+1);
	if (N.empty()) return {};
	rep(j,0,k+1) P[j].assign(N[0].begin() + j*w, N[0].begin() + (j+1)*w);
	return P;
}

ll nextTerm(const Mat& P, const vector<ll>& a, int i) {
	ll s = 0, q = 0;
	rep(j,0,sz(P)) {
		ll v = 0;
		for (int e = sz(P[j]); e--;) v = (v * i + P[j][e]) % mod;
		if (j) s = (s + v * a[i-j]) % mod;
		else q = v;
	}
	return q ? (mod - s) * modpow(q, mod-2) % mod : -1;
}

Mat guessPRec(const vector<ll>& a, int D) {
	int n = sz(a) - 5; // last 5 terms are held out for checking
	vector<ll> b(a.begin(), a.begin() + max(n, 0));
	rep(m,1,n) rep(k,0,m) {
		int d = m/(k+1) - 1;
		if (m % (k+1) || d > D || n-k < m) continue;
		Mat P = findPRec(b, k, d);
		bool ok = !P.empty();
		rep(i,n,sz(a)) ok &= nextTerm(P, a, i) == a[i];
		if (ok) return P;
	}
	return {};
}

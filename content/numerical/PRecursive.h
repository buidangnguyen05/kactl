/**
 * Author: Bui Dang Nguyen
 * Date: 2026-10-04
 * License: CC0
 * Source: folklore (guessing holonomic sequences by linear algebra)
 * Description: Finds polynomials $P_j$ of degree $\le d$ with
 * $\sum_{j=0}^{k} P_j(i)\,a_{i-j} = 0$ for $k \le i < n$, from $n > k$ terms mod a prime,
 * as $P[j][e]$ = coefficient of $i^e$ in $P_j$ (empty if none). guessPRec tries
 * $d \le D$ and any $k$ by increasing $m = (k+1)(d+1)$, keeping the first fit that
 * predicts 5 held-out terms; needs about $m+k+5$ terms. A low $D$ may force a longer
 * relation. nextTerm gives $a_i$ from earlier terms, or $-1$ if $P_0(i) = 0$.
 * Usage: Mat P = guessPRec(a, 3); // check sz(P) > 0
 * while (sz(a) < N) a.push_back(nextTerm(P, a, sz(a)));
 * Time: O(n m^2) per $(k,d)$ tried, nextTerm O(m + \log mod).
 * Status: stress-tested
 */
#pragma once

#include "SolveLinearMod.h"

Mat findPRec(const vector<ll>& a, int k, int d) {
	int n = sz(a) - k, w = d + 1;
	Mat A(n, vector<ll>((k+1) * w)), P(k+1);
	rep(i,0,n) rep(j,0,k+1) for (ll e = 0, p = a[i+k-j]; e < w; e++)
		A[i][j*w+e] = p, p = p * (i+k) % mod;
	Mat N = nullspace(A);
	if (N.empty()) return {};
	rep(j,0,k+1) P[j].assign(N[0].begin() + j*w, N[0].begin() + (j+1)*w);
	return P;
}

ll nextTerm(const Mat& P, const vector<ll>& a, int i) {
	ll s = 0, q = 0;
	rep(j,0,sz(P)) {
		ll v = 0;
		for (int e = sz(P[j]); e--;) v = (v * i + P[j][e]) % mod;
		if (j) s = (s + v * a[i-j]) % mod; else q = v;
	}
	return q ? (mod - s) * modpow(q, mod-2) % mod : -1;
}

Mat guessPRec(const vector<ll>& a, int D) {
	int n = sz(a) - 5; // last 5 terms are held out for checking
	rep(m,1,n) rep(k,0,m) {
		int d = m/(k+1) - 1;
		if (m % (k+1) || d > D || n-k < m) continue;
		Mat P = findPRec({a.begin(), a.begin() + n}, k, d);
		bool ok = sz(P);
		rep(i,n,sz(a)) ok &= nextTerm(P, a, i) == a[i];
		if (ok) return P;
	}
	return {};
}

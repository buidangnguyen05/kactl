/**
 * Author: Bui Dang Nguyen
 * Date: 2026-10-04
 * License: CC0
 * Source: folklore (Gauss-Jordan elimination)
 * Description: Linear algebra mod a prime. rref turns $A$ ($n \times m$) into reduced
 * row echelon form in place and returns the pivot column of each nonzero row
 * (its size is the rank). solveLinearMod solves $Ax = b$ for $x$ of size $m$, free
 * variables set to 0; returns rank, or $-1$ if no solution. nullspace returns a basis of
 * $\{x : Ax = 0\}$ ($A$ needs $\ge 1$ row): for each non-pivot column $c$, $x_c = 1$.
 * Values in $[0, mod)$.
 * Usage: vector<ll> x(m); int r = solveLinearMod(A, b, x);
 * Time: O(nm \cdot rank)
 * Status: bruteforce-tested mod 5 for n, m <= 4
 */
#pragma once

#include "../number-theory/ModPow.h"

typedef vector<vector<ll>> Mat;
vi rref(Mat& A) {
	int n = sz(A), m = n ? sz(A[0]) : 0;
	vi piv;
	rep(c,0,m) {
		int r = sz(piv), s = r;
		while (s < n && !A[s][c]) s++;
		if (s == n) continue;
		swap(A[r], A[s]);
		ll v = modpow(A[r][c], mod-2);
		for (ll& x : A[r]) x = x * v % mod;
		rep(t,0,n) if (t != r && A[t][c]) {
			ll f = A[t][c];
			rep(u,c,m) A[t][u] = (A[t][u] - f * A[r][u] % mod + mod) % mod;
		}
		piv.push_back(c);
	}
	return piv;
}

int solveLinearMod(Mat A, const vector<ll>& b, vector<ll>& x) {
	int m = sz(x);
	rep(i,0,sz(A)) A[i].push_back(b[i]);
	vi piv = rref(A);
	if (sz(piv) && piv.back() == m) return -1;
	x.assign(m, 0);
	rep(t,0,sz(piv)) x[piv[t]] = A[t][m];
	return sz(piv);
}

Mat nullspace(Mat A) {
	int m = sz(A[0]);
	vi piv = rref(A), fr(m, 1);
	for (int c : piv) fr[c] = 0;
	Mat res;
	rep(c,0,m) if (fr[c]) {
		vector<ll> x(m);
		x[c] = 1;
		rep(t,0,sz(piv)) x[piv[t]] = (mod - A[t][c]) % mod;
		res.push_back(x);
	}
	return res;
}

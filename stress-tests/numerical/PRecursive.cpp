#include "../utilities/template.h"

#include "../../content/numerical/PRecursive.h"

const int N = 400;
ll fact[2*N+1], ifact[2*N+1];
ll inv(ll x) { return modpow(x, mod-2); }
ll C(int n, int k) {
	if (k < 0 || k > n) return 0;
	return fact[n] * ifact[k] % mod * ifact[n-k] % mod;
}

// Sequences computed from their definitions, not from a recurrence.
vector<pair<string, vector<ll>>> known() {
	vector<pair<string, vector<ll>>> res;
	auto add = [&](string name, auto f) {
		vector<ll> a(N);
		rep(n,0,N) a[n] = ((f(n) % mod) + mod) % mod;
		res.push_back({name, a});
	};
	add("factorial", [](int n) { return fact[n]; });
	add("2^n n!", [](int n) { return modpow(2, n) * fact[n]; });
	add("catalan", [](int n) { return C(2*n, n) * inv(n+1); });
	add("central binomial", [](int n) { return C(2*n, n); });
	add("fibonacci", [](int n) {
		ll x = 0, y = 1;
		rep(i,0,n) x = (x + y) % mod, swap(x, y);
		return x;
	});
	add("n^5", [](int n) { return modpow(n, 5); });
	add("derangements", [](int n) {
		ll s = 0;
		rep(k,0,n+1) s += (k % 2 ? mod - 1 : 1) * fact[n] % mod * ifact[k] % mod;
		return s;
	});
	add("involutions", [](int n) {
		ll s = 0;
		for (int k = 0; 2*k <= n; k++)
			s += fact[n] * ifact[k] % mod * inv(modpow(2, k)) % mod * ifact[n-2*k] % mod;
		return s;
	});
	add("motzkin", [](int n) {
		ll s = 0;
		for (int k = 0; 2*k <= n; k++) s += C(n, 2*k) * C(2*k, k) % mod * inv(k+1) % mod;
		return s;
	});
	add("harmonic", [](int n) {
		ll s = 0;
		rep(i,1,n+1) s += inv(i);
		return s % mod;
	});
	add("left factorial", [](int n) {
		ll s = 0;
		rep(k,0,n) s += fact[k];
		return s % mod;
	});
	add("central delannoy", [](int n) {
		ll s = 0;
		rep(k,0,n+1) s += C(n, k) * C(n+k, k) % mod;
		return s;
	});
	add("franel", [](int n) {
		ll s = 0;
		rep(k,0,n+1) s += modpow(C(n, k), 3);
		return s % mod;
	});
	add("apery", [](int n) {
		ll s = 0;
		rep(k,0,n+1) s += modpow(C(n, k) * C(n+k, k) % mod, 2);
		return s % mod;
	});
	// Sparse sequences: zero rows carry no information, so these need the
	// held-out check to avoid fitting polynomials through the data.
	add("binom(n,n/2) at even n", [](int n) { return n % 2 ? 0 : C(n, n/2); });
	add("n! at 3 | n", [](int n) { return n % 3 ? 0 : fact[n]; });
	add("catalan at 4 | n", [](int n) {
		return n % 4 ? 0 : C(n/2, n/4) * inv(n/4 + 1);
	});
	add("C(n,10) n!", [](int n) { return C(n, 10) * fact[n]; });
	add("15 zeros, then fibonacci", [](int n) {
		ll x = 0, y = 1;
		rep(i,0,n) x = (x + y) % mod, swap(x, y);
		return n < 15 ? 0 : x;
	});
	return res;
}

int main() {
	fact[0] = 1;
	rep(i,1,2*N+1) fact[i] = fact[i-1] * i % mod;
	rep(i,0,2*N+1) ifact[i] = inv(fact[i]);
	mt19937 rng(2);
	auto rnd = [&](ll lo, ll hi) { return uniform_int_distribution<ll>(lo, hi)(rng); };
	auto ev = [&](const vector<ll>& p, ll x) {
		ll v = 0;
		for (int e = sz(p); e--;) v = (v * x + p[e]) % mod;
		return v;
	};

	// Known sequences: guess from 60 terms, extend to N and compare.
	// Each has a relation of degree <= 3, so D = 3 must find one; a lower
	// cap may find nothing, but anything it finds must respect the cap.
	for (auto& [name, full] : known()) rep(D,0,4) {
		vector<ll> a(full.begin(), full.begin() + 60);
		Mat P = guessPRec(a, D);
		if (D == 3) assert(sz(P));
		if (P.empty()) continue;
		for (auto& p : P) assert(sz(p) <= D+1);
		while (sz(a) < N) a.push_back(nextTerm(P, a, sz(a)));
		assert(a == full);
	}

	// Order-degree trade-off: (n^2+1) n! has relations of order 1 and
	// degree 3, and of order 3 and degree 1, but none with constant
	// coefficients. Lowering the cap must switch to the longer relation.
	{
		vector<ll> a(60);
		rep(n,0,60) a[n] = (ll(n) * n + 1) % mod * fact[n] % mod;
		auto shape = [&](int D) {
			Mat P = guessPRec(a, D);
			return P.empty() ? pii(-1, -1) : pii(sz(P) - 1, sz(P[0]) - 1);
		};
		assert(shape(3) == pii(1, 3));
		assert(shape(2) == pii(3, 1));
		assert(shape(1) == pii(3, 1));
		assert(shape(0) == pii(-1, -1));
	}

	// findPRec with a fixed (k, d): Catalan gives a multiple of
	// (i+1) a_i - (4i-2) a_{i-1}.
	{
		vector<ll> cat(30);
		rep(n,0,30) cat[n] = C(2*n, n) * inv(n+1) % mod;
		Mat P = findPRec(cat, 1, 1);
		assert(sz(P) == 2 && sz(P[0]) == 2);
		ll s = P[0][0];
		assert(s);
		assert(P[0][1] == s);
		assert(P[1][0] == 2 * s % mod);
		assert(P[1][1] == (mod - 4) * s % mod);
		assert(findPRec(cat, 1, 0).empty()); // no constant-ratio relation
	}

	// Random relations of order k, degree d: the guess must reproduce
	// the whole sequence from (k+1)(d+1) + k + 5 terms (the minimum).
	rep(it,0,1000) {
		int k = (int)rnd(1, 4), d = (int)rnd(0, 3), M = 150;
		Mat R(k+1, vector<ll>(d+1));
		for (auto& p : R) for (ll& x : p) x = rnd(0, mod-1);
		vector<ll> full(M);
		rep(i,0,k) full[i] = rnd(0, mod-1);
		bool ok = true;
		rep(i,k,M) {
			ll s = 0, q = ev(R[0], i);
			rep(j,1,k+1) s = (s + ev(R[j], i) * full[i-j]) % mod;
			if (!q) { ok = false; break; }
			full[i] = (mod - s) * inv(q) % mod;
		}
		if (!ok) continue;
		int F = (k+1)*(d+1) + k + 5 + (int)rnd(0, 3);
		vector<ll> a(full.begin(), full.begin() + F);
		Mat P = guessPRec(a, d + (int)rnd(0, 2));
		assert(sz(P) == k+1 && sz(P[0]) == d+1);
		while (sz(a) < M) a.push_back(nextTerm(P, a, sz(a)));
		assert(a == full);
		if (d) { // below the true degree: nothing, or a correct relation within the cap
			a.resize(F);
			P = guessPRec(a, d-1);
			if (P.empty()) continue;
			for (auto& p : P) assert(sz(p) <= d);
			while (sz(a) < M) a.push_back(nextTerm(P, a, sz(a)));
			assert(a == full);
		}
	}

	// Sequences that are not P-recursive must be rejected.
	{
		rep(it,0,20) {
			vector<ll> a((size_t)rnd(1, 50));
			for (ll& x : a) x = rnd(0, mod-1);
			assert(guessPRec(a, 100).empty());
		}
		vector<ll> part(100), bell(100), sq(60);
		part[0] = 1; // partition numbers
		rep(c,1,100) rep(n,c,100) part[n] = (part[n] + part[n-c]) % mod;
		vector<ll> row = {1}; // Bell triangle
		rep(n,0,100) {
			bell[n] = row[0];
			vector<ll> nxt = {row.back()};
			for (ll x : row) nxt.push_back((nxt.back() + x) % mod);
			row = nxt;
		}
		rep(n,0,60) sq[n] = modpow(2, (ll)n * n);
		assert(guessPRec(part, 100).empty());
		assert(guessPRec(bell, 100).empty());
		assert(guessPRec(sq, 100).empty());
	}
	cout << "Tests passed!" << endl;
}

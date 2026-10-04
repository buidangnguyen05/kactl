#include "../utilities/template.h"

const ll mod = 5;
ll modpow(ll a, ll e) {
	if (e == 0) return 1;
	ll x = modpow(a * a % mod, e >> 1);
	return e & 1 ? x * a % mod : x;
}

#define mod dummy
#define modpow dummy2
#include "../../content/number-theory/ModPow.h"
#undef mod
#undef modpow

#include "../../content/numerical/SolveLinearMod.h"

int main() {
	const int P = (int)mod;
	mt19937 rng(1);
	auto rnd = [&](int lo, int hi) { return uniform_int_distribution<int>(lo, hi)(rng); };
	auto dot = [&](const vector<ll>& r, const vector<ll>& x) {
		ll s = 0;
		rep(j,0,sz(x)) s += r[j] * x[j];
		return s % mod;
	};
	rep(it,0,100000) {
		int n = rnd(1, 4), m = rnd(1, 4), zeroBias = rnd(0, 3);
		Mat A(n, vector<ll>(m));
		for (auto& row : A) for (ll& x : row) x = rnd(0, 3) < zeroBias ? 0 : rnd(0, P-1);
		if (n > 1 && rnd(0, 2) == 0) { // a row that is a multiple of row 0
			int i = rnd(1, n-1), f = rnd(0, P-1);
			rep(c,0,m) A[i][c] = A[0][c] * f % mod;
		}
		vector<ll> b(n);
		for (ll& x : b) x = rnd(0, P-1);
		if (rnd(0, 1)) { // make the system consistent
			vector<ll> x0(m);
			for (ll& x : x0) x = rnd(0, P-1);
			rep(i,0,n) b[i] = dot(A[i], x0);
		}

		// brute force: count solutions of Ax = b and Ax = 0
		int total = 1, cntB = 0, cnt0 = 0;
		rep(j,0,m) total *= P;
		rep(code,0,total) {
			vector<ll> x(m);
			int c = code;
			rep(j,0,m) x[j] = c % P, c /= P;
			bool okB = true, ok0 = true;
			rep(i,0,n) {
				ll s = dot(A[i], x);
				okB &= s == b[i];
				ok0 &= s == 0;
			}
			cntB += okB, cnt0 += ok0;
		}
		int dim = 0;
		for (int c = cnt0; c > 1; c /= P) dim++;
		int rank = m - dim;

		// rref gives reduced row echelon form
		Mat R = A;
		vi piv = rref(R);
		assert(sz(piv) == rank);
		rep(t,0,sz(piv)) {
			if (t) assert(piv[t-1] < piv[t]);
			rep(i,0,n) assert(R[i][piv[t]] == (i == t));
			rep(c,0,piv[t]) assert(R[t][c] == 0);
		}
		rep(t,sz(piv),n) rep(c,0,m) assert(R[t][c] == 0);

		vector<ll> x(m);
		int r = solveLinearMod(A, b, x);
		if (!cntB) assert(r == -1);
		else {
			assert(r == rank);
			rep(i,0,n) assert(dot(A[i], x) == b[i]);
		}

		// nullspace: right size, each vector in the kernel, and independent
		// (vector p is 1 at the p'th free column and 0 at the other free columns)
		Mat N = nullspace(A);
		assert(sz(N) == dim);
		vi fr;
		rep(c,0,m) if (!count(all(piv), c)) fr.push_back(c);
		rep(p,0,sz(N)) {
			assert(sz(N[p]) == m);
			rep(i,0,n) assert(dot(A[i], N[p]) == 0);
			rep(q,0,sz(fr)) assert(N[p][fr[q]] == (p == q));
		}
	}
	{ // no equations
		Mat A;
		vector<ll> b, x(3, 1);
		assert(solveLinearMod(A, b, x) == 0 && x == vector<ll>(3));
	}
	cout << "Tests passed!" << endl;
}

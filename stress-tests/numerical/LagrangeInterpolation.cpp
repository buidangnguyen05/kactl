#include "../utilities/template.h"
#include "../../content/number-theory/ModPow.h"
#include "../../content/numerical/LagrangeInterpolation.h"

int main() {
	srand(11);
	// random polynomials, evaluated directly vs interpolated
	rep(it,0,300) {
		int d = rand() % 8;
		vector<ll> c(d + 1);
		rep(i,0,d+1) c[i] = rand() % mod;
		auto eval = [&](ll x) {
			ll r = 0, p = 1;
			rep(i,0,d+1) r = (r + c[i] * p) % mod, p = p * (x%mod) % mod;
			return r;
		};
		vector<ll> y(d + 1);
		rep(i,0,d+1) y[i] = eval(i);
		rep(q,0,20) {
			ll x = rand() % 1000000;
			assert(lagrange(y, x) == eval(x));
		}
	}
	// power sums: sum_{i=1}^{x} i^k for k = 0..5
	rep(k,0,6) {
		vector<ll> y(k + 3, 0);
		rep(i,1,k+3) y[i] = (y[i-1] + modpow(i, k)) % mod;
		rep(x,0,300) {
			ll want = 0;
			rep(i,1,x+1) want = (want + modpow(i, k)) % mod;
			assert(lagrange(y, x) == want);
		}
	}
	cout<<"Tests passed!"<<endl;
}

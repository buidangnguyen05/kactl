#include "../utilities/template.h"
#include "../../content/number-theory/CountPrimes.h"

int main() {
	const int L = 1000000;
	vector<char> comp(L + 1, 0);
	vi pi(L + 1, 0);
	rep(i,2,L+1) {
		if (!comp[i]) for (ll j = (ll)i*i; j <= L; j += i) comp[j] = 1;
		pi[i] = pi[i-1] + !comp[i];
	}
	rep(n,0,20001) assert(countPrimes(n) == pi[n]);   // exhaustive
	srand(1);
	rep(it,0,2000) { int n = rand() % (L + 1);        // sampled
		assert(countPrimes(n) == pi[n]); }
	assert(countPrimes(10000000LL) == 664579LL);      // known values
	assert(countPrimes(100000000LL) == 5761455LL);
	assert(countPrimes(1000000000LL) == 50847534LL);
	assert(countPrimes(10000000000LL) == 455052511LL);
	cout<<"Tests passed!"<<endl;
}

#include "../utilities/template.h"

#include "../../content/number-theory/FlooredDivisors.h"

int main() {
	rep(n,1,3001) {
		vector<pii> v = floors(n);
		set<int> values;
		rep(i,1,n+1) values.insert(n / i);
		assert(sz(v) == sz(values));
		int next = 1;
		for (auto [L, R] : v) {
			assert(L == next && L <= R);
			assert(n / L == n / R);
			assert(R == n || n / (R + 1) != n / R); // maximal
			next = R + 1;
		}
		assert(next == n + 1);
	}
	cout << "Tests passed!" << endl;
}

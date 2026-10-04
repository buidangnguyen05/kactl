#include "../utilities/template.h"
#include "../../content/various/BigNum.h"

typedef __int128 L;
string str(L x) {
	if (!x) return "0";
	bool neg = x < 0; if (neg) x = -x;
	string s;
	for (; x; x /= 10) s += char('0' + int(x % 10));
	if (neg) s += '-';
	reverse(all(s)); return s;
}
string str(const Big& b) { ostringstream o; o << b; return o.str(); }

int main() {
	srand(41);
	auto rnd = [&](int mx) {
		L x = 0; int d = rand() % mx + 1;
		rep(i,0,d) { x = x * 10 + rand() % 10; }
		return rand() % 2 ? x : -x;
	};
	rep(it,0,20000) {
		L x = rnd(30), y = rnd(30);
		Big a(str(x)), b(str(y));
		assert(str(a) == str(x) && str(b) == str(y));
		assert(str(a + b) == str(x + y));
		assert(str(a - b) == str(x - y));       // signs: now total
		assert(str(-a) == str(-x));
		assert((a < b) == (x < y));
		assert((a == b) == (x == y));
		L u = rnd(18), v = rnd(18);
		assert(str(Big(str(u)) * Big(str(v))) == str(u * v));
	}
	// Many-limb products, checked mod primes; limbs must stay normalized.
	// All-(B-1) limbs give the longest carry chains.
	auto modp = [](const Big& b, ll p) {
		ll r = 0;
		for (int i = sz(b.a); i--;) r = (r * Big::B + b.a[i]) % p;
		return b.sg < 0 ? (p - r) % p : r;
	};
	const ll ps[] = {1000000007, 998244353, 2147483647};
	rep(it,0,3000) {
		Big x, y;
		int n = rand() % 40, m = rand() % 40, all9 = rand() % 4 == 0;
		rep(i,0,n) x.a.push_back(all9 ? Big::B - 1 : rand() % Big::B);
		rep(i,0,m) y.a.push_back(all9 ? Big::B - 1 : rand() % Big::B);
		x.sg = rand() % 2 ? 1 : -1, y.sg = rand() % 2 ? 1 : -1;
		x.trim(), y.trim();
		Big z = x * y;
		for (int v : z.a) assert(0 <= v && v < Big::B);
		assert(z.a.empty() || z.a.back());
		if (sz(x.a) && sz(y.a)) assert(sz(z.a) >= sz(x.a) + sz(y.a) - 1);
		for (ll p : ps) assert(modp(z, p) == modp(x, p) * modp(y, p) % p);
	}
	assert(str(Big(-1234567890123456789LL)) == "-1234567890123456789");
	Big f(1);
	rep(i,1,101) f = f * Big((ll)i);
	string f100 = "9332621544394415268169923885626670049071596826438"
		"162146859296389521759999322991560894146397615651828625369792"
		"0827223758251185210916864000000000000000000000000";
	assert(str(f) == f100);
	istringstream in("-987654321098765432109876543210");
	Big h; in >> h;
	assert(str(h) == "-987654321098765432109876543210");
	assert(str(Big(0)) == "0" && str(Big("0")) == "0" && str(-Big(0)) == "0");
	cout<<"Tests passed!"<<endl;
}

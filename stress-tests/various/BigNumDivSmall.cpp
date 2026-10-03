#include "../utilities/template.h"
#include "../../content/various/BigNumDivSmall.h"

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
		L x = rnd(30);
		Big a(str(x));
		int d = rand() % 1000000 + 1;
		assert(str(a / d) == str(x / d));
		assert(a % d == int(x % d));
	}
	string f100 = "9332621544394415268169923885626670049071596826438"
		"162146859296389521759999322991560894146397615651828625369792"
		"0827223758251185210916864000000000000000000000000";
	Big g(f100);
	rep(i,1,101) g = g / i;
	assert(str(g) == "1");
	assert(str(Big(f100) % 997) == "172");
	assert(str(Big(0) / 7) == "0" && Big(0) % 7 == 0);
	cout<<"Tests passed!"<<endl;
}

/**
 * Author: buidangnguyen05
 * License: CC0
 * Description: Arbitrary-precision signed integer, base $10^9$,
 *  little-endian limbs. Addition, multiplication, comparison and I/O;
 *  division lives in BigNumDivMod.h and BigNumDivSmall.h.
 * Time: $O(n)$ for $\pm$, $O(nm)$ for $*$
 * Status: stress-tested against \_\_int128
 */
#pragma once

struct Big {
	static constexpr int B = 1000000000, W = 9;
	int sg = 1; vi a; // value = sg * a[], a little-endian, no lead 0
	Big(ll x = 0) {
		if (x < 0) sg = -1, x = -x;
		for (; x > 0; x /= B) a.push_back(int(x % B));
	}
	Big(const string& s) {
		int st = s[0] == '-' ? (sg = -1, 1) : 0;
		for (int i = sz(s), j; i > st; i = j)
			j = max(st, i - W), a.push_back(stoi(s.substr(j, i - j)));
		trim();
	}
	void trim() {
		while (sz(a) && !a.back()) a.pop_back();
		if (a.empty()) sg = 1; }
	static int cmp(const vi& x, const vi& y) { // compare magnitudes
		if (sz(x) != sz(y)) return sz(x) < sz(y) ? -1 : 1;
		for (int i = sz(x); i--;)
			if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
		return 0;
	}
	static void addTo(vi& x, const vi& y, int s) { // x += s * y
		assert(s > 0 || cmp(x, y) >= 0); // else borrow is dropped
		x.resize(max(sz(x), sz(y)) + 1, 0);
		int c = 0;
		rep(i,0,sz(x)) {
			int v = x[i] + s * (i < sz(y) ? y[i] : 0) + c;
			x[i] = v - (c = v < 0 ? -1 : v >= B) * B;
		}
		while (sz(x) && !x.back()) x.pop_back();
	}
	Big operator-() const {
		Big r = *this; r.sg = -sg; r.trim(); return r; }
	Big& operator+=(const Big& o) {
		if (sg == o.sg) addTo(a, o.a, 1);
		else if (cmp(a, o.a) >= 0) addTo(a, o.a, -1);
		else { vi t = o.a; addTo(t, a, -1); a = t, sg = o.sg; }
		trim(); return *this;
	}
	Big& operator-=(const Big& o) { return *this += -o; }
	bool operator<(const Big& o) const {
		if (sg != o.sg) return sg < o.sg;
		return sg * cmp(a, o.a) < 0;
	}
	bool operator==(const Big& o) const { return sg == o.sg && a == o.a; }
	friend Big operator*(const Big& x, const Big& y) {
		Big r; vector<ll> c(sz(x.a) + sz(y.a)); // carry as we go
		rep(i,0,sz(x.a)) rep(j,0,sz(y.a)) {
			c[i+j] += (ll)x.a[i] * y.a[j];
			c[i+j+1] += c[i+j] / B, c[i+j] %= B;
		}
		for (ll v : c) r.a.push_back(int(v));
		r.sg = x.sg * y.sg; r.trim(); return r;
	}
	friend Big operator+(Big x, const Big& y) { return x += y; }
	friend Big operator-(Big x, const Big& y) { return x -= y; }
	friend ostream& operator<<(ostream& os, const Big& x) {
		if (x.a.empty()) return os << 0;
		if (x.sg < 0) os << '-';
		os << x.a.back();
		char f = os.fill('0');
		for (int i = sz(x.a) - 1; i--;) os << setw(W) << x.a[i];
		os.fill(f); return os;
	}
	friend istream& operator>>(istream& is, Big& x) {
		string s; is >> s; x = Big(s); return is;
	}
};

/**
 * Author: Epiphyllum
 * Description: Fixed-size bitset with the word array exposed, for
 *  hand-rolled word tricks. Set \texttt{L} to $\lceil n/64 \rceil$.
 * Time: O(n/64) per operation
 * Status: stress-tested
 */
#pragma once

typedef unsigned long long ull;

struct bit {
	static const int L = 782;
	ull t[L] = {};
	void set(int x, bool y = 1) {
		if (y) t[x >> 6] |= 1ull << (x & 63);
		else t[x >> 6] &= ~(1ull << (x & 63));
	}
	void reset() { memset(t, 0, sizeof(t)); }
	void flip(int x) { t[x >> 6] ^= 1ull << (x & 63); }
	bool test(int x) const { return t[x >> 6] >> (x & 63) & 1; }
	ull& operator[](int i) { return t[i]; }
	int count() const {
		int res = 0;
		rep(i, 0, L) res += __builtin_popcountll(t[i]);
		return res;
	}
	int find_first() const { /// index of lowest set bit, L*64 if none
		rep(i, 0, L) if (t[i]) return i * 64 + __builtin_ctzll(t[i]);
		return L * 64;
	}
#define OP(o) friend bit operator o(const bit& a, const bit& b) { \
		bit c; rep(i, 0, L) c.t[i] = a.t[i] o b.t[i]; return c; }
	OP(^) OP(&) OP(|)
};

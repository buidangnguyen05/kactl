/**
 * Author: adamant
 * Source: https://codeforces.com/blog/entry/111602
 * Description: Order-statistic set over integers in $[0, N)$. Much
 *  faster than the pb\_ds tree below. Holds $N$ ints, declare globally.
 * Time: O(\log N) per operation
 * Status: stress-tested
 */
#pragma once

// Non-integer or uncompressible keys? Use pb_ds instead, with
// <ext/pb_ds/assoc_container.hpp> and <ext/pb_ds/tree_policy.hpp>:
// __gnu_pbds::tree<T, __gnu_pbds::null_type, less<T>,
//  __gnu_pbds::rb_tree_tag,
//  __gnu_pbds::tree_order_statistics_node_update> s;

template<int N> struct OrderedSet {
	int s[N + 1] = {}, num = 0;
	bitset<N> in;
	void upd(int x, int v) { for (++x; x <= N; x += x&-x) s[x] += v; }
	void insert(int x) { if (!in[x]) in[x] = 1, num++, upd(x, 1); }
	void erase(int x) { if (in[x]) in[x] = 0, num--, upd(x, -1); }
	int size() const { return num; }
	bool count(int x) const { return in[x]; }
	int order_of_key(int x) const { // number of elements < x
		int r = 0; for (; x; x -= x&-x) r += s[x]; return r;
	}
	int find_by_order(int k) const { // kth smallest, -1 if k >= size()
		if (k >= num) return -1;
		int x = 0;
		for (int i = 1 << __lg(N); i; i /= 2)
			if (x + i <= N && s[x + i] <= k) k -= s[x + i], x += i;
		return x;
	}
	int next(int x) const { // smallest element >= x, -1 if none
		return find_by_order(order_of_key(x));
	}
	int prev(int x) const { // largest element <= x, -1 if none
		int k = order_of_key(x + 1);
		return k ? find_by_order(k - 1) : -1;
	}
};

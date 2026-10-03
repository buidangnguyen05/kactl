/**
 * Author: awk
 * Description: Static wavelet tree over values in $[lo, hi]$, 1-indexed
 *  ranges. \texttt{rank} counts elements $\le k$, \texttt{sumSmallest}
 *  sums the $k$ smallest; their difference gives the $k$th smallest.
 *  \texttt{build} permutes the input.
 * Usage: WaveletTree w; w.build(1, all(a), lo, hi);
 * Time: $O(\log(hi-lo))$ per query, $O(n \log(hi-lo))$ to build
 * Status: stress-tested
 */
#pragma once

struct WaveletTree {
	struct TNode { int l = 0, r = 0, lp = 0, rp = 0; };
	vector<vi> val;      // val[id][p] = # of first p elements going left
	vector<vector<ll>> sum;
	vector<TNode> wt;
	int add() {
		val.emplace_back(), sum.emplace_back(), wt.emplace_back();
		return sz(wt) - 1;
	}
	WaveletTree() { add(), add(); } // 0 unused, 1 is the root
	template<class It> void build(int id, It i, It j, int l, int r) {
		wt[id].l = l, wt[id].r = r;
		if (l == r) return;
		val[id].push_back(0), sum[id].push_back(0);
		int m = (l + r - (l + r < 0)) / 2; // floor, also for l+r < 0
		int mx = l, mn = r;
		for (auto it = i; it != j; ++it) {
			val[id].push_back(val[id].back() + (*it <= m));
			sum[id].push_back(sum[id].back() + *it * (*it <= m));
			if (*it <= m) mx = max(mx, *it); else mn = min(mn, *it);
		}
		auto p = stable_partition(i, j, [=](auto w) { return w <= m; });
		int a = add(), b = add();
		wt[id].lp = a, wt[id].rp = b;
		build(a, i, p, l, mx), build(b, p, j, mn, r);
	}
	ll rank(int k, int i, int j) { // # of elements <= k in [i, j]
		--i;
		ll c = 0; int id = 1, l = wt[id].l, r = wt[id].r;
		while (l < r) {
			int m = (l + r - (l + r < 0)) / 2;
			if (k <= m) i = val[id][i], j = val[id][j], id = wt[id].lp;
			else {
				c += val[id][j] - val[id][i];
				i -= val[id][i], j -= val[id][j], id = wt[id].rp;
			}
			l = wt[id].l, r = wt[id].r;
		}
		return c + (k >= l) * (j - i);
	}
	ll sumSmallest(int k, int i, int j) { // sum of k smallest in [i, j]
		if (!k) return 0;
		--i;
		ll res = 0; int id = 1, l = wt[id].l, r = wt[id].r;
		while (l < r) {
			if (k <= val[id][j] - val[id][i])
				i = val[id][i], j = val[id][j], id = wt[id].lp;
			else {
				res += sum[id][j] - sum[id][i];
				k -= val[id][j] - val[id][i];
				i -= val[id][i], j -= val[id][j], id = wt[id].rp;
			}
			l = wt[id].l, r = wt[id].r;
		}
		return res + (ll)k * l;
	}
};

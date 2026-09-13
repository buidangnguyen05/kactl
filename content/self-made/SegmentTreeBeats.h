/**
 * Author: Jerry
 * Date: 2023-10-01
 * License: CC0
 * Description: Segment Tree Beats.
 * Converts range min-max queries into addition queries.
 * Problem statement:
 * Given an array, perform operations of 4 kinds: range max, range min, range add, range sum.
 * Time: O(n * log_2(n))
 */

const ll inf = 1e18;
ll a[N];
struct Node {
	ll max1, max2, cnt_max;
	ll min1, min2, cnt_min;
	ll sum, lazy;
};
struct SegmentTree {
	Node T[4 * N];
	void merge(int s) {
		T[s].sum = T[s << 1].sum + T[s << 1 | 1].sum;
		if (T[s << 1].max1 == T[s << 1 | 1].max1) {
			T[s].max1 = T[s << 1].max1;
			T[s].cnt_max = T[s << 1].cnt_max + T[s << 1 | 1].cnt_max;
			T[s].max2 = max(T[s << 1].max2, T[s << 1 | 1].max2);
		}
		else if (T[s << 1].max1 > T[s << 1 | 1].max1) {
			T[s].max1 = T[s << 1].max1;
			T[s].cnt_max = T[s << 1].cnt_max;
			T[s].max2 = max(T[s << 1].max2, T[s << 1 | 1].max1);
		}
		else {
			T[s].max1 = T[s << 1 | 1].max1;
			T[s].cnt_max = T[s << 1 | 1].cnt_max;
			T[s].max2 = max(T[s << 1].max1, T[s << 1 | 1].max2);
		}
		if (T[s << 1].min1 == T[s << 1 | 1].min1) {
			T[s].min1 = T[s << 1].min1;
			T[s].cnt_min = T[s << 1].cnt_min + T[s << 1 | 1].cnt_min;
			T[s].min2 = min(T[s << 1].min2, T[s << 1 | 1].min2);
		}
		else if (T[s << 1].min1 < T[s << 1 | 1].min1) {
			T[s].min1 = T[s << 1].min1;
			T[s].cnt_min = T[s << 1].cnt_min;
			T[s].min2 = min(T[s << 1].min2, T[s << 1 | 1].min1);
		}
		else {
			T[s].min1 = T[s << 1 | 1].min1;
			T[s].cnt_min = T[s << 1 | 1].cnt_min;
			T[s].min2 = min(T[s << 1].min1, T[s << 1 | 1].min2);
		}
	}
	void build(int s = 1, int l = 0, int r = n - 1) {
		if (l == r) {
			T[s].max1 = T[s].min1 = T[s].sum = a[l];
			T[s].cnt_max = T[s].cnt_min = 1;
			T[s].max2 = -inf, T[s].min2 = inf;
			return;
		}
		int mid = (l + r) >> 1;
		build(s << 1, l, mid); build(s << 1 | 1, mid + 1, r);
		merge(s);
	}
	void push_add(int s, int l, int r, ll val) {
		if (!val) return;
		T[s].sum += 1LL * (r - l + 1) * val;
		T[s].max1 += val; if (T[s].max2 != -inf) T[s].max2 += val;
		T[s].min1 += val; if (T[s].min2 != inf) T[s].min2 += val;
		T[s].lazy += val;
	}
	void push_max(int s, ll val, bool v) {
		if (val >= T[s].max1) return;
		T[s].sum -= 1LL * T[s].max1 * T[s].cnt_max;
		T[s].max1 = val;
		T[s].sum += 1LL * T[s].max1 * T[s].cnt_max;
		if (v) T[s].min1 = T[s].max1;
		else {
			if (val <= T[s].min1) T[s].min1 = val;
			else if (val < T[s].min2) T[s].min2 = val;
		}
	}
	void push_min(int s, ll val, bool v) {
		if (val <= T[s].min1) return;
		T[s].sum -= 1LL * T[s].min1 * T[s].cnt_min;
		T[s].min1 = val;
		T[s].sum += 1LL * T[s].min1 * T[s].cnt_min;

		if (v) T[s].max1 = T[s].min1;
		else {
			if (val >= T[s].max1) T[s].max1 = val;
			else if (val > T[s].max2) T[s].max2 = val;
		}
	}
	void push_down(int s, int l, int r) {
		if (l == r) return;
		int mid = (l + r) >> 1;
		if (T[s].lazy) {
			push_add(s << 1, l, mid, T[s].lazy);
			push_add(s << 1 | 1, mid + 1, r, T[s].lazy);
			T[s].lazy = 0;
		}
		push_max(s << 1, T[s].max1, l == mid);
		push_max(s << 1 | 1, T[s].max1, mid + 1 == r);
		push_min(s << 1, T[s].min1, l == mid);
		push_min(s << 1 | 1, T[s].min1, mid + 1 == r);
	}
	void update_add(int s, int l, int r, int u, int v, ll val) {
		if (l > v || r < u) return;
		if (l >= u && r <= v) {
			push_add(s, l, r, val);
			return;
		}
		push_down(s, l, r);
		int mid = (l + r) >> 1;
		update_add(s << 1, l, mid, u, v, val);
		update_add(s << 1 | 1, mid + 1, r, u, v, val);
		merge(s);
	}
	void update_max(int s, int l, int r, int u, int v, ll val) {
		if (l > v || r < u || val <= T[s].min1) return;
		if (l >= u && r <= v && val < T[s].min2) {
			push_min(s, val, l == r);
			return;
		}
		push_down(s, l, r);
		int mid = (l + r) >> 1;
		update_max(s << 1, l, mid, u, v, val);
		update_max(s << 1 | 1, mid + 1, r, u, v, val);
		merge(s);
	}
	void update_min(int s, int l, int r, int u, int v, ll val) {
		if (l > v || r < u || val >= T[s].max1) return;
		if (l >= u && r <= v && val > T[s].max2) {
			push_max(s, val, l == r);
			return;
		}
		push_down(s, l, r);
		int mid = (l + r) >> 1;
		update_min(s << 1, l, mid, u, v, val);
		update_min(s << 1 | 1, mid + 1, r, u, v, val);
		merge(s);
	}
	ll get(int s, int l, int r, int u, int v) {
		if (l > v || r < u) return 0;
		if (l >= u && r <= v) return T[s].sum;
		push_down(s, l, r);
		int mid = (l + r) >> 1;
		return get(s << 1, l, mid, u, v) + get(s << 1 | 1, mid + 1, r, u, v);
	}
} it;

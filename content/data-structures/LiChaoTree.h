/**
 * Author: buidangnguyen05
 * Description: Maintains a set of lines $y(x) = ax + b$ and returns the minimum $y(pos)$.
 * Time: O(n \cdot \log_2 n)
 */

#pragma once
struct line {
	int a, b;
};
int get(line m, int x) {
	return 1ll * m.a * x + m.b;
}
struct LiChaoTree {
	line t[4 * N];
	ll query(int s, int l, int r, int pos) {
		if (l > pos || r < pos) return 1e18;
		int res = get(t[s], pos);
		if (l == r) return res;
		int mid = (l + r) / 2;
		res = min(res, query(s * 2, l, mid, pos));
		res = min(res, query(s * 2 + 1, mid + 1, r, pos));
		return res;
	}
	void update(int s, int l, int r, int u, int v, line val) {
		if (l > v || u > r) return;
		int mid = (l + r) / 2;
		if (l >= u && r <= v) {
			if (get(t[s], l) <= get(val, l) && get(t[s], r) <= get(val, r)) return;
			if (get(t[s], l) >= get(val, l) && get(t[s], r) >= get(val, r)) {
				t[s] = val;
				return;
			}
			if (get(t[s], l) <= get(val, l) && get(t[s], mid) <= get(val, mid)) {
				update(2 * s + 1, mid + 1, r, u, v, val);
				return;
			}
			if (get(t[s], l) >= get(val, l) && get(t[s], mid + 1) >= get(val, mid + 1)) {
				update(2 * s + 1, mid + 1, r, u, v, t[s]);
				t[s] = val;
				return;
			}
			if (get(t[s], r) <= get(val, r) && get(t[s], mid) <= get(val, mid)) {
				update(2 * s, l, mid, u, v, val);
				return;
			}
			if (get(t[s], r) >= get(val, r) && get(t[s], mid + 1) >= get(val, mid + 1)) {
				update(2 * s, l, mid, u, v, t[s]);
				t[s] = val;
				return;
			}
		}
		update(2 * s, l, mid, u, v, val);
		update(2 * s + 1, mid + 1, r, u, v, val);
	}
} it;
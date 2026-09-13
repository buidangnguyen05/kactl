/**
 * Author: Epiphyllum
 * Description: Mo's algorithm. Includes 3d and 4d expansion.
 * Time: O(n \cdot \sqrt(n)) for 2d. 3d and 4d time in comments.
 * Status: stress-tested
 */
#pragma once

struct query {
	int l, r, id;
	friend bool operator < (query a, query b) {
		if (a.l / L != b.l / L) return a.l / L < b.l / L;
		if ((a.l / L) & 1) return a.r > b.r;
		else return a.r < b.r;
	}
};
void work() {
	repn(i, 1, m) {
		while (r < a[i].r) inc(++r);
		while (l > a[i].l) inc(--l);
		while (r > a[i].r) dec(r--);
		while (l < a[i].l) dec(l++);
	}
}
// n^{5/3}
const int L = pow(N, 2.0 / 3.0) + 10;
struct operation {
	int p, x, y;
}ops[N];
int ans[N];
struct query {
	int l, r, t, id;
	friend bool operator < (query a, query b) {
		if (a.l / L != b.l / L) return a.l / L < b.l / L;
		if (a.r / L != b.r / L) return a.r / L < b.r / L;
		if ((a.r / L) & 1) return a.t > b.t;
		else return a.t < b.t;
	}
};
vector<query> qry;
void MoAlgo() {
	int l = 1, r = 0, t = 0;
	for (auto p: qry) {
		int pl = p.l, pr = p.r, pt = p.t, id = p.id;
		while (t < pt) tinc(++t, l, r);
		while (t > pt) tdec(t--, l, r);
		while (r < pr) inc(++r);
		while (l > pl) inc(--l);
		while (r > pr) dec(r--);
		while (l < pl) dec(l++);
		ans[id] = getans();
	}
}
// n^{7/4}
const int B = 10000;
struct query {
	int l1, r1, l2, r2, id;
	friend bool operator < (query a, query b) {
		if (a.l1 / B != b.l1 / B) return a.l1 / B < b.l1 / B;
		if (a.r1 / B != b.r1 / B) return a.r1 / B < b.r1 / B;
		if (a.l2 / B != b.l2 / B) return a.l2 / B < b.l2 / B;
		return a.r2 < b.r2;
	}
}a[N];
void work() {
	repn(i, 1, q) {
		while (l1 > a[i].l1) inc(--l1);
		while (r1 < a[i].r1) inc(++r1);
		while (l2 > a[i].l2) inc(--l2);
		while (r2 < a[i].r2) inc(++r2);
		while (l1 < a[i].l1) dec(l1++);
		while (r1 > a[i].r1) dec(r1--);
		while (l2 < a[i].l2) dec(l2++);
		while (r2 > a[i].r2) dec(r2--);
	}
}
// roll back
#define mo RollBackMo
namespace RollBackMo {
	LL sum;
	struct record {
		int idx;
		LL dp[2][2], sum;
	};
	stack<record> st;
	void copy(LL dp1[2][2], LL dp2[2][2]) {
		dp1[0][0] = dp2[0][0];
		dp1[0][1] = dp2[0][1];
		dp1[1][0] = dp2[1][0];
		dp1[1][1] = dp2[1][1];
	}
	void add(int x, int dir, int l, int r) {
		int idx = x % k;
		record tmp;
		tmp.idx = idx, tmp.sum = sum;
		copy(tmp.dp, dp[idx]);
		st.push(tmp);
		int cnt = 1;
		if (x + k <= r) cnt++;
		if (x + k + k <= r) cnt++;
		if (x - k >= l) cnt++;
		if (x - k - k >= l) cnt++;
		if (cnt == 1) {
			dp[idx][0][0] = 0;
			dp[idx][1][0] = dp[idx][0][1] = dp[idx][1][1] = -INF;
		}
		else if (cnt == 2) {
			dp[idx][0][0] = 0;
			dp[idx][1][0] = dp[idx][0][1] = -INF;
			dp[idx][1][1] = a[x];
			if (x - k >= l) dp[idx][1][1] += a[x - k];
			if (x + k <= r) dp[idx][1][1] += a[x + k];
		}
		else if (dir == 1) {
			sum -= max(max(dp[idx][0][0], dp[idx][0][1]), max(dp[idx][1][0], dp[idx][1][1]));
			if (x - k >= l) {
				dp[idx][0][1] = tmp.dp[0][0] + a[x] + a[x - k];
				dp[idx][1][1] = tmp.dp[1][0] + a[x] + a[x - k];
			}
			else dp[idx][0][1] = dp[idx][1][1] = -INF;
			dp[idx][0][0] = max(tmp.dp[0][0], tmp.dp[0][1]);
			dp[idx][1][0] = max(tmp.dp[1][0], tmp.dp[1][1]);
		}
		else {
			sum -= max(max(dp[idx][0][0], dp[idx][0][1]), max(dp[idx][1][0], dp[idx][1][1]));
			if (x + k <= r) {
				dp[idx][1][0] = tmp.dp[0][0] + a[x] + a[x + k];
				dp[idx][1][1] = tmp.dp[0][1] + a[x] + a[x + k];
			}
			else dp[idx][1][0] = dp[idx][1][1] = -INF;
			dp[idx][0][0] = max(tmp.dp[0][0], tmp.dp[1][0]);
			dp[idx][0][1] = max(tmp.dp[0][1], tmp.dp[1][1]);
		}
		sum += max(max(dp[idx][0][0], dp[idx][0][1]), max(dp[idx][1][0], dp[idx][1][1]));
	}
	LL getans() {
		return sum;
	}
	void clear() {
		while (!st.empty()) st.pop();
	}
	void back() {
		while (!st.empty()) {
			auto p = st.top();
			st.pop();
			copy(dp[p.idx], p.dp);
			sum = p.sum;
		}
	}
	const int L = 320;
	struct query {
		int l, r, id;
		friend bool operator < (const query &a, const query &b) {
			if (a.l / L != b.l / L) return a.l / L < b.l / L;
			return a.r < b.r;
		}
	};
	LL bwork(int cl, int cr) {
		int l = cl, r = cl - 1;
		sum = 0;
		while (r < cr) r++, add(r, 1, l, r);
		LL res = getans();
		back();
		return res;
	}
	vector<LL> work(vector<PII> &qry) {
		int q = qry.size();
		vector<LL> ans(q);
		vector<query> seq;
		rep(i, 0, q) {
			if (qry[i].se - qry[i].fi <= L) ans[i] = bwork(qry[i].fi, qry[i].se);
			else seq.pb((query){qry[i].fi, qry[i].se, i});
		}
		sort(all(seq));
		int ql = 0, qr;
		q = seq.size();
		while (ql < q) {
			qr = ql;
			while (qr < q && seq[qr + 1].l / L == seq[ql].l / L) qr++;
			sum = 0;
			int l = seq[ql].l / L * L + L, r = l - 1;
			repn(i, ql, qr) {
				l = seq[ql].l / L * L + L;
				while (r < seq[i].r) ++r, add(r, 1, l, r);
				clear();
				while (l > seq[i].l) --l, add(l, 0, l, r);
				ans[seq[i].id] = getans();
				back();
			}
			ql = qr + 1;
		}
		return ans;
	}
}

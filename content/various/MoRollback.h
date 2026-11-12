/**
 * Author: itsjerr
 * Description: Alternative version of Mo's algorithm when del() is not possible.
 * Time: O((n + q)\sqrt n)
 * Status: stress-tested against brute force
 */
#pragma once

struct Ledger { // add() must change state only through lg.set()
	vector<pair<ll*, ll>> log;
	bool on = 0;
	void set(ll& x, ll v) { if (on) log.push_back({&x, x}); x = v; }
	void undo(int mark = 0) {
		while (sz(log) > mark)
			*log.back().first = log.back().second, log.pop_back();
	}
} lg;

void add(int i, int dir); // dir: 1 = right end, 0 = left end
void reset();              // reset all data structures of window
ll get();                  // e.g. count distinct: return d;

struct QR { int l, r, id; };
vector<ll> moRB(vector<QR> q, int n) {
	int B = max(1, (int)sqrt((double)n));
	vector<ll> ans(sz(q));
	vector<QR> big;
	for (QR& x : q) {
		int e = min(n, x.l / B * B + B);
		if (x.r >= e) { big.push_back(x); continue; }
		reset();
		rep(k,x.l,x.r+1) add(k, 1);
		ans[x.id] = get();
	}
	sort(all(big), [&](QR a, QR b) {
		if (a.l / B != b.l / B) return a.l < b.l;
		return a.r < b.r; });
	int r = -1, blk = -1;
	for (QR& x : big) {
		int e = min(n, x.l / B * B + B);
		if (x.l / B != blk)
			blk = x.l / B, reset(), r = e - 1;
		while (r < x.r) add(++r, 1);
		lg.on = 1;
		for (int k = e - 1; k >= x.l; k--) add(k, 0);
		ans[x.id] = get();
		lg.on = 0, lg.undo();
	}
	return ans;
}

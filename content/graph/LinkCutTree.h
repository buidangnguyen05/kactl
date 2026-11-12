/**
 * Author: Claude
 * Description: Represents a forest of rooted trees on nodes 1..n, all values 0
 * at first; the root of each tree decides what parent and subtree mean. You can
 * add and remove edges (as long as the result is still a forest), re-root a
 * tree, and add to or sum over any path or subtree. For path max/min instead,
 * apply the three \texttt{max:} notes (\texttt{sub\_*} and \texttt{comp\_*} then break and can
 * be dropped); subtree max is not possible, as \texttt{access()} removes a child by subtracting it.
 * Time: All operations take amortized O(\log N).
 * Status: stress-tested against a brute force for N <= 10000,
 * cross-checked against a second implementation at N = 10^6
 */
#pragma once

struct LCT {
	struct N {
		int p = 0, c[2] = {0, 0}, pp = 0, flip = 0;
		int cnt = 1, tsz = 1, vsz = 0;
		ll val = 0, sum = 0, ssum = 0, vsum = 0;
		ll plz = 0, slz = 0, vlz = 0, snap = 0;
	}; /// For path max/min: drop sub_* and comp_*, delete tsz/vsz/ssum/vsum/slz/vlz/snap and every line naming one, except the three "max:" lines, which you edit as noted.
	vector<N> t;
	LCT(int n) : t(n+1) { t[0].cnt=t[0].tsz=0; } // max: t[0].sum = -inf
	int dir(int x, int y) { return t[x].c[1] == y; }
	void ap(int x, ll v, bool sub) { /// add v to x: sub=0 only its path, sub=1 its whole subtree
		if (!x || !v) return;
		N& X = t[x];
		X.val += v; X.sum += v * X.cnt;  // max: X.sum += v
		X.ssum += v * (sub ? X.tsz : X.cnt);
		if (!sub) { X.plz += v; return; }
		X.vsum += v * X.vsz; X.slz += v; X.vlz += v;
	}
	void pull(int x) {
		N& X = t[x]; N& l = t[X.c[0]]; N& r = t[X.c[1]];
		X.cnt = l.cnt + r.cnt + 1;
		X.sum = l.sum + r.sum + X.val;  // max: max({l.sum, r.sum, X.val})
		X.tsz = l.tsz + r.tsz + X.vsz + 1;
		X.ssum = l.ssum + r.ssum + X.vsum + X.val;
	}
	void push(int x) {
		if (!x) return;
		N& X = t[x]; int &l = X.c[0], &r = X.c[1];
		if (X.flip) {
			X.flip = 0; swap(l, r); t[l].flip ^= 1; t[r].flip ^= 1;
		}
		if (X.slz) ap(l, X.slz, 1), ap(r, X.slz, 1), X.slz = 0;
		if (X.plz) ap(l, X.plz, 0), ap(r, X.plz, 0), X.plz = 0;
	}
	void set(int x, int d, int y) {
		if (x) t[x].c[d] = y, pull(x);
		if (y) t[y].p = x;
	}
	void rot(int x, int d) {
		int y = t[x].p, z = t[y].p, w = t[x].c[d];
		swap(t[x].pp, t[y].pp);
		swap(t[x].snap, t[y].snap);
		set(y, !d, w); set(x, d, y); set(z, dir(z, y), x);
	}
	void splay(int x) {
		for (push(x); t[x].p;) {
			int y = t[x].p, z = t[y].p; push(z); push(y); push(x);
			int dx = dir(y, x), dy = dir(z, y);
			if (!z) rot(x, !dx);
			else if (dx == dy) rot(y, !dx), rot(x, !dx);
			else rot(x, dy), rot(x, dx);
		}
	}
	void hang(int x, int v) {
		t[v].pp = x;
		t[v].snap = t[x].vlz;
	}
	int access(int u) {
		int last = u;
		for (int v = 0, x = u; x; x = t[v = x].pp) {
			splay(x); splay(v);
			ap(v, t[x].vlz - t[v].snap, 1);
			int r = t[x].c[1];
			t[x].vsz += t[r].tsz - t[v].tsz;
			t[x].vsum += t[r].ssum - t[v].ssum;
			t[v].pp = 0; if (r) t[r].p = 0, hang(x, r);
			set(x, 1, v); last = x;
		}
		splay(u); return last;
	}
	void make_root(int u) { // make u the root of its tree
		access(u); int l = t[u].c[0]; if (!l) return;
		t[l].flip ^= 1; t[l].p = 0; hang(u, l);
		t[u].vsz += t[l].tsz; t[u].vsum += t[l].ssum;
		set(u, 0, 0);
	}
	bool link(int u, int v) { // v's tree goes under u; false if same tree
		if (connected(u, v)) return false;
		make_root(v); access(u); hang(u, v);
		t[u].vsz += t[v].tsz; t[u].vsum += t[v].ssum;
		pull(u);
		return true;
	}
	bool cut(int u) { // drop the edge above u; false if u is a root
		access(u); int l = t[u].c[0];
		if (!l) return false;
		t[l].p = 0; t[u].c[0] = 0; pull(u); return true;
	}
	int find_root(int u) { // root of u's tree
		access(u); push(u);
		while (t[u].c[0]) u = t[u].c[0], push(u);
		splay(u); return u;
	}
	int parent(int u) { // parent of u, 0 if u is a root
		access(u); push(u); u = t[u].c[0]; push(u);
		while (t[u].c[1]) u = t[u].c[1], push(u);
		splay(u); return u;
	}
	bool connected(int u, int v) { return find_root(u) == find_root(v); }
	int depth(int u) { access(u); return t[u].cnt; } // #nodes on root..u
	int lca(int u, int v) { // 0 if u and v are in different trees
		if (u == v) return u;
		if (!connected(u, v)) return 0;
		if (depth(u) > depth(v)) swap(u, v);
		access(v); return access(u);
	}
	void upd(int u, ll x) { access(u); t[u].val += x; pull(u); } /// Add x at u.
	void path_upd(int u, int v, ll x) { // add x on u..v (connected)
		int r = find_root(u);
		make_root(u); access(v); ap(v, x, 0); make_root(r);
	}
	void sub_upd(int u, ll x) { // add x to u's whole subtree
		access(u); t[u].val += x;
		t[u].vlz += x; t[u].vsum += x * t[u].vsz;
		pull(u);
	}
	ll query(int u, int v) { // sum on u..v (connected)
		int r = find_root(u); make_root(u); access(v);
		ll ans = t[v].sum; make_root(r); return ans;
	}
	int sub_size(int u) { access(u); return t[u].vsz + 1; } /// Nodes under u, for the current root.
	ll sub_sum(int u) { access(u); return t[u].vsum + t[u].val; } /// Sum under u, for the current root.
	int comp_size(int u) { return t[find_root(u)].tsz; } /// Nodes in u's whole tree.
	ll comp_sum(int u) { return t[find_root(u)].ssum; } /// Sum over u's whole tree.
};
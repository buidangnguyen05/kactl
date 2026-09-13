/**
 * Author: Unknown
 * Date: Unknown
 * Source: https://cp-algorithms.com/data_structures/treap.html
 * Description: A short self-balancing tree. It acts as a
 *  sequential container with log-time splits/joins, and
 *  is easy to augment with additional data. Also supports
 *  range updates.
 * Time: $O(\log N)$ per split/merge
 * Status: stress-tested, tested on cses Substring Reversals and Cut and Paste
 */
#pragma once

struct Node {
	Node *l = 0, *r = 0;
	int val, y, c = 1, rev = 0;
	Node(int val) : val(val), y(rand()) {}
};

int cnt(Node* n) { return n ? n->c : 0; }
void pull(Node* n) { if (n) n->c = cnt(n->l) + cnt(n->r)+1; }
void push(Node* n) {
	if (!n) return;
	if (n->rev) {
		swap(n->l, n->r);
		if (n->l) n->l->rev ^= 1;
		if (n->r) n->r->rev ^= 1;
		n->rev = 0;
	}
}

template<class F> void each(Node* n, F f) {
	if (n) { push(n);each(n->l, f);f(n->val);each(n->r, f); }
}

pair<Node*, Node*> split(Node* n, int k) {
	if (!n) return {};
	if (cnt(n->l) >= k) { // "n->val >= k" for lower_bound(k)
		auto [L,R] = split(n->l, k);
		n->l = R;
		n->recalc();
		return {L, n};
	} else {
		auto [L,R] = split(n->r,k - cnt(n->l) - 1); // and just "k"
		n->r = L;
		n->recalc();
		return {n, R};
	}
}

Node* merge(Node* l, Node* r) {
	if (!l) return r;
	if (!r) return l;
	if (l->y > r->y) {
		l->r = merge(l->r, r);
		return l->recalc(), l;
	} else {
		r->l = merge(l, r->l);
		return r->recalc(), r;
	}
}

Node* ins(Node* t, Node* n, int pos) {
	auto [l,r] = split(t, pos);
	return merge(merge(l, n), r);
}

// Example application: move the range [l, r) to index k
void move(Node*& t, int l, int r, int k) {
	Node* a, * b, * c;
	split(t, a, c, r), split(a, a, b, l), merge(t, a, c);
	if (k<=l) insert(t, b, k);
	else insert(t, b, k - r + l);
}

// Example application: reverse the range [l, r)
void rev(Node*& t, int l, int r) {
	Node* a, * b, * c;
	split(t, a, c, r), split(a, a, b, l);
	b->rev ^= 1;
	merge(a, a, b), merge(t, a, c);
}
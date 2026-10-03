/**
 * Author: someone on Codeforces
 * Date: 2017-03-14
 * Source: folklore
 * Description: A short self-balancing tree. It acts as a
 *  sequential container with log-time splits/joins, and
 *  is easy to augment with additional data.
 *  \texttt{flip}/\texttt{push} add lazy range reversal; drop them
 *  if you only need a container.
 * Time: $O(\log N)$
 * Status: stress-tested
 */
#pragma once

struct Node {
	Node *l = 0, *r = 0;
	int val, y, c = 1, flip = 0;
	Node(int val) : val(val), y(rand()) {}
	void recalc();
};

int cnt(Node* n) { return n ? n->c : 0; }
void Node::recalc() { c = cnt(l) + cnt(r) + 1; }
void push(Node* n) {
	if (!n || !n->flip) return;
	swap(n->l, n->r); n->flip = 0;
	if (n->l) n->l->flip ^= 1;
	if (n->r) n->r->flip ^= 1;
}

template<class F> void each(Node* n, F f) {
	if (n) { push(n); each(n->l, f); f(n->val); each(n->r, f); }
}

pair<Node*, Node*> split(Node* n, int k) {
	if (!n) return {};
	push(n);
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
	push(l), push(r);
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
	Node *a, *b, *c;
	tie(a,b) = split(t, l); tie(b,c) = split(b, r - l);
	if (k <= l) t = merge(ins(a, b, k), c);
	else t = merge(a, ins(c, b, k - r));
}

// Example application: reverse the range [l, r)
void reverse(Node*& t, int l, int r) {
	Node *a, *b, *c;
	tie(a,b) = split(t, l); tie(b,c) = split(b, r - l);
	if (b) b->flip ^= 1;
	t = merge(merge(a, b), c);
}

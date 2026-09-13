// persistent IT, each update increases a[pos] by val

struct Vertex {
	Vertex *l, *r;
	int sum = 0;

	Vertex(int val) : l(nullptr), r(nullptr), sum(val) {}
	Vertex(Vertex *l, Vertex *r) : l(l), r(r), sum(0) {
		if (l) sum += l -> sum;
		if (r) sum += r -> sum;
	}
};
Vertex* root[N];

struct PersistentSegmentTree {
	int at[N];

	Vertex* build(int l, int r) {
		if (l == r) return new Vertex(at[l] = 0);
		int mid = (l + r) >> 1;
		return new Vertex(build(l, mid), build(mid + 1, r));
	}

	int get(Vertex* s, int l, int r, int u, int v) {
		if (l > v || r < u || u > v) return 0;
		if (l >= u && r <= v) return s -> sum;
		int mid = (l + r) >> 1;
		return get(s -> l, l, mid, u, v) + get(s -> r, mid + 1, r, u, v);
	}

	Vertex* up(Vertex* s, int l, int r, int pos, int val) {
		if (l == r) return new Vertex(at[pos] += val);
		int mid = (l + r) >> 1;
		if (pos <= mid) return new Vertex(up(s -> l, l, mid, pos, val), s -> r);
		return new Vertex(s -> l, up(s -> r, mid + 1, r, pos, val));
	}
} it;